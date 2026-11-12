#pragma once
#include "../config1_default.h"
import device.can_init;
import device.adc_init;
import device.opamp_init;

// OPAMP3 follows PB13 (VINP1) and drives ADC3 IN13 internally, PB1 (VOUT) stays free
constexpr auto config1_opamp_init = OPAMPInit{
    .opamp3 = {
        .enable = OPAMPEnable::ENABLED,
        .input = OPAMPInput::VINP1,
        .mode = OPAMPMode::FOLLOWER,
        .high_speed = OPAMPEnable::ENABLED,
        .internal_output = OPAMPEnable::ENABLED,
    },
};

// ADC3 converts the OPAMP3 output on every HRTIM ADC trigger 1, which
// config1_hrtim_init puts on timer F cmp3
constexpr uint8_t config1_adc_channel = 13;           // ADC3 IN13: OPAMP3 internal output
constexpr auto config1_adc_init = []() consteval {
    ADCInit init{};
    init.adc3 = {
        .enable = ADCEnable::ENABLED,
        .clock = ADCClock::HCLK_DIV4,                 // 42.5 MHz
        .trigger = ADCTrigger::HRTIM_TRG1,
        .trigger_edge = ADCTriggerEdge::RISING,
        .overwrite = ADCEnable::ENABLED,              // DR always holds the latest conversion
    };
    init.adc3.sequence[0] = config1_adc_channel;
    init.adc3.sample_time[config1_adc_channel] = ADCSampleTime::CYCLES_12_5;   // 294 ns, the opamp output needs 200 ns
    return init;
}();

volatile int adc_reading = 0;
struct SpecialEncoder {
    void read() {
        adc_reading = adc_read<3>();
    }
};

struct Config1_1SensorStatus {
    int adc_reading;
};

struct Config1_1SensorPolicy : Config1SensorPolicy {
    Config1_1SensorStatus update() {
        Config1SensorPolicy::update();
        count += 2;
        adc_reading = adc_read<3>();
        return {adc_reading};
    }
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
    init.b[8].mode = GPIOMode::ALTERNATE;    // FDCAN1 RX
    init.b[8].af = GPIOAF::AF9;
    init.b[9].mode = GPIOMode::ALTERNATE;    // FDCAN1 TX
    init.b[9].af = GPIOAF::AF9;
    init.b[9].speed = GPIOSpeed::VERY_HIGH;
    // PB13 (OPAMP3 VINP1) is analog from reset
    return init;
}();

constexpr auto config1_rcc_init = []() consteval {
    return RCCInit{
        .adc345 = RCCEnable::ENABLED,
        .gpioe = RCCEnable::ENABLED,
        .gpiod = RCCEnable::ENABLED,
        .gpioc = RCCEnable::ENABLED,
        .gpiob = RCCEnable::ENABLED,
        .gpioa = RCCEnable::ENABLED,
        .fdcan = RCCEnable::ENABLED,
        .hrtim1 = RCCEnable::ENABLED,
        .syscfg = RCCEnable::ENABLED};    // clocks the opamps
}();

// no spare pin to toggle on this board
inline void config1_toggle() {}

constexpr auto config1_rcc_clock_init = RCCClockInit{.fdcan_clock = RCCFDCANClock::PCLK1};   // 170 MHz

constexpr auto config1_can_init = CANInit{.kernel_clock_hz = config1_rcc_clock_init.cpu_frequency_hz};

inline void config1_device_init() {
    device_opamp_config<config1_opamp_init>();
    device_adc_config<config1_adc_init>();
}
