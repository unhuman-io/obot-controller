#include "basic_config.h"

struct Config1SensorPolicy {
    int update() { count++; return 0; }
    int count = 0;
};

struct DefaultConfig1 : BasicDefaultConfig {
    static constexpr int fast_loop_frequency = 50'000;
    using MainLoopSensorPolicy = Config1SensorPolicy;
};
