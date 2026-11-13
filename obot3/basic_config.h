#pragma once
#include "loops.h"
#include "basic_foc_controller.h"

struct DefaultFastLoopSensorPolicy {
    int update() { return 0; }
};

struct DefaultFastLoopOutputPolicy {
    template<typename Output, typename Measured>
    void update(const Output &output, const Measured &measured) {}
};

struct DefaultSensorPolicy {
    int update() { return 0; }
};

struct DefaultControllerPolicy {
    template<typename Status>
    typename Status::ExpectedCommand update(const Status &status) { count++; return {}; }
    int count = 0;
};

struct BasicDefaultConfig {
    int fast_loop_frequency = 100'000;
    int main_loop_frequency = 10'000;
    using FastLoopSensorPolicy = DefaultFastLoopSensorPolicy;
    using FastLoopControllerType = BasicFOCController;
    using FastLoopOutputPolicy = DefaultFastLoopOutputPolicy;
    using MainLoopSensorPolicy = DefaultSensorPolicy;
    using MainLoopControllerPolicy = DefaultControllerPolicy;
};

template<auto config = BasicDefaultConfig{}>
struct BasicConfig {
    static constexpr int fast_loop_frequency = config.fast_loop_frequency;
    static constexpr int main_loop_frequency = config.main_loop_frequency;
    
    struct MainLoopConfig {
        static constexpr int frequency = main_loop_frequency;
        using SensorPolicy = decltype(config)::MainLoopSensorPolicy;
        using ControllerPolicy = decltype(config)::MainLoopControllerPolicy;
    };
    
    template <typename System>
    using MainLoopType = MainLoop<System, MainLoopConfig{}>;

    struct FastLoopConfig {
        static constexpr int frequency = fast_loop_frequency;
        using SensorPolicy = decltype(config)::FastLoopSensorPolicy;
        using ControllerType = decltype(config)::FastLoopControllerType;
        using OutputPolicy = decltype(config)::FastLoopOutputPolicy;
    };
    
    template <typename System>
    using FastLoopType = FastLoop<System, FastLoopConfig{}>;

};
