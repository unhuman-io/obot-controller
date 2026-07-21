#include <concepts>

template <typename System, auto config>
struct MainLoop {
    using SensorPolicy = typename decltype(config)::SensorPolicy;
    //using DiagnosticPolicy = typename decltype(config)::DiagnosticPolicy;
    //using ControllerPolicy = typename decltype(config)::ControllerPolicy;
    static inline SensorPolicy sensors;
    using SensorStatus = decltype(sensors.update());
    static void update() {
        System::FastLoop::get_status();
        SensorStatus sensor_status = sensors.update();
      //  DiagnosticPolicy::update();
      //  ControllerPolicy::update();
    }
    static void get_command() {}
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
    static void update() {
        System::MainLoop::get_command();
        encoder.read();
        controller.update();
    }
    static void get_status() {}
};