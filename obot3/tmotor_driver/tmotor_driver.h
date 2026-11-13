#pragma once
// tmotor_driver: the obot3 counterpart of
// obot_g474/config/config_obot_g474_tmotor_driver.cpp. An obot_g474 motor
// board with
//   - CAN FD on FDCAN3 (PA8 rx, PB4 tx), 1 Mbps / 8 Mbps
//   - an MA732 motor encoder on SPI3, chip select PD2
//   - the output encoder derived from the motor encoder through the 6:1 gear
//   - no torque sensor: its chip selects (PA0, PC3) are parked high
//   - 50 kHz pwm and fast loop, 10 kHz main loop
// It is linked behind the can bootloader and answers the obot text api on the
// can address the bootloader uses, see tmotor_driver.cpp.
#include "obot_g474_motor.h"
import device.can_init;
import device.irq_init;
import device.spi_init;
import device.system;

constexpr uint32_t tmotor_pwm_frequency_hz = 50'000;
constexpr uint32_t tmotor_main_loop_frequency_hz = 10'000;
constexpr int tmotor_gear_ratio = 6;

// The can address is the first byte of the parameters in flash, which this
// firmware leaves alone: the same one the bootloader and the obot_g474
// firmware answer to.
constexpr uint32_t tmotor_param_address = 0x08060000;
inline uint32_t tmotor_can_address() {
    return *reinterpret_cast<const uint8_t*>(tmotor_param_address) & 0x7F;
}

// obot can ids: message << 7 | address
constexpr uint32_t obot_can_ascii = 4, obot_can_ascii_response = 5, obot_can_enum = 0xF;
constexpr uint32_t obot_can_broadcast_address = 0x7F;
constexpr uint32_t obot_can_id(uint32_t message, uint32_t address) { return message << 7 | address; }

// ---------------------------------------------------------------- i/o config

constexpr auto tmotor_rcc_clock_init = RCCClockInit{.fdcan_clock = RCCFDCANClock::PCLK1};   // 170 MHz

constexpr auto tmotor_rcc_init = []() consteval {
    RCCInit init = motor_default_rcc_init();
    init.fdcan = RCCEnable::ENABLED;
    init.spi3 = RCCEnable::ENABLED;
    return init;
}();

constexpr auto tmotor_gpio_init = []() consteval {
    GPIOInit init = motor_default_gpio_init();
    motor_pin_alternate(init.a[8], GPIOAF::AF11, "fdcan3 rx");    // in place of i48v
    motor_pin_alternate(init.b[4], GPIOAF::AF11, "fdcan3 tx");    // taken from the jtag
    init.b[4].pullupd = GPIOPullUpD::NO_PULL;
    motor_pin_output(init.a[0], GPIOSet::SET, "torque sensor cs");   // in place of qepa
    init.a[0].speed = GPIOSpeed::MEDIUM;
    init.a[0].pullupd = GPIOPullUpD::NO_PULL;
    init.c[3].bsrr = GPIOSet::SET;                                // output encoder cs
    return init;
}();

constexpr auto tmotor_hrtim_init = motor_default_hrtim_init(tmotor_pwm_frequency_hz, tmotor_main_loop_frequency_hz);

constexpr auto tmotor_adc_init = []() consteval {
    ADCInit init = motor_default_adc_init();
    init.adc5.injected_sequence_length = 2;     // no i48v: PA8 is the can rx
    return init;
}();

constexpr auto tmotor_opamp_init = motor_default_opamp_init();
constexpr auto tmotor_tim_init = motor_default_tim_init();

constexpr auto tmotor_can_init = CANInit{
    .instance = 2,                              // FDCAN3
    .kernel_clock_hz = tmotor_rcc_clock_init.cpu_frequency_hz,
    .nominal_bitrate_hz = 1'000'000,
    .data_bitrate_hz = 8'000'000,
    .standard_filter_count = 2,
};
constexpr int tmotor_can_ascii_fifo = 1, tmotor_can_enum_fifo = 0;

// The fast loop runs at the end of the ADC5 injected sequence, when the phase
// currents of this pwm period are in; the main loop on the HRTIM master cmp1.
// The fast loop preempts the main loop.
void fast_loop_isr();
void main_loop_isr();
constexpr auto tmotor_irq_init = []() consteval {
    IRQInit init{};
    init.irq[ADC5_IRQn] = {.handler = fast_loop_isr, .priority = 0};
    init.irq[HRTIM1_Master_IRQn] = {.handler = main_loop_isr, .priority = 1};
    return init;
}();

// SPI3 for the MA732: 16 bit frames at pclk / 16 = 10.6 MHz, chip select on PD2
constexpr auto tmotor_spi_init = SPIInit{
    .spi3 = {
        .enable = SPIEnable::ENABLED,
        .baud_rate = SPIBaudRate::PCLK_DIV16,
        .data_bits = 16,
    },
};

// Everything past the clocks and pins, in order. The interrupts come after
// this, in pin_config.
inline void tmotor_device_init() {
    device_opamp_config<tmotor_opamp_init>();
    device_adc_config<tmotor_adc_init>();
    motor_adc_calibrate();
    device_tim_config<tmotor_tim_init>();
    device_spi_config<tmotor_spi_init>();
    device_can_config<tmotor_can_init>();
    can_filter<tmotor_can_init>(0, obot_can_id(obot_can_ascii, tmotor_can_address()), tmotor_can_ascii_fifo);
    can_filter<tmotor_can_init>(1, obot_can_id(obot_can_enum, obot_can_broadcast_address), tmotor_can_enum_fifo);
    device_hrtim_config<tmotor_hrtim_init>();   // last: starts the master and pwm timers in phase, and the ADC triggers with them
}

// ---------------------------------------------------------------- fast loop

// What the fast loop reads every pwm period, for the main loop and the
// debugger. Each is a single word: a read from a lower priority is whole.
struct TMotorFastLoopIO {
    volatile int32_t motor_encoder;     // counts, 65536 per turn, multiturn
    volatile uint32_t ia, ib, ic;       // adc counts
    volatile uint32_t count;            // fast loop passes
};
inline constinit TMotorFastLoopIO tmotor_fast_loop_io{};

// MA732: 16 bit angle per frame, summed over the roll-over. The frame takes
// 1.5 us and is waited for.
struct TMotorEncoder {
    void read() {
        gpio_clear(gpio_port<'D'>(), 2);                // cs low, 80 ns before the clock
        for (int i = 0; i < 4; i++) __asm__ volatile("");
        const uint16_t angle = spi_transfer<3>(0);
        gpio_set(gpio_port<'D'>(), 2);
        count += static_cast<int16_t>(angle - last_angle);
        last_angle = angle;
        tmotor_fast_loop_io.motor_encoder = count;

        // the fast loop's other inputs ride along: this is its only hook
        tmotor_fast_loop_io.ia = motor_ia();
        tmotor_fast_loop_io.ib = motor_ib();
        tmotor_fast_loop_io.ic = motor_ic();
        tmotor_fast_loop_io.count = tmotor_fast_loop_io.count + 1;
    }
    uint16_t last_angle = 0;
    int32_t count = 0;
};

// ---------------------------------------------------------------- main loop

struct TMotorSensorStatus {
    int32_t motor_encoder;
    int32_t output_encoder;             // motor encoder counts through the gear
    uint32_t vbus;                      // adc counts
};

struct TMotorSensorPolicy {
    TMotorSensorStatus update() {
        const int32_t motor_encoder = tmotor_fast_loop_io.motor_encoder;
        status = {motor_encoder, motor_encoder / tmotor_gear_ratio, motor_vbus()};
        count = count + 1;
        return status;
    }
    TMotorSensorStatus status{};
    volatile uint32_t count = 0;        // main loop passes
};

struct TMotorConfig : BasicDefaultConfig {
    constexpr TMotorConfig() {
        fast_loop_frequency = tmotor_pwm_frequency_hz;
        main_loop_frequency = tmotor_main_loop_frequency_hz;
    }
    using MotorEncoderType = TMotorEncoder;
    using MainLoopSensorPolicy = TMotorSensorPolicy;
};
