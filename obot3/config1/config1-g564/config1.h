#pragma once
#include "../config1_default.h"

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
    return init;
}();

constexpr auto config1_rcc_init = []() consteval {
    return RCCInit{.gpiog = RCCEnable::ENABLED,
        .gpioe = RCCEnable::ENABLED,
        .gpiod = RCCEnable::ENABLED,
        .gpioc = RCCEnable::ENABLED,
        .gpiob = RCCEnable::ENABLED,
        .gpioa = RCCEnable::ENABLED,
        .hrtim1 = RCCEnable::ENABLED};
}();

inline void config1_toggle() {
    gpio_toggle(GPIOG, 3);
}
