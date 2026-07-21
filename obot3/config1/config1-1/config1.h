#include "../config1_default.h"

struct SpecialEncoder {
    void read() {}
};

struct Config1_1SensorStatus {
    int adc_reading;
};

struct Config1_1SensorPolicy : Config1SensorPolicy {
    Config1_1SensorStatus update() { Config1SensorPolicy::update(); count += 2; return {adc_reading}; }
    volatile int adc_reading;
};

struct Config1 : DefaultConfig1 {
    using MotorEncoderType = SpecialEncoder;
    using MainLoopSensorPolicy = Config1_1SensorPolicy;
    int special_parameter = 11;
};

struct Config1Pins : TracePins {
    static constexpr int gpio_a_moder = 0x1234;
};
