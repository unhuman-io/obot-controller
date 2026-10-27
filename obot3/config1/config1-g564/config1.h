#pragma once
#include "../config1_default.h"
#include "play_init.h"

struct SpecialEncoder {
    void read() {}
};

struct Config1G564SensorStatus {
    int adc_reading;
};

struct Config1G564SensorPolicy : Config1SensorPolicy {
    Config1G564SensorStatus update() { Config1SensorPolicy::update(); count += 2; return {adc_reading}; }
    volatile int adc_reading = 0;
};

struct Config1 : DefaultConfig1 {
    using MotorEncoderType = SpecialEncoder;
    using MainLoopSensorPolicy = Config1G564SensorPolicy;
    int special_parameter = 11;
};

constexpr auto config1_gpio_init = []() consteval {
    GPIOInit init = config1_default_gpio_init();
    init.a[2].mode = GPIOMode::ALTERNATE;
    init.g[3].mode = GPIOMode::OUTPUT;
    init.b[5].mode = GPIOMode::ALTERNATE;
    init.b[5].af = GPIOAF::AF12;
    init.d[11].mode = GPIOMode::ALTERNATE;   // PLAY_OUT0
    init.d[11].af = GPIOAF::AF14;
    init.d[11].speed = GPIOSpeed::VERY_HIGH;  // adc trigger pulse is a few hrtim clocks wide
    return init;
}();

constexpr auto config1_rcc_init = []() consteval {
    return RCCInit{.gpiog = RCCEnable::ENABLED,
        .gpioe = RCCEnable::ENABLED,
        .gpiod = RCCEnable::ENABLED,
        .gpioc = RCCEnable::ENABLED,
        .gpiob = RCCEnable::ENABLED,
        .gpioa = RCCEnable::ENABLED,
        .playapb = RCCEnable::ENABLED,
        .play = RCCEnable::ENABLED,
        .hrtim1 = RCCEnable::ENABLED};
}();

// 280 MHz from the 24 MHz crystal (24 / 3 = 8 MHz, x 70 = 560 MHz VCO, / 2);
// the PLAY gets the same clock from PLL2 so it can catch one-cycle HRTIM triggers
constexpr auto config1_rcc_clock_init = RCCClockInit{
    .cpu_frequency_hz = 280'000'000,
    .pll_source = RCCPLLSource::HSE,
    .hse_frequency_hz = 24'000'000,
    .pll2_frequency_hz = 280'000'000,
    .play_clock = RCCKernelClock::PLL2P};

// PLAY -> PLAY_OUT0 (PD11), two monitors of the ADC trigger:
//   toggle: input 15 source 7 (HRTIM1 CHF1, toggled on every timer F cmp3)
//   pulse:  input 1 source 5 (HRTIM1 ADC trigger 1, a one hrtim clock pulse)
constexpr bool config1_play_toggle = true;
constexpr auto config1_play_init = []() consteval {
    PLAYInit init{};
    if constexpr (config1_play_toggle) {
        init.filter[15] = {.premux_select = 7};
        init.le[1] = {.lut = play_lut_passthrough(0), .in0 = play_in_filter(15)};
    } else {
        init.filter[1] = {.premux_select = 5};
        init.le[1] = {.lut = play_lut_passthrough(0), .in0 = play_in_filter(1)};
    }
    init.out[0] = {.select = play_out_le_direct(1)};   // PD11 is PLAY_OUT0
    return init;
}();

inline void config1_device_init() {
    device_play_config<config1_play_init>();
}

inline void config1_toggle() {
    gpio_toggle(GPIOG, 3);
}
