#include <concepts>

template <typename System, auto config>
struct MainLoop {
    using SensorPolicy = typename decltype(config)::SensorPolicy;
    SensorPolicy sensors;
    using SensorStatus = decltype(sensors.update());
    using FastLoopCommand = System::FastLoop::FastLoopCommand;
    using FastLoopStatus = System::FastLoop::FastLoopStatus;
    struct MainLoopStatus {
        using ExpectedCommand = FastLoopCommand;
        FastLoopStatus fast_loop;
        SensorStatus sensors;
    };
    //using DiagnosticPolicy = typename decltype(config)::DiagnosticPolicy;
    using ControllerPolicy = typename decltype(config)::ControllerPolicy;
    ControllerPolicy controller;

    FastLoopCommand fast_loop_command {};
    MainLoopStatus status {};
    void update() {
        status.fast_loop = System::fast_loop.get_status();
        status.sensors = sensors.update();
      //  DiagnosticPolicy::update();
        fast_loop_command = controller.update(status);
    }
    FastLoopCommand get_command() { return fast_loop_command; }
};


template <typename T>
concept IsFastLoopConfig = requires(T config) {
    { config.frequency } -> std::convertible_to<int>;
    typename T::ControllerType;
};

template <typename System, IsFastLoopConfig auto config>
struct FastLoop {
    using Controller = typename decltype(config)::ControllerType;
    Controller controller{config.kp};
    typename decltype(config)::EncoderType encoder;
    struct FastLoopStatus {
        int encoder_value;
    };
    struct FastLoopCommand {
        Controller::Command foc;
    };
    void update() {
        FastLoopCommand command = System::main_loop.get_command();
        encoder.read();
        controller.update(command.foc);
    }
    FastLoopStatus get_status() { return {}; }
};