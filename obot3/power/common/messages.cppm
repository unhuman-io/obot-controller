module;
#include <cstddef>
#include <cstdint>

export module power.messages;
import power.aes;

export {
// What the three processors of the power board and the e-stop remote tell
// each other, and how it is packed:
//
//   remote      -- radio -->   radio module   RemoteStatus in a Heartbeat frame, 20 a second
//   remote      <-- radio --   radio module   RobotStatus in a StatusFrame, a few a second and on a change
//   radio module -- uart -->   main           RadioReport: the state, may the bus be on, is there a pause
//   radio module <-- uart --   main           MainReport: what it measures and switches
//
// Everything is little endian and packed by hand, byte by byte: the same
// bytes from a Cortex-M33, from a test on a PC, from Python.

// ---- what the robot is doing, as the screens and the CAN bus say it --------
enum class RobotState : uint8_t {
    OFF           = 0,            // computer supply off; only the radio module is awake, asleep most of the time
    STARTING      = 1,       // converters on, waiting for the main controller
    STOPPED       = 2,        // on, motor bus off: the red button is in, or there is no remote
    PAUSED        = 3,         // motor bus on, motors asked to hold still
    RUNNING       = 4,
    SHUTTING_DOWN = 5,  // motor bus off, the computer has been told and still has power
    CHARGING      = 6,       // off, but the converters run for the remote on the dock
    FAULT         = 7,          // motor bus off for a reason in the fault bits
};
constexpr const char* robot_state_name(RobotState state) {
    constexpr const char* names[] = {"OFF", "START", "STOP", "PAUSE", "RUN", "HALT", "CHARGE", "FAULT"};
    return static_cast<unsigned>(state) < 8 ? names[static_cast<unsigned>(state)] : "?";
}

// the red button, by its two contacts
enum class StopSwitch : uint8_t {
    RUN    = 0,        // out: the normally closed contact closed AND the normally open one open
    STOP   = 1,       // in
    BROKEN = 2,     // the two disagree (a wire off, the plug out): treated as STOP, shown as a fault
};

namespace fault {
inline constexpr uint8_t BATTERY_LOW = 1 << 0;      // below what the bus is allowed to run from
inline constexpr uint8_t BATTERY_HIGH = 1 << 1;
inline constexpr uint8_t BUS = 1 << 2;              // bus eFuse enabled but not on (tripped, or held off elsewhere)
inline constexpr uint8_t OUTPUT = 1 << 3;           // a computer output eFuse reports not good
inline constexpr uint8_t MONITOR = 1 << 4;          // a power monitor does not answer
inline constexpr uint8_t MAIN_SILENT = 1 << 5;      // the radio module does not hear the main controller
inline constexpr uint8_t RADIO_SILENT = 1 << 6;     // the main controller does not hear the radio module
inline constexpr uint8_t STOP_WIRING = 1 << 7;      // the remote's red button reads BROKEN
// with any of these the motor bus stays off
inline constexpr uint8_t BLOCKING = BATTERY_LOW | BATTERY_HIGH | BUS | MAIN_SILENT | RADIO_SILENT;
}

struct RobotStatus {
    RobotState state = RobotState::OFF;
    uint8_t faults = 0;
    bool bus_on = false;            // motor bus eFuse on
    bool computer_on = false;       // 12 V and 5 V outputs on
    bool remote_linked = false;     // the controller hears its remote
    bool remote_docked = false;
    uint16_t battery_cv = 0;        // V / 100
    int16_t battery_ca = 0;         // A / 100, out of the battery positive
    uint16_t bus_cv = 0;
    uint8_t battery_percent = 255;  // 255: not known (voltage alone says little on a lithium pack)
    constexpr bool operator==(const RobotStatus&) const = default;
};

struct RemoteStatus {
    StopSwitch stop = StopSwitch::STOP;
    uint8_t pause_presses = 0;      // counts short presses of PAUSE: a change toggles pause
    bool power_request = false;     // PAUSE was held down: switch the robot on, or off. Up until the robot's state shows it was heard
    bool docked = false;
    bool charging = false;
    uint8_t cell_level = 0;         // 0 (empty) to 5
    uint16_t cell_mv = 0;
    constexpr bool operator==(const RemoteStatus&) const = default;
};

// ---- bytes ----------------------------------------------------------------------
constexpr void put8(uint8_t*& at, uint8_t value) { *at++ = value; }
constexpr void put16(uint8_t*& at, uint16_t value) {
    *at++ = static_cast<uint8_t>(value);
    *at++ = static_cast<uint8_t>(value >> 8);
}
constexpr void put32(uint8_t*& at, uint32_t value) {
    put16(at, static_cast<uint16_t>(value));
    put16(at, static_cast<uint16_t>(value >> 16));
}
constexpr uint8_t get8(const uint8_t*& at) { return *at++; }
constexpr uint16_t get16(const uint8_t*& at) {
    const uint16_t value = static_cast<uint16_t>(at[0] | (at[1] << 8));
    at += 2;
    return value;
}
constexpr uint32_t get32(const uint8_t*& at) {
    const uint32_t low = get16(at);
    return low | (static_cast<uint32_t>(get16(at)) << 16);
}

inline constexpr std::size_t robot_status_size = 10, remote_status_size = 5;

constexpr void pack(const RobotStatus& s, uint8_t*& at) {
    put8(at, static_cast<uint8_t>(s.state));
    put8(at, s.faults);
    put8(at, static_cast<uint8_t>((s.bus_on ? 1 : 0) | (s.computer_on ? 2 : 0) | (s.remote_linked ? 4 : 0) | (s.remote_docked ? 8 : 0)));
    put16(at, s.battery_cv);
    put16(at, static_cast<uint16_t>(s.battery_ca));
    put16(at, s.bus_cv);
    put8(at, s.battery_percent);
}
constexpr RobotStatus unpack_robot_status(const uint8_t*& at) {
    RobotStatus s;
    const uint8_t state = get8(at);
    s.state = state <= static_cast<uint8_t>(RobotState::FAULT) ? static_cast<RobotState>(state) : RobotState::FAULT;
    s.faults = get8(at);
    const uint8_t flags = get8(at);
    s.bus_on = flags & 1;
    s.computer_on = flags & 2;
    s.remote_linked = flags & 4;
    s.remote_docked = flags & 8;
    s.battery_cv = get16(at);
    s.battery_ca = static_cast<int16_t>(get16(at));
    s.bus_cv = get16(at);
    s.battery_percent = get8(at);
    return s;
}

constexpr void pack(const RemoteStatus& s, uint8_t*& at) {
    put8(at, static_cast<uint8_t>(s.stop));
    put8(at, s.pause_presses);
    put8(at, static_cast<uint8_t>((s.docked ? 1 : 0) | (s.charging ? 2 : 0) | (s.power_request ? 4 : 0) | (s.cell_level << 4)));
    put16(at, s.cell_mv);
}
constexpr RemoteStatus unpack_remote_status(const uint8_t*& at) {
    RemoteStatus s;
    const uint8_t stop = get8(at);
    s.stop = stop == 0 ? StopSwitch::RUN : stop == 1 ? StopSwitch::STOP
                                                     : StopSwitch::BROKEN;    // anything unknown is not RUN
    s.pause_presses = get8(at);
    const uint8_t flags = get8(at);
    s.docked = flags & 1;
    s.charging = flags & 2;
    s.power_request = flags & 4;
    s.cell_level = static_cast<uint8_t>(flags >> 4);
    s.cell_mv = get16(at);
    return s;
}

// ---- over the radio ------------------------------------------------------------------
// The two were given the same 16 byte key when the remote was last on the
// dock. Every frame ends in the first bytes of a CMAC with that key over
//
//     a letter for the kind of frame | the session number | the frame before its code
//
// The session number is a random number the controller makes for each
// connection and sends with its status; the remote has to use it. So a frame
// recorded yesterday, or a minute ago, is worth nothing: its code was made
// with another session number. Within a session the remote counts its frames
// and the controller takes none that does not count upwards.
//
//   Heartbeat   counter (4) | RemoteStatus (5) | code (8)        17 bytes, remote -> controller
//   StatusFrame session (4) | RobotStatus (10) | code (4)        18 bytes, controller -> remote
//
// Both fit the 20 bytes that a Bluetooth LE attribute carries without asking
// for longer packets. The status has a shorter code: a forged status changes a
// picture on the remote, a forged heartbeat lets the motors run.
inline constexpr std::size_t radio_key_size = 16;
inline constexpr std::size_t heartbeat_size = 4 + remote_status_size + 8;
inline constexpr std::size_t status_frame_size = 4 + robot_status_size + 4;

constexpr void radio_code(const Aes128& key, char kind, uint32_t session, const uint8_t* frame, std::size_t length, uint8_t* code, std::size_t code_size) {
    uint8_t message[5 + 32];
    uint8_t* at = message;
    put8(at, static_cast<uint8_t>(kind));
    put32(at, session);
    for (std::size_t k = 0; k < length; k++) put8(at, frame[k]);
    uint8_t mac[16];
    aes_cmac(key, message, 5 + length, mac);
    for (std::size_t k = 0; k < code_size; k++) code[k] = mac[k];
}

// same in every byte, in a time that does not depend on where they differ
constexpr bool same_bytes(const uint8_t* a, const uint8_t* b, std::size_t length) {
    uint8_t difference = 0;
    for (std::size_t k = 0; k < length; k++) difference |= static_cast<uint8_t>(a[k] ^ b[k]);
    return difference == 0;
}

constexpr void make_heartbeat(const Aes128& key, uint32_t session, uint32_t counter, const RemoteStatus& status, uint8_t* frame) {
    uint8_t* at = frame;
    put32(at, counter);
    pack(status, at);
    radio_code(key, 'H', session, frame, 4 + remote_status_size, at, 8);
}

// What the controller makes of a frame it was written: true and the status if
// it is from the paired remote, for this session, and newer than `last_counter`
// (which is then moved on).
constexpr bool take_heartbeat(const Aes128& key, uint32_t session, uint32_t& last_counter, const uint8_t* frame, std::size_t length, RemoteStatus& status) {
    if (length != heartbeat_size) return false;
    uint8_t code[8];
    radio_code(key, 'H', session, frame, 4 + remote_status_size, code, 8);
    if (!same_bytes(code, frame + 4 + remote_status_size, 8)) return false;
    const uint8_t* at = frame;
    const uint32_t counter = get32(at);
    if (counter <= last_counter) return false;
    last_counter = counter;
    status = unpack_remote_status(at);
    return true;
}

constexpr void make_status_frame(const Aes128& key, uint32_t session, const RobotStatus& status, uint8_t* frame) {
    uint8_t* at = frame;
    put32(at, session);
    pack(status, at);
    radio_code(key, 'S', session, frame, 4 + robot_status_size, at, 4);
}

// What the remote makes of a notification: true, the session number to use and
// the robot's status, if the controller that sent it has our key
constexpr bool take_status_frame(const Aes128& key, const uint8_t* frame, std::size_t length, uint32_t& session, RobotStatus& status) {
    if (length != status_frame_size) return false;
    const uint8_t* at = frame;
    const uint32_t said = get32(at);
    uint8_t code[4];
    radio_code(key, 'S', said, frame, 4 + robot_status_size, code, 4);
    if (!same_bytes(code, frame + 4 + robot_status_size, 4)) return false;
    session = said;
    status = unpack_robot_status(at);
    return true;
}

// ---- between the radio module and the main controller (UART) -------------------------
//   0xA5 | kind | length of what follows before the check | ... | CRC-16 (CCITT, 0xFFFF), low byte first
struct RadioReport {                // radio module -> main controller, 20 a second
    RobotState state = RobotState::OFF;
    uint8_t faults = 0;             // the radio module's own
    bool allow_bus = false;         // false: the main controller takes BUS_EN low
    bool computer_on = false;       // the 12 V output (the 5 V output follows its converter)
    bool pause = false;
    bool remote_linked = false;
    bool remote_docked = false;
    StopSwitch stop = StopSwitch::STOP;
    uint8_t remote_cell_level = 0;
    constexpr bool operator==(const RadioReport&) const = default;
};
struct MainReport {                 // main controller -> radio module, 20 a second
    uint8_t faults = 0;             // the main controller's own
    bool bus_enabled = false;       // it drives BUS_EN high
    bool bus_on = false;            // the bus eFuse reports power good
    bool outputs_good = false;      // the computer output eFuses report good
    bool computer_on = false;
    uint16_t battery_cv = 0;
    int16_t battery_ca = 0;
    uint16_t bus_cv = 0;
    int16_t out12_ca = 0;
    int16_t out5_ca = 0;
    uint32_t energy_mwh = 0;        // out of the battery since power on
    constexpr bool operator==(const MainReport&) const = default;
};
inline constexpr uint8_t link_start = 0xA5;
inline constexpr uint8_t link_kind_radio = 1, link_kind_main = 2;
inline constexpr std::size_t radio_report_size = 5, main_report_size = 16;
inline constexpr std::size_t link_frame_max = 3 + 24 + 2;       // (the dock's frames use the same form and are longer)

constexpr uint16_t crc16(const uint8_t* data, std::size_t length) {
    uint16_t crc = 0xFFFF;
    for (std::size_t k = 0; k < length; k++) {
        crc ^= static_cast<uint16_t>(data[k] << 8);
        for (int bit = 0; bit < 8; bit++) crc = static_cast<uint16_t>((crc & 0x8000) ? (crc << 1) ^ 0x1021 : crc << 1);
    }
    return crc;
}

constexpr std::size_t finish_link_frame(uint8_t* frame, uint8_t* at) {
    frame[2] = static_cast<uint8_t>(at - frame - 3);
    put16(at, crc16(frame, static_cast<std::size_t>(at - frame)));
    return static_cast<std::size_t>(at - frame);
}

constexpr std::size_t make_link_frame(const RadioReport& r, uint8_t* frame) {
    uint8_t* at = frame;
    put8(at, link_start);
    put8(at, link_kind_radio);
    put8(at, 0);
    put8(at, static_cast<uint8_t>(r.state));
    put8(at, r.faults);
    put8(at, static_cast<uint8_t>((r.allow_bus ? 1 : 0) | (r.computer_on ? 2 : 0) | (r.pause ? 4 : 0) | (r.remote_linked ? 8 : 0) | (r.remote_docked ? 16 : 0)));
    put8(at, static_cast<uint8_t>(r.stop));
    put8(at, r.remote_cell_level);
    return finish_link_frame(frame, at);
}

constexpr std::size_t make_link_frame(const MainReport& m, uint8_t* frame) {
    uint8_t* at = frame;
    put8(at, link_start);
    put8(at, link_kind_main);
    put8(at, 0);
    put8(at, m.faults);
    put8(at, static_cast<uint8_t>((m.bus_enabled ? 1 : 0) | (m.bus_on ? 2 : 0) | (m.outputs_good ? 4 : 0) | (m.computer_on ? 8 : 0)));
    put16(at, m.battery_cv);
    put16(at, static_cast<uint16_t>(m.battery_ca));
    put16(at, m.bus_cv);
    put16(at, static_cast<uint16_t>(m.out12_ca));
    put16(at, static_cast<uint16_t>(m.out5_ca));
    put32(at, m.energy_mwh);
    return finish_link_frame(frame, at);
}

constexpr RadioReport unpack_radio_report(const uint8_t* at) {
    RadioReport r;
    const uint8_t state = get8(at);
    r.state = state <= static_cast<uint8_t>(RobotState::FAULT) ? static_cast<RobotState>(state) : RobotState::FAULT;
    r.faults = get8(at);
    const uint8_t flags = get8(at);
    r.allow_bus = flags & 1;
    r.computer_on = flags & 2;
    r.pause = flags & 4;
    r.remote_linked = flags & 8;
    r.remote_docked = flags & 16;
    const uint8_t stop = get8(at);
    r.stop = stop == 0 ? StopSwitch::RUN : stop == 1 ? StopSwitch::STOP
                                                     : StopSwitch::BROKEN;
    r.remote_cell_level = get8(at);
    return r;
}

constexpr MainReport unpack_main_report(const uint8_t* at) {
    MainReport m;
    m.faults = get8(at);
    const uint8_t flags = get8(at);
    m.bus_enabled = flags & 1;
    m.bus_on = flags & 2;
    m.outputs_good = flags & 4;
    m.computer_on = flags & 8;
    m.battery_cv = get16(at);
    m.battery_ca = static_cast<int16_t>(get16(at));
    m.bus_cv = get16(at);
    m.out12_ca = static_cast<int16_t>(get16(at));
    m.out5_ca = static_cast<int16_t>(get16(at));
    m.energy_mwh = get32(at);
    return m;
}

// ---- on the CAN bus ----------------------------------------------------------------------
// The power board is an obot node (id = message << 7 | address) and sends this
// as its status, ten a second: what the motor controllers need (is there a
// pause), and what the computer shows and logs. 20 bytes, a CAN FD frame.
//
//   0 state (RobotState)   1 faults   2 flags: 1 bus on, 2 computer on, 4 pause, 8 remote heard, 16 remote on the dock
//   3 the red button (StopSwitch) in the low 4 bits, the remote's cell level (0..5) in the high 4
//   4 battery V/100 (u16)   6 battery A/100 (i16, out of the battery positive)   8 motor bus V/100 (u16)
//   10 12 V output A/100 (i16)   12 5 V output A/100 (i16)   14 0   16 energy out of the battery, mWh (u32)
inline constexpr std::size_t can_status_size = 20;
constexpr void make_can_status(const RadioReport& radio, const MainReport& main, uint8_t* frame) {
    uint8_t* at = frame;
    put8(at, static_cast<uint8_t>(radio.state));
    put8(at, static_cast<uint8_t>(radio.faults | main.faults));
    put8(at, static_cast<uint8_t>((main.bus_on ? 1 : 0) | (main.computer_on ? 2 : 0) | (radio.pause ? 4 : 0) | (radio.remote_linked ? 8 : 0) |
                                  (radio.remote_docked ? 16 : 0)));
    put8(at, static_cast<uint8_t>(static_cast<uint8_t>(radio.stop) | radio.remote_cell_level << 4));
    put16(at, main.battery_cv);
    put16(at, static_cast<uint16_t>(main.battery_ca));
    put16(at, main.bus_cv);
    put16(at, static_cast<uint16_t>(main.out12_ca));
    put16(at, static_cast<uint16_t>(main.out5_ca));
    put16(at, 0);
    put32(at, main.energy_mwh);
}

// The receiving end: bytes go in one at a time as the UART has them; a
// complete frame with the right check comes out. Anything else (noise while
// the other processor starts, half a frame) is dropped and the search for a
// start byte goes on.
struct LinkReceiver {
    uint8_t frame[link_frame_max]{};
    std::size_t count = 0;

    // true when `byte` completed a frame: kind() and payload() are then valid until the next call
    constexpr bool put(uint8_t byte) {
        if (count == 0 && byte != link_start) return false;
        if (count == 2 && byte > link_frame_max - 5) {       // a length no frame has
            count = 0;
            return false;
        }
        frame[count++] = byte;
        if (count < 3 || count < static_cast<std::size_t>(3 + frame[2] + 2)) return false;
        const std::size_t body = 3 + static_cast<std::size_t>(frame[2]);
        const uint16_t sent = static_cast<uint16_t>(frame[body] | (frame[body + 1] << 8));
        count = 0;
        return sent == crc16(frame, body);
    }
    constexpr uint8_t kind() const { return frame[1]; }
    constexpr uint8_t length() const { return frame[2]; }
    constexpr const uint8_t* payload() const { return frame + 3; }
};
}
