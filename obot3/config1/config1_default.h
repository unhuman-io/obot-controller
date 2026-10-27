#pragma once
#include "basic_config.h"
#include "../trace/trace.h"
#include "hrtim_init.h"

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

// HRTIM A: center aligned 100 kHz, 50% on output 1
// HRTIM F: edge aligned 100 kHz, ADC trigger 1 on cmp3 at 50%
constexpr auto config1_hrtim_init = []() consteval {
    HRTIMInit init{};
    init.a = {
        .enable = HRTIMEnable::ENABLED,
        .mode = HRTIMMode::CENTER_ALIGNED,
        .continuous = HRTIMEnable::ENABLED,
        .preload = HRTIMEnable::ENABLED,
        .update_on_reset = HRTIMEnable::ENABLED,
        .period = hrtim_period(100'000, 0, HRTIMMode::CENTER_ALIGNED),
        .cmp1 = static_cast<uint16_t>(hrtim_period(100'000, 0, HRTIMMode::CENTER_ALIGNED) / 2),
        .out1_set = HRTIMEvent::CMP1,
        .out1 = HRTIMEnable::ENABLED,
    };
    init.f = {
        .enable = HRTIMEnable::ENABLED,
        .continuous = HRTIMEnable::ENABLED,
        .preload = HRTIMEnable::ENABLED,
        .update_on_reset = HRTIMEnable::ENABLED,
        .period = hrtim_period(100'000),
        .cmp3 = static_cast<uint16_t>(hrtim_period(100'000) / 2),
    };
    init.adc_trigger1 = HRTIMADCTrigger::TIMF_CMP3;
    init.adc_trigger1_update = HRTIMTimer::F;
    return init;
}();
