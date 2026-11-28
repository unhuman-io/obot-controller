// The power board's radio module (STM32WBA5MMG): the one part of the robot
// that is always powered. It sleeps, listens for the remote for a moment
// every second, and watches the power button; it starts the converters (and
// with them the main controller, ../main); it hears the remote's red button
// and holds the motor bus off, by a line of its own, whenever it does not hear
// it say "out"; it draws the status screen and drives the three lights.
//
// What is decided is in power.logic (ControllerLogic), tested on a PC. This
// file is the pins and the messages around it; the processor and the radio
// are behind ../wba/port.h.
//
//   PC13 power button (low = down)     PB13 pairing button (low = down; held: forget the remote)
//   PB2  DCDC_ON, PB15 V5_ON           the 12 V converter, the 5 V converter (= the main controller's supply)
//   PB0  BUS_KILL, open drain          low = the bus eFuse's enable held low
//   PA8  G5_RST, open drain            low = the main controller in reset       PA9  G5_BOOT: high at its reset = its boot loader
//   PB5 / PA10  LPUART1                link to the main controller
//   PB12 USART1, one wire              the dock's data contact
//   PA3 red, PA4 green, PB14 blue      lights, lit when low
//   PA0 / PB8 SPI3, PA5 CS, PB9 DISP   display
//   PA1  ADC                           battery voltage, 2.2 M / 100 k
#include <cstring>
#include "../wba/display.h"
#include "port.h"
import power.aes;
import power.messages;
import power.logic;
import power.dock;
import power.screen;

namespace {
ControllerLogic logic;
Display display;

Pairing pairing;
Aes128 key;
uint8_t own_address[6];

// what the remote said last, and when
uint32_t session, last_counter;
RemoteStatus remote;
uint32_t heard_ms, session_ms;
bool heard;

// the main controller
LinkReceiver from_main;
MainReport main_report;
uint32_t main_ms;
bool main_heard;

// the dock
LinkReceiver from_dock;
DockBoard dock;
uint32_t dock_ms;
bool dock_seen, dock_wants_charge;

uint16_t battery_cv;
uint32_t measured_ms, reported_ms, sent_ms;
int listening = -1;             // 0 not, 1 a window a second, 2 all the time
ScreenData shown;
bool drawn;

void new_session() {
    session = port_random();
    last_counter = 0;
    session_ms = port_ms();
}

void pairing_changed() {
    uint8_t record[pairing_record_size];
    pack(pairing, record);
    // the flash and the radio do not share time well: nothing on the air while it is written
    port_radio_send(nullptr, 0, 0);
    port_radio_listen(nullptr, 0, 0);
    listening = -1;
    port_store_write(record, sizeof record);
    if (pairing.valid) key.set_key(pairing.key);
    heard = false;
    new_session();
}

void dock_poll(uint32_t now) {
    uint8_t byte;
    port_dock_poll();
    while (port_dock_read(&byte)) {
        if (!from_dock.put(byte)) continue;
        uint8_t random[16], reply[link_frame_max];
        for (int k = 0; k < 4; k++) {
            const uint32_t word = port_random();
            std::memcpy(random + 4 * k, &word, 4);
        }
        bool changed, docked, wants;
        const std::size_t length = dock.receive(from_dock.kind(), from_dock.payload(), from_dock.length(), own_address, random, pairing, changed, docked, wants, reply);
        if (docked) {
            dock_seen = true;
            dock_wants_charge = wants;
            dock_ms = now;
        }
        if (changed) pairing_changed();
        if (length != 0) port_dock_write(reply, length);
    }
    if (dock_seen && now - dock_ms > 3000) dock_seen = dock_wants_charge = false;
}
}  // namespace

extern "C" void app_radio_received(const uint8_t* frame, uint8_t length, int8_t) {
    RemoteStatus status;
    if (!pairing.valid || !take_heartbeat(key, session, last_counter, frame, length, status)) return;
    remote = status;
    heard_ms = port_ms();
    heard = true;
}

extern "C" void app_init(void) {
    port_pin_output('B', 0, true, false);       // the bus held off
    port_pin_output('B', 2, false, false);      // converters off
    port_pin_output('B', 15, false, false);
    port_pin_output('A', 8, true, true);        // the main controller not held in reset
    port_pin_output('A', 9, false, false);      // and to start its program, not its boot loader
    port_pin_output('A', 3, false, true);       // lights off
    port_pin_output('A', 4, false, true);
    port_pin_output('B', 14, false, true);
    port_pin_wake('C', 13, 0);                  // power button: pulled up on the board
    port_pin_input('B', 13, 0);                 // pairing button: pulled up on the board
    port_link_init();
    port_dock_init();
    display.start('A', 5, 'B', 9);

    uint8_t record[pairing_record_size];
    port_radio_address(own_address);
    if (port_store_read(record, sizeof record)) pairing = unpack_pairing(record);
    if (pairing.valid) key.set_key(pairing.key);
    new_session();
    port_tick_period(20);
}

extern "C" void app_tick(void) {
    const uint32_t now = port_ms();
    uint8_t byte;
    while (port_link_read(&byte)) {
        if (from_main.put(byte) && from_main.kind() == link_kind_main && from_main.length() == main_report_size) {
            main_report = unpack_main_report(from_main.payload());
            main_ms = now;
            main_heard = true;
        }
    }
    dock_poll(now);
    if (now - measured_ms >= 500 || measured_ms == 0) {
        battery_cv = static_cast<uint16_t>(static_cast<uint32_t>(port_adc_mv()) * 23 / 10);
        measured_ms = now;
    }

    ControllerInputs in;
    in.now_ms = now;
    in.power_button = !port_pin_read('C', 13);
    in.pair_button = !port_pin_read('B', 13);
    in.remote_fresh = heard && now - heard_ms < heartbeat_timeout_ms;
    in.remote = remote;
    in.main_fresh = main_heard && now - main_ms < main_timeout_ms;
    in.main = main_report;
    in.dock_charge_wanted = dock_wants_charge;
    in.battery_cv = battery_cv;
    const ControllerOutputs out = logic.update(in);

    port_pin_write('B', 0, !out.bus_kill);      // open drain: high lets go
    port_pin_write('B', 2, out.converters_on);
    port_pin_write('B', 15, out.converters_on);

    if (out.forget_remote && pairing.valid) {
        pairing = Pairing{};
        pairing_changed();
    }
    // One new session number when the remote has been silent for a second
    // (see power.messages): what was recorded before is then worth nothing, and
    // a remote that started again and counts from 1 is taken again.
    if (heard && now - heard_ms > 1000 && session_ms - heard_ms > 0x80000000u) new_session();

    const bool on = out.converters_on;
    if (on && now - reported_ms >= 50) {        // to the main controller, 20 a second
        uint8_t frame[link_frame_max];
        port_link_write(frame, make_link_frame(out.report, frame));
        reported_ms = now;
    }
    // On the air: our status every 100 ms while the robot is on and every second
    // while it is off; listening all the time while on, 30 ms of every second off.
    const int listen = !pairing.valid ? 0 : on ? 2
                                               : 1;
    if (listen != listening) {
        if (listen == 0) {
            port_radio_listen(nullptr, 0, 0);
            port_radio_send(nullptr, 0, 0);
        } else {
            port_radio_listen(pairing.peer, listen == 2 ? 100 : 1000, listen == 2 ? 100 : 30);
        }
        listening = listen;
        sent_ms = now - 1000;
    }
    if (pairing.valid && now - sent_ms >= (on ? 100u : 1000u)) {
        uint8_t frame[status_frame_size];
        make_status_frame(key, session, out.status, frame);
        port_radio_send(frame, sizeof frame, on ? 100 : 1000);
        sent_ms = now;
    }

    const Lights light = lights(out.status.state, in.remote_fresh, false, true, now);
    port_pin_write('A', 3, !light.red);
    port_pin_write('A', 4, !light.green);
    port_pin_write('B', 14, !light.blue);

    const ScreenData screen{out.status, remote, in.remote_fresh, false};
    const bool same = drawn && screen.robot == shown.robot && screen.remote == shown.remote && screen.linked == shown.linked;
    if (!same || now - display.flipped_ms >= 500) {
        display.show(screen, now);
        shown = screen;
        drawn = true;
    }

    // Off, nothing pressed, nobody on the dock: asleep between the radio's
    // windows, awake twice a second for the display. Else: every 20 ms, clocks on.
    const bool idle = !on && !in.power_button && !in.pair_button && !dock_seen;
    port_allow_stop(idle);
    port_tick_period(idle ? 500 : 20);
}
