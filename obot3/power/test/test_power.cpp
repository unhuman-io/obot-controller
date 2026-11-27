// Host test of the power board's hardware-independent modules: power/test/run.sh
// With a directory as its argument it also writes the status screens there as
// pictures (.pbm), to look at.
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <string>
#include <string_view>
import power.frame;
import power.font;
import power.aes;
import power.messages;
import power.logic;
import power.screen;

static int failures = 0;
#define CHECK(condition) \
    do { \
        if (!(condition)) { \
            failures++; \
            std::printf("FAIL  %s:%d  %s\n", __FILE__, __LINE__, #condition); \
        } \
    } while (0)

static void bytes(const char* hex, uint8_t* out) {
    for (int k = 0; hex[2 * k]; k++) {
        unsigned value;
        std::sscanf(hex + 2 * k, "%2x", &value);
        out[k] = static_cast<uint8_t>(value);
    }
}

static void test_aes() {
    // FIPS 197, appendix C.1
    uint8_t key[16], plain[16], expected[16], out[16];
    bytes("000102030405060708090a0b0c0d0e0f", key);
    bytes("00112233445566778899aabbccddeeff", plain);
    bytes("69c4e0d86a7b0430d8cdb78070b4c55a", expected);
    Aes128(key).encrypt(plain, out);
    CHECK(std::memcmp(out, expected, 16) == 0);
    // ... and at compile time
    static constexpr uint8_t zero_key[16]{};
    static constexpr Aes128 zero(zero_key);
    static_assert(aes_table.box[0x53] == 0xED && aes_table.box[0x00] == 0x63 && aes_table.box[0xFF] == 0x16);
    CHECK(zero.round_key[1][0] == 0x62);        // 00..00 expands to 62636363 ...

    // RFC 4493, section 4
    uint8_t message[64], mac[16];
    bytes("2b7e151628aed2a6abf7158809cf4f3c", key);
    bytes("6bc1bee22e409f96e93d7e117393172aae2d8a571e03ac9c9eb76fac45af8e51"
          "30c81c46a35ce411e5fbc1191a0a52eff69f2445df4f9b17ad2b417be66c3710", message);
    const Aes128 aes(key);
    const struct {
        std::size_t length;
        const char* mac;
    } cases[] = {{0, "bb1d6929e95937287fa37d129b756746"}, {16, "070a16b46b4d4144f79bdd9dd04a287c"},
                 {40, "dfa66747de9ae63030ca32611497c827"}, {64, "51f0bebf7e3b9d92fc49741779363cfe"}};
    for (const auto& c : cases) {
        bytes(c.mac, expected);
        aes_cmac(aes, message, c.length, mac);
        CHECK(std::memcmp(mac, expected, 16) == 0);
    }
}

static void test_display() {
    Frame frame;
    frame.clear();
    CHECK(!frame.black(0, 0) && frame.line[5][3] == 0xFF);
    frame.set(0, 0);
    frame.set(9, 2);
    frame.set(127, 127);
    frame.set(128, 0);      // outside: nothing
    frame.set(-1, 5);
    CHECK(frame.line[0][0] == 0xFE && frame.line[2][1] == 0xFD && frame.line[127][15] == 0x7F);    // the leftmost pixel is bit 0
    CHECK(frame.black(9, 2) && !frame.black(8, 2) && !frame.black(200, 2));

    // the bytes for lines 2 and 3 (numbered 3 and 4 on the wire)
    uint8_t out[lcd_transfer_max];
    const std::size_t n = lcd_lines(frame, 2, 3, true, out);
    CHECK(n == 1 + 2 * 18 + 1 && out[0] == 0x03 && out[1] == 3 && out[3] == 0xFD && out[18] == 0 && out[19] == 4 && out[n - 1] == 0);
    CHECK(lcd_lines(frame, 0, 127, false, out) == lcd_transfer_max && out[0] == 0x01 && out[1] == 1);
    CHECK(lcd_hold(true, out) == 2 && out[0] == 0x02 && out[1] == 0);
    CHECK(lcd_hold(false, out) == 2 && out[0] == 0x00);
    CHECK(lcd_clear(false, out) == 2 && out[0] == 0x04);

    // only what changed is sent
    LcdShadow shadow;
    LcdShadow::Run run = shadow.next(frame);
    CHECK(run.first == 0 && run.last == 127);                       // nothing known: everything
    shadow.sent(frame, run);
    shadow.known = true;
    CHECK(shadow.next(frame).empty());
    frame.set(4, 10);
    frame.set(4, 12);       // one line between: sent along
    frame.set(4, 40);
    run = shadow.next(frame);
    CHECK(run.first == 10 && run.last == 12);
    shadow.sent(frame, run);
    run = shadow.next(frame, run.last + 1);
    CHECK(run.first == 40 && run.last == 40);
    shadow.sent(frame, run);
    CHECK(shadow.next(frame).empty());

    // letters and numbers
    CHECK(text_width("RUN", 3) == 3 * 18 - 3 && text_width("") == 0);
    Frame text;
    text.clear();
    CHECK(draw_text(text, 0, 0, "1") == 6);
    CHECK(!text.black(0, 0) && text.black(2, 0) && text.black(1, 1) && text.black(1, 6) && text.black(3, 6) && !text.black(4, 6));
    CHECK(find_glyph('q') == find_glyph('Q') && find_glyph(' ') == nullptr && find_glyph('~') == nullptr);
    const auto number = [](int32_t milli, int decimals) {
        char buffer[16];
        return std::string(buffer, static_cast<std::size_t>(format_milli(milli, decimals, buffer)));
    };
    CHECK(number(54230, 1) == "54.2" && number(54250, 1) == "54.3" && number(-3140, 1) == "-3.1" && number(0, 1) == "0.0");
    CHECK(number(168000, 0) == "168" && number(-40, 1) == "0.0" && number(999, 0) == "1" && number(50, 2) == "0.05");
}

static void test_messages() {
    RobotStatus robot{.state = RobotState::RUNNING, .faults = fault::MONITOR, .bus_on = true, .computer_on = true, .remote_linked = true,
                      .battery_cv = 5423, .battery_ca = -314, .bus_cv = 5410, .battery_percent = 80};
    uint8_t buffer[32];
    uint8_t* at = buffer;
    pack(robot, at);
    CHECK(static_cast<std::size_t>(at - buffer) == robot_status_size);
    const uint8_t* from = buffer;
    CHECK(unpack_robot_status(from) == robot);
    RemoteStatus remote{.stop = StopSwitch::RUN, .pause_presses = 7, .power_request = true, .docked = true, .charging = true, .cell_level = 4, .cell_mv = 3910};
    at = buffer;
    pack(remote, at);
    CHECK(static_cast<std::size_t>(at - buffer) == remote_status_size);
    from = buffer;
    CHECK(unpack_remote_status(from) == remote);
    // a stop value that nobody defined is not RUN
    buffer[0] = 9;
    from = buffer;
    CHECK(unpack_remote_status(from).stop == StopSwitch::BROKEN);

    // a heartbeat is taken from the remote with our key, for this session, once
    uint8_t key_bytes[16], other_bytes[16];
    bytes("2b7e151628aed2a6abf7158809cf4f3c", key_bytes);
    bytes("2b7e151628aed2a6abf7158809cf4f3d", other_bytes);
    const Aes128 key(key_bytes), other(other_bytes);
    uint8_t frame[heartbeat_size];
    make_heartbeat(key, 0x1234, 5, remote, frame);
    uint32_t last = 4;
    RemoteStatus got;
    CHECK(take_heartbeat(key, 0x1234, last, frame, sizeof frame, got) && got == remote && last == 5);
    CHECK(!take_heartbeat(key, 0x1234, last, frame, sizeof frame, got));                // the same again: no
    last = 0;
    CHECK(!take_heartbeat(key, 0x1235, last, frame, sizeof frame, got) && last == 0);   // another session
    CHECK(!take_heartbeat(other, 0x1234, last, frame, sizeof frame, got));              // another key
    CHECK(!take_heartbeat(key, 0x1234, last, frame, sizeof frame - 1, got));
    for (std::size_t k = 0; k < sizeof frame; k++) {                                     // any bit changed
        frame[k] ^= 0x10;
        CHECK(!take_heartbeat(key, 0x1234, last, frame, sizeof frame, got));
        frame[k] ^= 0x10;
    }
    CHECK(take_heartbeat(key, 0x1234, last, frame, sizeof frame, got));
    CHECK(heartbeat_size <= 20 && status_frame_size <= 20);

    uint8_t status[status_frame_size];
    make_status_frame(key, 0xCAFE0001, robot, status);
    uint32_t session = 0;
    RobotStatus heard;
    CHECK(take_status_frame(key, status, sizeof status, session, heard) && session == 0xCAFE0001 && heard == robot);
    session = 7;
    CHECK(!take_status_frame(other, status, sizeof status, session, heard) && session == 7);
    status[5] ^= 1;
    CHECK(!take_status_frame(key, status, sizeof status, session, heard));

    // the two processors of the board, over their UART
    CHECK(crc16(reinterpret_cast<const uint8_t*>("123456789"), 9) == 0x29B1);
    const RadioReport radio{.state = RobotState::PAUSED, .faults = fault::STOP_WIRING, .allow_bus = true, .computer_on = true, .pause = true,
                            .remote_linked = true, .remote_docked = false, .stop = StopSwitch::RUN, .remote_cell_level = 3};
    const MainReport main{.faults = fault::OUTPUT, .bus_enabled = true, .bus_on = true, .outputs_good = false, .computer_on = true, .battery_cv = 5423,
                          .battery_ca = -314, .bus_cv = 5410, .out12_ca = 250, .out5_ca = 120, .energy_mwh = 123456};
    uint8_t link[link_frame_max * 2 + 8];
    std::size_t n = 0;
    link[n++] = 0x00;                       // noise
    link[n++] = 0xA5;                       // a start with a length no frame has
    link[n++] = 0x01;
    link[n++] = 0xFF;
    n += make_link_frame(radio, link + n);
    const std::size_t second = n;
    n += make_link_frame(main, link + n);
    CHECK(n - second == 3 + main_report_size + 2 && second - 4 == 3 + radio_report_size + 2);
    LinkReceiver receiver;
    int frames = 0;
    for (std::size_t k = 0; k < n; k++) {
        if (!receiver.put(link[k])) continue;
        frames++;
        if (receiver.kind() == link_kind_radio) CHECK(receiver.length() == radio_report_size && unpack_radio_report(receiver.payload()) == radio);
        if (receiver.kind() == link_kind_main) CHECK(receiver.length() == main_report_size && unpack_main_report(receiver.payload()) == main);
    }
    CHECK(frames == 2);
    link[second + 5] ^= 0x40;               // one bit wrong in the second frame
    frames = 0;
    for (std::size_t k = second; k < n; k++) frames += receiver.put(link[k]);
    CHECK(frames == 0);
}

// The power board's radio module through a day: `step` moves the time on and
// gives the outputs.
struct Controller {
    ControllerLogic logic;
    ControllerInputs in;
    ControllerOutputs out;
    ControllerOutputs& step(uint32_t ms) {
        for (uint32_t t = 0; t < ms; t += 10) {
            in.now_ms += 10;
            out = logic.update(in);
        }
        return out;
    }
    void press(bool ControllerInputs::* button, uint32_t ms) {
        in.*button = true;
        step(ms);
        in.*button = false;
        step(50);
    }
};

static void test_controller() {
    Controller c;
    c.in.now_ms = 0xFFFFF000;                   // the time wraps in the middle of this
    c.step(100);
    CHECK(c.out.status.state == RobotState::OFF && !c.out.converters_on && c.out.bus_kill && !c.out.report.computer_on);

    c.press(&ControllerInputs::power_button, 100);
    CHECK(c.out.status.state == RobotState::STARTING && c.out.converters_on && c.out.bus_kill);
    c.in.main_fresh = true;
    c.in.main = {.bus_on = false, .computer_on = true, .battery_cv = 5400, .battery_ca = 30, .bus_cv = 0};
    c.step(50);
    CHECK(c.out.status.state == RobotState::STOPPED && c.out.bus_kill && c.out.report.computer_on && !c.out.report.allow_bus);
    CHECK(c.out.status.battery_cv == 5400 && c.out.status.computer_on && !c.out.status.remote_linked);

    // the remote comes, its button out: the bus may come on, the robot is paused
    c.in.remote_fresh = true;
    c.in.remote = {.stop = StopSwitch::RUN, .pause_presses = 200};
    c.step(50);
    CHECK(c.out.status.state == RobotState::PAUSED && !c.out.bus_kill && c.out.report.allow_bus && c.out.report.pause);
    c.in.remote.pause_presses++;
    c.step(20);
    CHECK(c.out.status.state == RobotState::RUNNING && !c.out.report.pause);
    c.in.remote.pause_presses++;
    c.step(20);
    CHECK(c.out.status.state == RobotState::PAUSED);
    c.in.remote.pause_presses++;
    c.step(20);
    CHECK(c.out.status.state == RobotState::RUNNING);

    // out of range: off at once, and paused when it is back
    c.in.remote_fresh = false;
    c.step(10);
    CHECK(c.out.status.state == RobotState::STOPPED && c.out.bus_kill && !c.out.report.allow_bus);
    c.in.remote_fresh = true;
    c.in.remote.pause_presses += 3;             // pressed while away: counts for nothing
    c.step(10);
    CHECK(c.out.status.state == RobotState::PAUSED && !c.out.bus_kill);
    c.in.remote.pause_presses++;
    c.step(10);
    CHECK(c.out.status.state == RobotState::RUNNING);

    // the red button, and a wire off it
    c.in.remote.stop = StopSwitch::STOP;
    c.step(10);
    CHECK(c.out.status.state == RobotState::STOPPED && c.out.bus_kill && c.out.status.faults == 0);
    c.in.remote.stop = StopSwitch::BROKEN;
    c.step(10);
    CHECK(c.out.status.state == RobotState::STOPPED && c.out.bus_kill && c.out.status.faults == fault::STOP_WIRING);
    c.in.remote.stop = StopSwitch::RUN;
    c.step(10);
    CHECK(c.out.status.state == RobotState::PAUSED);

    // faults of the main controller, and its silence
    c.in.main.faults = fault::OUTPUT;           // does not take the bus
    c.step(10);
    CHECK(c.out.status.state == RobotState::PAUSED && c.out.status.faults == fault::OUTPUT);
    c.in.main.faults = fault::BATTERY_LOW;
    c.step(10);
    CHECK(c.out.status.state == RobotState::FAULT && c.out.bus_kill);
    c.in.main.faults = 0;
    c.in.main_fresh = false;
    c.step(10);
    CHECK(c.out.status.state == RobotState::FAULT && c.out.bus_kill && c.out.status.faults == fault::MAIN_SILENT);
    c.in.main_fresh = true;
    c.step(10);
    CHECK(c.out.status.state == RobotState::PAUSED);

    // held: the computer is told and keeps its supply for a while, the bus goes at once
    c.in.power_button = true;
    c.step(long_press_ms + 100);
    c.in.power_button = false;
    c.step(100);
    CHECK(c.out.status.state == RobotState::SHUTTING_DOWN && c.out.bus_kill && c.out.report.computer_on && c.out.converters_on);
    c.step(shutdown_grace_ms - 2000);
    CHECK(c.out.status.state == RobotState::SHUTTING_DOWN);
    c.step(2000);
    CHECK(c.out.status.state == RobotState::OFF && !c.out.converters_on && !c.out.report.computer_on);

    // PAUSE held on the remote: on; still said afterwards: nothing more; said again: off
    c.in.remote.power_request = true;
    c.step(500);
    CHECK(c.out.status.state == RobotState::PAUSED);
    c.in.remote.power_request = false;
    c.step(100);
    CHECK(c.out.status.state == RobotState::PAUSED);
    c.in.remote.power_request = true;
    c.step(100);
    CHECK(c.out.status.state == RobotState::SHUTTING_DOWN);
    c.in.remote.power_request = false;
    c.step(shutdown_grace_ms + 100);
    CHECK(c.out.status.state == RobotState::OFF);
    // ... also when that is the first thing the controller hears from it (it woke the remote up)
    c.in.remote_fresh = false;
    c.step(1000);
    c.in.remote.power_request = true;
    c.in.remote_fresh = true;
    c.step(50);
    CHECK(c.out.status.state != RobotState::OFF);
    c.in.power_button = true;
    c.step(long_press_ms + 100);                // (and off again, by the button, before the main controller is there)
    c.in.power_button = false;
    c.in.remote_fresh = false;
    c.in.main_fresh = false;
    c.step(shutdown_grace_ms + 100);
    CHECK(c.out.status.state == RobotState::OFF);

    // the remote on the dock wants charging: converters, nothing else
    c.in.dock_charge_wanted = true;
    c.step(50);
    CHECK(c.out.status.state == RobotState::CHARGING && c.out.converters_on && c.out.bus_kill && !c.out.report.computer_on);
    c.in.dock_charge_wanted = false;
    c.step(50);
    CHECK(c.out.status.state == RobotState::OFF && !c.out.converters_on);

    // no main controller after starting: on, bus off, and it says why
    c.press(&ControllerInputs::power_button, 100);
    c.step(start_timeout_ms + 100);
    CHECK(c.out.status.state == RobotState::FAULT && c.out.status.faults == fault::MAIN_SILENT && c.out.bus_kill);

    // the pairing button held: forget, once
    int forgets = 0;
    c.in.pair_button = true;
    for (int k = 0; k < 800; k++) forgets += c.step(10).forget_remote;
    c.in.pair_button = false;
    CHECK(forgets == 1);
    c.press(&ControllerInputs::pair_button, 500);
    CHECK(!c.out.forget_remote);
}

struct Remote {
    RemoteLogic logic;
    RemoteInputs in;
    RemoteOutputs out;
    RemoteOutputs& step(uint32_t ms) {
        for (uint32_t t = 0; t < ms; t += 10) {
            in.now_ms += 10;
            out = logic.update(in);
        }
        return out;
    }
};

static void test_remote() {
    Remote r;
    r.in.now_ms = 0xFFFFFF00;
    r.in.cell_mv = 3800;
    // woken with the red button in
    r.in.contact_a_closed = false;
    r.in.contact_b_closed = true;
    r.step(100);
    CHECK(r.out.status.stop == StopSwitch::STOP && r.out.radio_on && r.out.status.cell_level == 3);
    // let out: RUN after a moment, not at once
    r.in.contact_a_closed = true;
    r.in.contact_b_closed = false;
    r.step(20);
    CHECK(r.out.status.stop == StopSwitch::STOP);
    r.step(60);
    CHECK(r.out.status.stop == StopSwitch::RUN);
    // pressed: STOP in the same step, also while the contacts are between
    r.in.contact_a_closed = false;
    r.step(10);
    CHECK(r.out.status.stop == StopSwitch::STOP);
    r.in.contact_b_closed = true;
    r.step(10);
    CHECK(r.out.status.stop == StopSwitch::STOP);
    // a wire off the normally closed contact with the button out: both open, for good
    r.in.contact_b_closed = false;
    r.step(stop_broken_ms + 50);
    CHECK(r.out.status.stop == StopSwitch::BROKEN);
    // ... and both closed
    r.in.contact_a_closed = true;
    r.in.contact_b_closed = true;
    r.step(stop_broken_ms + 50);
    CHECK(r.out.status.stop == StopSwitch::BROKEN);
    r.in.contact_b_closed = false;
    r.step(100);
    CHECK(r.out.status.stop == StopSwitch::RUN);

    // PAUSE: short counts, long asks for power until the robot shows it heard
    const uint8_t presses = r.out.status.pause_presses;
    r.in.pause_button = true;
    r.step(200);
    r.in.pause_button = false;
    r.step(100);
    CHECK(r.out.status.pause_presses == static_cast<uint8_t>(presses + 1) && !r.out.status.power_request);
    r.in.pause_button = true;
    r.step(long_press_ms + 100);
    CHECK(r.out.status.power_request && r.out.status.pause_presses == static_cast<uint8_t>(presses + 1));
    r.in.pause_button = false;
    r.step(100);
    CHECK(r.out.status.power_request && r.out.status.pause_presses == static_cast<uint8_t>(presses + 1));   // no short press after a long one
    r.in.linked = true;
    r.in.robot.state = RobotState::OFF;
    r.step(100);
    CHECK(r.out.status.power_request);
    r.in.robot.state = RobotState::STARTING;
    r.step(20);
    CHECK(!r.out.status.power_request);
    // nobody heard: it stops asking by itself
    r.in.linked = false;
    r.in.pause_button = true;
    r.step(long_press_ms + 100);
    r.in.pause_button = false;
    CHECK(r.out.status.power_request);
    r.step(power_request_ms);
    CHECK(!r.out.status.power_request);

    // the radio: on while a robot that is on is heard, however long; else off some time after the last press
    r.in.linked = true;
    r.in.robot.state = RobotState::RUNNING;
    r.step(remote_awake_ms * 3);
    CHECK(r.out.radio_on);
    r.in.robot.state = RobotState::OFF;
    r.step(remote_awake_ms + 100);
    CHECK(!r.out.radio_on);
    r.in.linked = false;
    r.in.docked = true;                         // put on the dock: awake again for a while
    r.step(100);
    CHECK(r.out.radio_on && r.out.status.docked);
    r.step(remote_awake_ms + 100);
    CHECK(!r.out.radio_on);

    CHECK(cell_level(3300) == 0 && cell_level(3400) == 1 && cell_level(3699) == 2 && cell_level(3900) == 4 && cell_level(4150) == 5);
}

static void test_lights() {
    CHECK(lights(RobotState::RUNNING, true, false, true, 500) == (Lights{.green = true}));
    CHECK(lights(RobotState::PAUSED, true, false, true, 500) == (Lights{.red = true, .green = true}));
    CHECK(lights(RobotState::STOPPED, false, false, true, 700) == (Lights{.red = true}));
    CHECK(lights(RobotState::OFF, false, false, false, 0) == Lights{});
    // the remote flashes: mostly dark
    CHECK(lights(RobotState::RUNNING, true, true, true, 20) == (Lights{.green = true}) && lights(RobotState::RUNNING, true, true, true, 500) == Lights{});
    CHECK(lights(RobotState::RUNNING, false, true, true, 20) == (Lights{.blue = true}) && lights(RobotState::RUNNING, false, true, false, 20) == Lights{});
    CHECK(lights(RobotState::FAULT, true, false, true, 0).red && !lights(RobotState::FAULT, true, false, true, 130).red);
}

static void save(const Frame& frame, const std::string& path) {
    std::FILE* f = std::fopen(path.c_str(), "wb");
    if (!f) return;
    std::fprintf(f, "P1\n%d %d\n", lcd_width, lcd_height);
    for (int y = 0; y < lcd_height; y++) {
        for (int x = 0; x < lcd_width; x++) std::fputc(frame.black(x, y) ? '1' : '0', f);
        std::fputc('\n', f);
    }
    std::fclose(f);
}

static void test_screens(const char* directory) {
    const RobotStatus running{.state = RobotState::RUNNING, .bus_on = true, .computer_on = true, .remote_linked = true,
                              .battery_cv = 5423, .battery_ca = 314, .bus_cv = 5410, .battery_percent = 70};
    RobotStatus stopped = running, faulted = running, off{};
    stopped.state = RobotState::STOPPED;
    stopped.bus_on = false;
    stopped.battery_ca = 42;
    faulted.state = RobotState::FAULT;
    faulted.faults = fault::BATTERY_LOW;
    faulted.bus_on = false;
    faulted.battery_cv = 3890;
    const RemoteStatus remote{.stop = StopSwitch::RUN, .cell_level = 4, .cell_mv = 3900};
    RemoteStatus in = remote, charging = remote;
    in.stop = StopSwitch::STOP;
    charging.docked = charging.charging = true;
    charging.cell_level = 2;
    const struct {
        const char* name;
        ScreenData data;
    } screens[] = {
        {"remote_running", {running, remote, true, true}},
        {"remote_stopped", {stopped, in, true, true}},
        {"remote_no_link", {off, remote, false, true}},
        {"remote_fault", {faulted, remote, true, true}},
        {"board_running", {running, remote, true, false}},
        {"board_off_charging", {RobotStatus{.state = RobotState::CHARGING, .battery_cv = 5380, .battery_ca = 12}, charging, true, false}},
        {"board_no_remote", {stopped, RemoteStatus{}, false, false}},
    };
    Frame frame;
    for (const auto& screen : screens) {
        draw_screen(frame, screen.data);
        int black = 0;
        for (int y = 0; y < lcd_height; y++) {
            for (int x = 0; x < lcd_width; x++) black += frame.black(x, y);
        }
        CHECK(black > 600 && black < lcd_width * lcd_height / 2);
        if (directory) save(frame, std::string(directory) + "/" + screen.name + ".pbm");
    }
    // not good: the top band is black with white letters
    draw_screen(frame, screens[1].data);
    CHECK(frame.black(1, 1) && frame.black(126, 31));
    draw_screen(frame, screens[0].data);
    CHECK(!frame.black(1, 1));
    // a changed number is a few lines, not the picture
    LcdShadow shadow;
    shadow.sent(frame, {0, lcd_height - 1});
    shadow.known = true;
    ScreenData next = screens[0].data;
    next.robot.battery_ca = 315;
    draw_screen(frame, next);
    int lines = 0;
    for (LcdShadow::Run run = shadow.next(frame); !run.empty(); run = shadow.next(frame, run.last + 1)) lines += run.last - run.first + 1;
    CHECK(lines > 0 && lines <= 24);
}

int main(int argc, char** argv) {
    test_aes();
    test_display();
    test_messages();
    test_controller();
    test_remote();
    test_lights();
    test_screens(argc > 1 ? argv[1] : nullptr);
    std::printf(failures ? "%d FAILED\n" : "all passed\n", failures);
    return failures ? 1 : 0;
}
