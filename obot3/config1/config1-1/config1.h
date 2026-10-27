#pragma once
#include "../config1_default.h"

struct SpecialEncoder {
    void read() {}
};

struct Config1_1SensorStatus {
    int adc_reading;
};

struct Config1_1SensorPolicy : Config1SensorPolicy {
    Config1_1SensorStatus update() { Config1SensorPolicy::update(); count += 2; return {adc_reading}; }
    volatile int adc_reading = 0;
};

struct Config1 : DefaultConfig1 {
    using MotorEncoderType = SpecialEncoder;
    using MainLoopSensorPolicy = Config1_1SensorPolicy;
    int special_parameter = 11;
};

constexpr auto config1_gpio_init = []() consteval {
    GPIOInit init = config1_default_gpio_init();
    init.a[2].mode = GPIOMode::ALTERNATE;
    return init;
}();

constexpr auto config1_rcc_init = []() consteval {
    return RCCInit{.gpioe = RCCEnable::ENABLED,
        .gpiod = RCCEnable::ENABLED,
        .gpioc = RCCEnable::ENABLED,
        .gpiob = RCCEnable::ENABLED,
        .gpioa = RCCEnable::ENABLED,
        .hrtim1 = RCCEnable::ENABLED};
}();

// no spare pin to toggle on this board
inline void config1_toggle() {}

constexpr auto config1_rcc_clock_init = RCCClockInit{};

inline void config1_device_init() {}
