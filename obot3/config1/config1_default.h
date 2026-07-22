#pragma once
#include "basic_config.h"
#include "../trace/trace.h"

struct Config1SensorPolicy {
    int update() { count++; return 0; }
    int count = 0;
};

struct DefaultConfig1 : BasicDefaultConfig {
    static constexpr int fast_loop_frequency = 50'000;
    using MainLoopSensorPolicy = Config1SensorPolicy;
};

constexpr auto config1_default_gpio_init = [](){
    GPIOInit init = trace_gpio_init();
    init.a[2].mode = GPIOMode::ALTERNATE;
    return init;
};
