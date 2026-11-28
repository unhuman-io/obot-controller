module;
#include <cstdint>
#include <string_view>

export module power.screen;
import power.frame;
import power.font;
import power.messages;

export {
// The status screen, the same on the power board and on the remote:
//
//     +----------------+
//     |      RUN       |   what the robot is doing, large; white on black when it is not good
//     | [###  ] 54.2V  |   the robot's battery
//     |  168W   3.1A   |
//     | BUS 54.1V  ON  |   the motor bus, or what is wrong
//     | COMPUTER   ON  |
//     |----------------|
//     | REMOTE [|||| ] |   the remote's cell, charging, on the dock
//     +----------------+
//
// The remote shows NO LINK in place of the robot's state when it does not
// hear the robot, and then nothing it does not know.
struct ScreenData {
    RobotStatus robot{};
    RemoteStatus remote{};
    bool linked = false;        // remote: it hears the robot. power board: it hears the remote
    bool is_remote = false;
};

constexpr std::string_view fault_text(uint8_t faults) {
    if (faults & fault::MAIN_SILENT) return "MAIN CONTROLLER ?";
    if (faults & fault::RADIO_SILENT) return "RADIO MODULE ?";
    if (faults & fault::BATTERY_LOW) return "BATTERY LOW";
    if (faults & fault::BATTERY_HIGH) return "BATTERY HIGH";
    if (faults & fault::BUS) return "BUS FUSE";
    if (faults & fault::OUTPUT) return "OUTPUT FUSE";
    if (faults & fault::MONITOR) return "POWER MONITOR ?";
    if (faults & fault::STOP_WIRING) return "STOP BUTTON WIRE";
    return "";
}

// a battery lying down, `filled` of `cells` cells black
constexpr void draw_battery(Frame& frame, int x, int y, int width, int height, int cells, int filled) {
    frame.box(x, y, width, height);
    frame.fill(x + width, y + height / 4, 2, height - 2 * (height / 4));     // the terminal
    const int inner = width - 4, cell = inner / cells;
    for (int k = 0; k < filled && k < cells; k++) frame.fill(x + 2 + k * cell, y + 2, cell - 1, height - 4);
}

constexpr void draw_number(Frame& frame, int x, int y, int32_t milli, int decimals, std::string_view unit, int scale) {
    char text[16];
    int n = format_milli(milli, decimals, text);
    for (const char c : unit) text[n++] = c;
    draw_text(frame, x, y, std::string_view(text, static_cast<std::size_t>(n)), scale);
}

constexpr void draw_screen(Frame& frame, const ScreenData& data) {
    frame.clear();
    const bool know_robot = !data.is_remote || data.linked;
    const RobotStatus& robot = data.robot;

    // what the robot is doing
    const std::string_view banner = know_robot ? robot_state_name(robot.state) : "NO LINK";
    draw_text_centred(frame, lcd_width / 2, 6, banner, 3);
    const bool bad = !know_robot || robot.state == RobotState::STOPPED || robot.state == RobotState::FAULT;
    if (bad) frame.invert(0, 0, lcd_width, 33);

    if (know_robot) {
        // the battery: five cells from the per cent if that is known, else an empty outline
        const int cells = robot.battery_percent <= 100 ? (robot.battery_percent + 10) / 20 : 0;
        draw_battery(frame, 4, 39, 32, 14, 5, cells);
        draw_number(frame, 46, 39, robot.battery_cv * 10, 1, "V", 2);
        const int32_t watts = static_cast<int32_t>(robot.battery_cv) * robot.battery_ca / 10000;
        draw_number(frame, 4, 62, watts * 1000, 0, "W", 1);
        draw_number(frame, 58, 58, robot.battery_ca * 10, 1, "A", 2);

        if (const std::string_view wrong = fault_text(robot.faults); !wrong.empty()) {
            draw_text(frame, 4, 80, wrong);
        } else {
            int x = draw_text(frame, 4, 80, "BUS ");
            if (robot.bus_on) {
                draw_number(frame, x, 80, robot.bus_cv * 10, 1, "V", 1);
            } else {
                draw_text(frame, x, 80, "OFF");
            }
        }
        draw_text(frame, 4, 92, robot.computer_on ? "COMPUTER ON" : "COMPUTER OFF");
    } else {
        draw_text_centred(frame, lcd_width / 2, 52, "LOOKING FOR", 1);
        draw_text_centred(frame, lcd_width / 2, 64, "THE ROBOT", 1);
    }

    // the remote
    frame.fill(0, 104, lcd_width, 1);
    const bool know_remote = data.is_remote || data.linked;
    if (know_remote) {
        draw_text(frame, 4, 113, "REMOTE");
        draw_battery(frame, 46, 110, 34, 13, 5, data.remote.cell_level);
        if (data.remote.charging) {
            draw_text(frame, 88, 113, "CHARGE");
        } else if (data.remote.docked) {
            draw_text(frame, 88, 113, "DOCK");
        } else if (data.is_remote || data.remote.stop != StopSwitch::RUN) {
            draw_text(frame, 88, 113, data.remote.stop == StopSwitch::RUN ? "OUT" : data.remote.stop == StopSwitch::STOP ? "STOP"
                                                                                                                         : "WIRE?");
        }
    } else {
        draw_text(frame, 4, 113, "NO REMOTE");
    }
}
}
