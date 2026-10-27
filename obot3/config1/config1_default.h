#pragma once
#include "basic_config.h"
#include "../trace/trace.h"
import device.hrtim_init;

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

// HRTIM A: center aligned 100 kHz, 50% on output 1. Reset on cmp1 going up
// (and so set on cmp1 going down): on time = 2 * cmp1, centered on the
// valley, so a higher cmp1 is a higher duty and the crest is the off time center
// HRTIM F: edge aligned over the whole pwm period (/2 prescaler), ADC trigger 1
// on cmp3 anywhere in the period (period / 2 = the crest of A = off time center),
// output 1 toggled by the same event as a monitor
constexpr uint16_t config1_pwm_period = hrtim_period(100'000, 0, HRTIMMode::CENTER_ALIGNED);
constexpr uint16_t config1_trigger_period = hrtim_period(100'000, 1);
constexpr auto config1_hrtim_init = []() consteval {
    HRTIMInit init{};
    init.a = {
        .enable = HRTIMEnable::ENABLED,
        .mode = HRTIMMode::CENTER_ALIGNED,
        .continuous = HRTIMEnable::ENABLED,
        .preload = HRTIMEnable::ENABLED,
        .update_on_reset = HRTIMEnable::ENABLED,
        .period = config1_pwm_period,
        .cmp1 = config1_pwm_period / 2,
        .out1_reset = HRTIMEvent::CMP1,
        .out1 = HRTIMEnable::ENABLED,
    };
    init.f = {
        .enable = HRTIMEnable::ENABLED,
        .prescaler = 1,
        .continuous = HRTIMEnable::ENABLED,
        .preload = HRTIMEnable::ENABLED,
        .update_on_reset = HRTIMEnable::ENABLED,
        .period = config1_trigger_period,
        .cmp3 = config1_trigger_period / 2,   // pwm center
        .out1_set = HRTIMEvent::CMP3,
        .out1_reset = HRTIMEvent::CMP3,       // same event sets and resets: toggle
        .out1 = HRTIMEnable::ENABLED,
    };
    init.adc_trigger1 = HRTIMADCTrigger::TIMF_CMP3;
    init.adc_trigger1_update = HRTIMTimer::F;
    return init;
}();
