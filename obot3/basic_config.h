#include "loops.h"
#include "basic_foc_controller.h"

struct EncoderBase {};

struct DefaultSensorPolicy {
    int update() { return 0; }
};

struct BasicDefaultConfig {
    int fast_loop_frequency = 100'000;
    int main_loop_frequency = 10'000;
    using MotorEncoderType = EncoderBase;
    using MainLoopSensorPolicy = DefaultSensorPolicy;
};

template<auto config = BasicDefaultConfig{}>
struct BasicConfig {
    static constexpr int fast_loop_frequency = config.fast_loop_frequency;
    static constexpr int main_loop_frequency = config.main_loop_frequency;
    
    struct MainLoopConfig {
        static constexpr int frequency = main_loop_frequency;
        using SensorPolicy = decltype(config)::MainLoopSensorPolicy;
    };
    
    template <typename System>
    using MainLoopType = MainLoop<System, MainLoopConfig{}>;

    struct FastLoopConfig {
        static constexpr int frequency = fast_loop_frequency;
        using ControllerType = BasicFOCController;
        using EncoderType = decltype(config)::MotorEncoderType;
        static constexpr int kp = 3;
    };
    
    template <typename System>
    using FastLoopType = FastLoop<System, FastLoopConfig{}>;

};
