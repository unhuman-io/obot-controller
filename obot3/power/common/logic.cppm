module;
#include <cstdint>

export module power.logic;
import power.messages;

export {
// What the radio module of the power board and the remote decide, as
// functions of what they see and of the time: no hardware in here, so all of
// it runs in a test on a PC (power/test). Time is in milliseconds and wraps
// (differences are taken, never comparisons of two times).
//
// The rule for the motor bus: it is on only while an authenticated message
// from the paired remote, saying its red button is OUT, is less than
// heartbeat_timeout_ms old. No remote, a remote out of range, a flat cell, a
// broken wire in the button: all of those are silence or "not out", and the
// bus is off. A wireless stop is a convenience, not a safety-rated function.
inline constexpr uint32_t heartbeat_timeout_ms = 300;   // the remote sends 20 a second: six in a row lost
inline constexpr uint32_t main_timeout_ms = 500;        // the main controller reports 20 a second
inline constexpr uint32_t start_timeout_ms = 3000;      // converters on to the main controller's first report
inline constexpr uint32_t shutdown_grace_ms = 20000;    // the computer keeps its supply this long after being told
inline constexpr uint32_t long_press_ms = 1500;
inline constexpr uint32_t debounce_ms = 30;
inline constexpr uint32_t forget_press_ms = 5000;       // the pairing button held: forget the remote
inline constexpr uint32_t stop_release_ms = 50;         // the red button has to read OUT this long
inline constexpr uint32_t stop_broken_ms = 200;         // contacts that disagree for longer than their travel
inline constexpr uint32_t remote_awake_ms = 30000;      // the remote's radio after the last press, with no robot to talk to
inline constexpr uint32_t power_request_ms = 5000;      // how long a held PAUSE is said to the robot

// A push button: debounced, and told apart by how long it was held. SHORT
// comes when it is let go, LONG as soon as it has been down long enough (and
// then no SHORT follows).
struct Button {
    enum class Event : uint8_t { NONE, SHORT, LONG };
    bool down = false;
    bool reported = false;
    bool raw = false;
    uint32_t raw_since = 0, down_since = 0;

    constexpr Event update(bool pressed, uint32_t now_ms, uint32_t long_ms = long_press_ms) {
        if (pressed != raw) {
            raw = pressed;
            raw_since = now_ms;
        }
        if (raw != down && now_ms - raw_since >= debounce_ms) {
            down = raw;
            if (down) {
                down_since = raw_since;
                reported = false;
            } else if (!reported) {
                return Event::SHORT;
            }
        }
        if (down && !reported && now_ms - down_since >= long_ms) {
            reported = true;
            return Event::LONG;
        }
        return Event::NONE;
    }
};

constexpr bool robot_is_on(RobotState state) {
    return state != RobotState::OFF && state != RobotState::CHARGING;
}

// How full a 15 cell pack is, from its voltage, in per cent: a rough figure
// (a lithium pack's voltage says little in the middle and sags under load),
// good for "half" and "nearly empty". 255 when there is no voltage to go by.
constexpr uint8_t battery_percent(uint16_t battery_cv) {
    constexpr int empty_cv = 4950, full_cv = 6230;      // 3.3 and 4.15 V a cell
    if (battery_cv < 3000) return 255;
    const int percent = (static_cast<int>(battery_cv) - empty_cv) * 100 / (full_cv - empty_cv);
    return static_cast<uint8_t>(percent < 0 ? 0 : percent > 100 ? 100 : percent);
}

// ---- the radio module of the power board ----------------------------------------
struct ControllerInputs {
    uint32_t now_ms = 0;
    bool power_button = false;          // down
    bool pair_button = false;
    bool remote_fresh = false;          // an authenticated heartbeat less than heartbeat_timeout_ms old
    RemoteStatus remote{};              // the newest (meaningless without remote_fresh)
    bool main_fresh = false;            // a report from the main controller less than main_timeout_ms old
    MainReport main{};
    bool dock_charge_wanted = false;    // the remote is on the dock and its cell wants charging
    uint16_t battery_cv = 0;            // by the radio module's own divider; 0: not measured
};

struct ControllerOutputs {
    bool converters_on = false;         // DCDC_ON and V5_ON: 12 V and 5 V converters, and with them the main controller
    bool bus_kill = true;               // BUS_KILL: hold the bus eFuse's enable low whatever the main controller says
    RadioReport report{};               // for the main controller
    RobotStatus status{};               // for the remote and the screen
    bool forget_remote = false;         // once, when the pairing button has been held: the stored key is to go
};

struct ControllerLogic {
    enum class Power : uint8_t { OFF, CHARGING, STARTING, ON, SHUTTING_DOWN };
    Power power = Power::OFF;
    uint32_t since = 0;                 // when `power` became what it is
    bool paused = true;
    bool remote_was_fresh = false;
    uint8_t pause_presses = 0;          // the remote's, when last looked at
    bool power_request_seen = false;    // the remote's power request has been acted on and is still up
    Button power_button, pair_button;

    constexpr void go(Power next, uint32_t now_ms) {
        power = next;
        since = now_ms;
    }

    constexpr ControllerOutputs update(const ControllerInputs& in) {
        ControllerOutputs out;
        const Button::Event pressed = power_button.update(in.power_button, in.now_ms);
        out.forget_remote = pair_button.update(in.pair_button, in.now_ms, forget_press_ms) == Button::Event::LONG;

        // the remote: PAUSE pressed (a change of its count), PAUSE held (its request, taken once)
        bool pause_pressed = false, power_asked = false;
        if (in.remote_fresh) {
            if (remote_was_fresh && in.remote.pause_presses != pause_presses) pause_pressed = true;
            pause_presses = in.remote.pause_presses;
            if (in.remote.power_request && !power_request_seen) power_asked = true;
            power_request_seen = in.remote.power_request;
        }
        remote_was_fresh = in.remote_fresh;

        switch (power) {
            case Power::OFF:
            case Power::CHARGING:
                if (pressed == Button::Event::SHORT || power_asked) {
                    go(Power::STARTING, in.now_ms);
                } else if (in.dock_charge_wanted != (power == Power::CHARGING)) {
                    go(in.dock_charge_wanted ? Power::CHARGING : Power::OFF, in.now_ms);
                }
                break;
            case Power::STARTING:
                if (pressed == Button::Event::LONG) {
                    go(Power::OFF, in.now_ms);
                } else if (in.main_fresh || in.now_ms - since >= start_timeout_ms) {
                    go(Power::ON, in.now_ms);       // without the main controller: on, with its fault showing
                }
                break;
            case Power::ON:
                if (pressed == Button::Event::LONG || power_asked) go(Power::SHUTTING_DOWN, in.now_ms);
                break;
            case Power::SHUTTING_DOWN:
                if (in.now_ms - since >= shutdown_grace_ms || pressed == Button::Event::LONG) go(Power::OFF, in.now_ms);
                break;
        }

        uint8_t faults = 0;
        if (in.remote_fresh && in.remote.stop == StopSwitch::BROKEN) faults |= fault::STOP_WIRING;
        const bool on = power == Power::ON;
        if (on || power == Power::SHUTTING_DOWN) {
            if (in.main_fresh) {
                faults |= in.main.faults;
            } else {
                faults |= fault::MAIN_SILENT;
            }
        }
        const bool allow_bus = on && in.remote_fresh && in.remote.stop == StopSwitch::RUN && !(faults & fault::BLOCKING);
        // After anything that took the bus away the robot is paused: letting the
        // red button out again powers the motors, it does not make them move.
        if (!allow_bus) {
            paused = true;
        } else if (pause_pressed) {
            paused = !paused;
        }

        RobotState state = RobotState::OFF;
        switch (power) {
            case Power::OFF: state = RobotState::OFF; break;
            case Power::CHARGING: state = RobotState::CHARGING; break;
            case Power::STARTING: state = RobotState::STARTING; break;
            case Power::SHUTTING_DOWN: state = RobotState::SHUTTING_DOWN; break;
            case Power::ON:
                state = (faults & fault::BLOCKING) ? RobotState::FAULT : !allow_bus ? RobotState::STOPPED : paused ? RobotState::PAUSED : RobotState::RUNNING;
                break;
        }

        out.converters_on = power != Power::OFF;
        out.bus_kill = !allow_bus;
        out.report.state = state;
        out.report.faults = faults;
        out.report.allow_bus = allow_bus;
        out.report.computer_on = on || power == Power::SHUTTING_DOWN;
        out.report.pause = paused;
        out.report.remote_linked = in.remote_fresh;
        out.report.remote_docked = in.remote_fresh ? in.remote.docked : in.dock_charge_wanted;
        out.report.stop = in.remote_fresh ? in.remote.stop : StopSwitch::STOP;
        out.report.remote_cell_level = in.remote_fresh ? in.remote.cell_level : 0;

        out.status.state = state;
        out.status.faults = faults;
        out.status.bus_on = in.main_fresh && in.main.bus_on;
        out.status.computer_on = in.main_fresh && in.main.computer_on;
        out.status.remote_linked = in.remote_fresh;
        out.status.remote_docked = out.report.remote_docked;
        if (in.main_fresh) {
            out.status.battery_cv = in.main.battery_cv;
            out.status.battery_ca = in.main.battery_ca;
            out.status.bus_cv = in.main.bus_cv;
            out.status.battery_percent = battery_percent(in.main.battery_cv);
        } else if (in.battery_cv != 0) {
            out.status.battery_cv = in.battery_cv;          // the radio module's own divider: all there is while the rest is off
            out.status.battery_percent = battery_percent(in.battery_cv);
        }
        return out;
    }
};

// ---- the main controller of the power board --------------------------------------------
// It measures, switches the computer's 12 V output, and is one of the two
// that have to agree before the motor bus is on: it drives the bus eFuse's
// enable high only while the radio module, which it must have heard from in
// the last radio_timeout_ms, allows it and the battery is in range. The radio
// module can pull the same line low by itself.
inline constexpr uint32_t radio_timeout_ms = 300;       // the radio module reports 20 a second
inline constexpr uint32_t settle_ms = 500;              // an eFuse told to switch on has this long to say "good"
// The pack is 15 lithium cells in series (63 V full; the board's own over-voltage lock-out is at 64.8 V)
inline constexpr uint16_t battery_low_cv = 4500, battery_low_clear_cv = 4700;       // 3.0 V a cell
inline constexpr uint16_t battery_high_cv = 6350, battery_high_clear_cv = 6300;

struct MainInputs {
    uint32_t now_ms = 0;
    bool radio_fresh = false;           // a report from the radio module less than radio_timeout_ms old
    RadioReport radio{};
    bool monitors_ok = false;           // all three power monitors answer
    uint16_t battery_cv = 0;
    int16_t battery_ca = 0;
    uint16_t bus_cv = 0;
    int16_t out12_ca = 0, out5_ca = 0;
    bool bus_good = false;              // the bus eFuse's power good
    bool outputs_good = false;          // the computer outputs' power good (both, wired together)
};

struct MainOutputs {
    bool bus_enable = false;            // BUS_EN
    bool output12_on = false;           // OUT12_ON (the 5 V output is on whenever its converter runs)
    MainReport report{};
};

struct MainLogic {
    bool battery_low = false, battery_high = false;
    bool bus_enable = false, output12_on = false;
    uint32_t bus_since = 0, output_since = 0, last_ms = 0;
    bool started = false;
    uint64_t energy_uws = 0;            // V x A x ms, out of the battery

    constexpr MainOutputs update(const MainInputs& in) {
        if (!started) {
            started = true;
            last_ms = bus_since = output_since = in.now_ms;
        }
        uint8_t faults = 0;
        if (!in.monitors_ok) {
            faults |= fault::MONITOR;       // no voltage to judge by: the limits below keep what they said last
        } else {
            battery_low = in.battery_cv < (battery_low ? battery_low_clear_cv : battery_low_cv);
            battery_high = in.battery_cv > (battery_high ? battery_high_clear_cv : battery_high_cv);
            if (in.battery_ca > 0) energy_uws += static_cast<uint64_t>(in.battery_cv) * static_cast<uint64_t>(in.battery_ca) * (in.now_ms - last_ms) / 10;
        }
        last_ms = in.now_ms;
        if (battery_low) faults |= fault::BATTERY_LOW;
        if (battery_high) faults |= fault::BATTERY_HIGH;
        if (!in.radio_fresh) faults |= fault::RADIO_SILENT;

        const bool enable = in.radio_fresh && in.radio.allow_bus && !battery_low && !battery_high;
        if (enable != bus_enable) {
            bus_enable = enable;
            bus_since = in.now_ms;
        }
        // The computer's supply follows the radio module while it is heard, and
        // stays as it is while it is not: a radio module that restarts must not
        // switch the computer off.
        if (in.radio_fresh && in.radio.computer_on != output12_on) {
            output12_on = in.radio.computer_on;
            output_since = in.now_ms;
        }
        // enabled, given time, and not on: tripped (it retries by itself), or held off by the radio module
        if (bus_enable && !in.bus_good && in.now_ms - bus_since >= settle_ms) faults |= fault::BUS;
        if (output12_on && !in.outputs_good && in.now_ms - output_since >= settle_ms) faults |= fault::OUTPUT;

        MainOutputs out;
        out.bus_enable = bus_enable;
        out.output12_on = output12_on;
        out.report.faults = faults;
        out.report.bus_enabled = bus_enable;
        out.report.bus_on = in.bus_good;
        out.report.outputs_good = in.outputs_good;
        out.report.computer_on = output12_on;
        out.report.battery_cv = in.battery_cv;
        out.report.battery_ca = in.battery_ca;
        out.report.bus_cv = in.bus_cv;
        out.report.out12_ca = in.out12_ca;
        out.report.out5_ca = in.out5_ca;
        out.report.energy_mwh = static_cast<uint32_t>(energy_uws / 3'600'000);
        return out;
    }
};

// ---- the remote --------------------------------------------------------------------------
struct RemoteInputs {
    uint32_t now_ms = 0;
    bool contact_a_closed = false;      // the red button's normally closed contact: closed while the button is OUT
    bool contact_b_closed = false;      // its normally open contact: closed while the button is IN
    bool pause_button = false;          // down
    bool docked = false;
    bool charging = false;
    uint16_t cell_mv = 0;
    bool linked = false;                // connected, and a status from the controller with our key has come
    RobotStatus robot{};                // the newest (meaningless without linked)
};

struct RemoteOutputs {
    RemoteStatus status{};              // what goes into the heartbeat
    bool radio_on = false;              // false: disconnect, stop looking, sleep until a button or the dock wakes
};

// The cell, in six steps: a lithium cell is flat between 3.7 and 3.9 V, so
// more steps would only look exact.
constexpr uint8_t cell_level(uint16_t cell_mv) {
    constexpr uint16_t steps[] = {3400, 3600, 3700, 3850, 4000};
    uint8_t level = 0;
    for (const uint16_t step : steps) {
        if (cell_mv >= step) level++;
    }
    return level;
}

struct RemoteLogic {
    Button pause;
    StopSwitch stop = StopSwitch::STOP;
    bool started = false;
    bool was_out = false, was_docked = false;
    uint32_t out_since = 0, disagree_since = 0, awake_since = 0, request_since = 0;
    bool disagree = false;
    uint8_t pause_presses = 0;
    bool power_request = false;
    bool request_from_on = false;       // the robot was on when PAUSE was held

    constexpr RemoteOutputs update(const RemoteInputs& in) {
        if (!started) {
            started = true;
            awake_since = in.now_ms;
        }
        // The red button. OUT is believed only when both contacts say so and
        // have for a moment; IN at once; contacts that disagree are IN, and
        // after longer than the button's travel a fault as well.
        const bool out_now = in.contact_a_closed && !in.contact_b_closed;
        const bool in_now = !in.contact_a_closed && in.contact_b_closed;
        if (out_now != was_out) {
            was_out = out_now;
            out_since = in.now_ms;
            awake_since = in.now_ms;
        }
        if ((!out_now && !in_now) != disagree) {
            disagree = !disagree;
            disagree_since = in.now_ms;
        }
        if (out_now) {
            if (in.now_ms - out_since >= stop_release_ms) stop = StopSwitch::RUN;
        } else if (disagree && in.now_ms - disagree_since >= stop_broken_ms) {
            stop = StopSwitch::BROKEN;
        } else {
            stop = StopSwitch::STOP;
        }

        const bool robot_on = in.linked && robot_is_on(in.robot.state);
        switch (pause.update(in.pause_button, in.now_ms)) {
            case Button::Event::SHORT:
                pause_presses++;
                awake_since = in.now_ms;
                break;
            case Button::Event::LONG:
                power_request = true;
                request_since = in.now_ms;
                request_from_on = robot_on;
                awake_since = in.now_ms;
                break;
            case Button::Event::NONE:
                break;
        }
        if (in.pause_button) awake_since = in.now_ms;
        if (in.docked != was_docked) {
            was_docked = in.docked;
            awake_since = in.now_ms;
        }
        // said until the robot shows it has heard (it changed between off and on), or for a while
        if (power_request && ((in.linked && robot_on != request_from_on) || in.now_ms - request_since >= power_request_ms)) power_request = false;

        RemoteOutputs out;
        out.status.stop = stop;
        out.status.pause_presses = pause_presses;
        out.status.power_request = power_request;
        out.status.docked = in.docked;
        out.status.charging = in.charging;
        out.status.cell_mv = in.cell_mv;
        out.status.cell_level = cell_level(in.cell_mv);
        // The radio: for as long as there is a robot that is on to keep alive,
        // and for a while after anything was pressed. A robot that is off needs
        // no remote, and looking for one costs the cell.
        out.radio_on = robot_on || power_request || in.now_ms - awake_since < remote_awake_ms;
        return out;
    }
};

// ---- the three status lights (red, green, blue) ------------------------------------------
struct Lights {
    bool red = false, green = false, blue = false;
    constexpr bool operator==(const Lights&) const = default;
};

// On the power board the colour stays on while the robot is on; on the remote
// it flashes once a second, which is a thirtieth of the current from a cell
// that has little.
constexpr Lights lights(RobotState state, bool linked, bool is_remote, bool awake, uint32_t now_ms) {
    const uint32_t phase = now_ms % 1000;
    const bool flash = phase < 40, blink = phase < 500, fast = (now_ms % 250) < 125;
    const bool steady = is_remote ? flash : true;
    Lights l;
    if (is_remote && !linked) {
        l.blue = awake && flash;            // looking for the robot
        return l;
    }
    switch (state) {
        case RobotState::OFF: break;
        case RobotState::CHARGING: l.blue = flash; break;
        case RobotState::STARTING: l.blue = blink; break;
        case RobotState::SHUTTING_DOWN: l.blue = blink; break;
        case RobotState::STOPPED: l.red = steady; break;
        case RobotState::PAUSED: l.red = l.green = steady; break;      // yellow
        case RobotState::RUNNING: l.green = steady; break;
        case RobotState::FAULT: l.red = fast; break;
    }
    return l;
}
}
