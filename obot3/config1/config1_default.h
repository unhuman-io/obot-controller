#include "basic_config.h"

struct DefaultConfig1 : BasicDefaultConfig {
    static constexpr int fast_loop_frequency = 50'000;
};
