#include <concepts>

template <typename System, auto config>
struct MainLoop {
    using SensorPolicy = typename decltype(config)::SensorPolicy;
    static inline constinit SensorPolicy sensors;
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
    static inline ControllerPolicy controller;

    static inline FastLoopCommand fast_loop_command {};
    static inline MainLoopStatus status {};
    static void update() {
        status.fast_loop = System::FastLoop::get_status();
        status.sensors = sensors.update();
      //  DiagnosticPolicy::update();
        fast_loop_command = controller.update(status);
    }
    static FastLoopCommand get_command() { return fast_loop_command; }
};


template <typename T>
concept IsFastLoopConfig = requires(T config) {
    { config.frequency } -> std::convertible_to<int>;
    typename T::ControllerType;
};

template <typename System, IsFastLoopConfig auto config>
struct FastLoop {
    using Controller = typename decltype(config)::ControllerType;
    static inline Controller controller{config.kp};
    static inline typename decltype(config)::EncoderType encoder;
    struct FastLoopStatus {
        int encoder_value;
    };
    struct FastLoopCommand {
        Controller::Command foc;
    };
    static void update() {
        FastLoopCommand command = System::MainLoop::get_command();
        encoder.read();
        controller.update(command.foc);
    }
    static FastLoopStatus get_status() { return {}; }
};