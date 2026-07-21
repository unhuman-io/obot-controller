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

struct DefaultConfig1Pins : TracePins {
    static constexpr int gpio_a_moder = 0x1234;
};
