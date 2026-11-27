// The e-stop remote (STM32WBA5MMG, one lithium cell): the red button, the
// pause button, the status screen and three lights. While there is a robot
// that is on it says twenty or more times a second where the red button is;
// when there is none it sleeps, and a button or the dock wakes it.
//
// What is decided is in power.logic (RemoteLogic), tested on a PC. This file
// is the pins and the messages around it; the processor and the radio are
// behind ../wba/port.h.
//
//   PB2  red button, normally closed contact (low = closed = button out)     PB1  its normally open contact (low = closed = button in)
//   PC13 pause button (low = down)       PB6  on the dock (high)             PB0  charging (low)
//   PB12 USART1, one wire                the dock's data contact
//   PA3 red, PA4 green, PB14 blue        lights, lit when low
//   PA0 / PB8 SPI3, PA5 CS, PB9 DISP     display
//   PA1  ADC                             cell voltage, 1 M / 1 M
#include <cstring>
#include "../wba/display.h"
#include "port.h"
import power.aes;
import power.messages;
import power.logic;
import power.dock;
import power.screen;

namespace {
RemoteLogic logic;
Display display;

Pairing pairing;
Aes128 key;
uint8_t own_address[6];

// what the robot said last, and when
uint32_t session, counter;
RobotStatus robot;
uint32_t heard_ms;
bool heard;

LinkReceiver from_dock;
DockRemote dock;
uint32_t hello_ms;

uint16_t cell_mv;
uint32_t measured_ms;
int listening = -1;             // 0 not, 1 a fifth of the time, 2 all the time
ScreenData shown;
bool drawn;

void dock_poll(uint32_t now, bool docked, bool wants_charge) {
    uint8_t byte, frame[link_frame_max];
    if (!docked) return;
    port_dock_poll();
    while (port_dock_read(&byte)) {
        if (!from_dock.put(byte)) continue;
        bool changed;
        const std::size_t length = dock.receive(from_dock.kind(), from_dock.payload(), from_dock.length(), own_address, pairing, changed, frame);
        if (changed) {
            uint8_t record[pairing_record_size];
            pack(pairing, record);
            port_radio_send(nullptr, 0, 0);     // nothing on the air while the flash is written
            port_radio_listen(nullptr, 0, 0);
            listening = -1;
            port_store_write(record, sizeof record);
            key.set_key(pairing.key);
            heard = false;
        }
        if (length != 0) port_dock_write(frame, length);
    }
    if (now - hello_ms >= 500) {
        port_dock_write(frame, dock.hello(pairing, own_address, wants_charge, frame));
        hello_ms = now;
    }
}
}  // namespace

extern "C" void app_radio_received(const uint8_t* frame, uint8_t length, int8_t) {
    uint32_t said;
    RobotStatus status;
    if (!pairing.valid || !take_status_frame(key, frame, length, said, status)) return;
    if (!heard || said != session) {
        session = said;         // a new session: count from the start
        counter = 0;
    }
    robot = status;
    heard_ms = port_ms();
    heard = true;
}

extern "C" void app_init(void) {
    port_pin_output('A', 3, false, true);       // lights off
    port_pin_output('A', 4, false, true);
    port_pin_output('B', 14, false, true);
    port_pin_wake('B', 2, 0);                   // the buttons are pulled up on the board
    port_pin_wake('B', 1, 0);
    port_pin_wake('C', 13, 0);
    port_pin_wake('B', 6, 0);                   // dock detect: a divider from the dock's 5 V
    port_pin_input('B', 0, 0);
    port_dock_init();
    display.start('A', 5, 'B', 9);

    uint8_t record[pairing_record_size];
    port_radio_address(own_address);
    if (port_store_read(record, sizeof record)) pairing = unpack_pairing(record);
    if (pairing.valid) key.set_key(pairing.key);
    port_tick_period(20);
}

extern "C" void app_tick(void) {
    const uint32_t now = port_ms();
    if (now - measured_ms >= 1000 || measured_ms == 0) {
        cell_mv = static_cast<uint16_t>(port_adc_mv() * 2);
        measured_ms = now;
    }
    const bool linked = heard && now - heard_ms < 3000;

    RemoteInputs in;
    in.now_ms = now;
    in.contact_a_closed = !port_pin_read('B', 2);
    in.contact_b_closed = !port_pin_read('B', 1);
    in.pause_button = !port_pin_read('C', 13);
    in.docked = port_pin_read('B', 6);
    in.charging = !port_pin_read('B', 0);
    in.cell_mv = cell_mv;
    in.linked = linked;
    in.robot = robot;
    const RemoteOutputs out = logic.update(in);

    dock_poll(now, in.docked, out.status.cell_level < 5);

    // On the air: listening all the time until the robot is heard, then a fifth
    // of the time (its status comes ten times a second; one in a second is
    // enough here, and listening is what costs the cell); the heartbeat every
    // 20 ms once the robot has given the session number to make it with.
    const bool radio = pairing.valid && out.radio_on;
    const int listen = !radio ? 0 : linked ? 1
                                           : 2;
    if (listen != listening) {
        if (listen == 0) {
            port_radio_listen(nullptr, 0, 0);
            port_radio_send(nullptr, 0, 0);
        } else {
            port_radio_listen(pairing.peer, listen == 1 ? 200 : 100, listen == 1 ? 40 : 100);
        }
        listening = listen;
    }
    if (radio && linked) {
        uint8_t frame[heartbeat_size];
        make_heartbeat(key, session, ++counter, out.status, frame);
        port_radio_send(frame, sizeof frame, 20);
    } else if (listening != 0) {
        port_radio_send(nullptr, 0, 0);
    }

    const Lights light = lights(robot.state, linked, true, out.radio_on, now);
    port_pin_write('A', 3, !light.red);
    port_pin_write('A', 4, !light.green);
    port_pin_write('B', 14, !light.blue);

    const ScreenData screen{linked ? robot : RobotStatus{}, out.status, linked, true};
    const bool same = drawn && screen.robot == shown.robot && screen.remote == shown.remote && screen.linked == shown.linked;
    if (!same || now - display.flipped_ms >= 500) {
        display.show(screen, now);
        shown = screen;
        drawn = true;
    }

    // Asleep between the radio's events whenever the dock's UART is not in use;
    // with the radio off too, awake only twice a second for the display, and
    // when a button or the dock wakes it.
    port_allow_stop(!in.docked);
    port_tick_period(out.radio_on || in.docked || in.pause_button ? 20 : 500);
}
