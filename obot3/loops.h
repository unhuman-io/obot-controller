#pragma once
#include <concepts>
#include <cstdint>
#include <utility>
#include "control/control_fun.h"

// The main loop's sensors may use what the fast loop measured:
// update(fast_loop_status), or update() if they don't
template <typename Sensors, typename FastLoopStatus>
auto main_loop_sensors_update(Sensors& sensors, const FastLoopStatus& fast_loop) {
    if constexpr (requires { sensors.update(fast_loop); }) {
        return sensors.update(fast_loop);
    } else {
        return sensors.update();
    }
}

// Main loop: reads its sensors, then the controller policy turns the status
// (the fast loop's and the sensors') into the fast loop's next command.
template <typename System, auto config>
struct MainLoop {
    using SensorPolicy = typename decltype(config)::SensorPolicy;
    SensorPolicy sensors;
    using FastLoopCommand = System::FastLoop::FastLoopCommand;
    using FastLoopStatus = System::FastLoop::FastLoopStatus;
    using SensorStatus = decltype(main_loop_sensors_update(std::declval<SensorPolicy&>(), std::declval<const FastLoopStatus&>()));
    struct MainLoopStatus {
        using ExpectedCommand = FastLoopCommand;
        FastLoopStatus fast_loop;
        SensorStatus sensors;
    };
    //using DiagnosticPolicy = typename decltype(config)::DiagnosticPolicy;
    using ControllerPolicy = typename decltype(config)::ControllerPolicy;
    ControllerPolicy controller;

    Latest<FastLoopCommand> fast_loop_command {};
    MainLoopStatus status {};
    void update() {
        status.fast_loop = System::fast_loop.get_status();
        status.sensors = main_loop_sensors_update(sensors, status.fast_loop);
      //  DiagnosticPolicy::update();
        fast_loop_command.write(controller.update(status));
    }
    FastLoopCommand get_command() const { return fast_loop_command.read(); }
};


template <typename T>
concept IsFastLoopConfig = requires(T config) {
    { config.frequency } -> std::convertible_to<int>;
    typename T::SensorPolicy;
    typename T::ControllerType;
    typename T::OutputPolicy;
};

// Fast loop, once per pwm period:
//   sensors.update()                        -> the measurement
//   controller.update(command, measurement) -> the controller's output
//   output.update(output, measurement)         to the power stage
// The command is the main loop's latest; measurement and output are the
// status the main loop gets back. Sensors with a feedback(output) see the
// controller's output as well.
template <typename System, IsFastLoopConfig auto config>
struct FastLoop {
    using Sensors = typename decltype(config)::SensorPolicy;
    using Controller = typename decltype(config)::ControllerType;
    using OutputStage = typename decltype(config)::OutputPolicy;
    using Measurement = decltype(std::declval<Sensors&>().update());
    Sensors sensors;
    Controller controller;
    OutputStage output;
    struct FastLoopStatus {
        Measurement measured;
        typename Controller::Output controller;
    };
    struct FastLoopCommand {
        typename Controller::Command foc;
    };
    Latest<FastLoopStatus> status {};
    volatile uint32_t count = 0;
    void update() {
        const FastLoopCommand command = System::main_loop.get_command();
        FastLoopStatus s;
        s.measured = sensors.update();
        s.controller = controller.update(command.foc, s.measured);
        output.update(s.controller, s.measured);
        if constexpr (requires { sensors.feedback(s.controller); }) {
            sensors.feedback(s.controller);
        }
        status.write(s);
        count = count + 1;
    }
    FastLoopStatus get_status() const { return status.read(); }
};
