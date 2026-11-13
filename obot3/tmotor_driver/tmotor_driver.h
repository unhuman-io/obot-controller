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
#include "control/motor_controller.h"
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
constexpr uint32_t obot_can_command = 1, obot_can_command_status = 2, obot_can_status = 3;
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
    .standard_filter_count = 5,
};
constexpr int tmotor_can_ascii_fifo = 1, tmotor_can_command_fifo = 0;

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
    can_filter<tmotor_can_init>(1, obot_can_id(obot_can_enum, obot_can_broadcast_address), tmotor_can_command_fifo);
    can_filter<tmotor_can_init>(2, obot_can_id(obot_can_command, tmotor_can_address()), tmotor_can_command_fifo);
    can_filter<tmotor_can_init>(3, obot_can_id(obot_can_command_status, tmotor_can_address()), tmotor_can_command_fifo);
    can_filter<tmotor_can_init>(4, obot_can_id(obot_can_status, tmotor_can_address()), tmotor_can_command_fifo);
    device_hrtim_config<tmotor_hrtim_init>();   // last: starts the master and pwm timers in phase, and the ADC triggers with them
}

// ---------------------------------------------------------------- fast loop
// sensors -> FOC -> pwm. The values are those of
// obot_g474/param/param_obot_g474_tmotor_driver.c

// MA732: 16 bit angle per frame, summed over the roll-over. The frame takes
// 1.5 us and is waited for.
struct TMotorEncoder {
    int32_t read() {
        gpio_clear(gpio_port<'D'>(), 2);                // cs low, 80 ns before the clock
        for (int i = 0; i < 4; i++) __asm__ volatile("");
        const uint16_t angle = spi_transfer<3>(0);
        gpio_set(gpio_port<'D'>(), 2);
        count += static_cast<int16_t>(angle - last_angle);
        last_angle = angle;
        return count;
    }
    uint16_t last_angle = 0;
    int32_t count = 0;
};

constexpr float tmotor_num_poles = 14;
constexpr auto tmotor_sensor_param = MotorSensorParam{
    .frequency_hz = tmotor_pwm_frequency_hz,
    .adc_gain = -3.3f / 4096 / (.033f * 5),                 // 33 mohm shunts, driver gain 5: needs the driver set up for it
    .vbus_gain = 3.3f / 4096 * (215 + 13.7f) / 13.7f,       // nominal 3.3 V reference
    .encoder_cpr = 65536,
    .encoder_dir = -1,
    .encoder_rollover = 1 << 24,
    .num_poles = tmotor_num_poles,
    .phase_mode = -1,
    .current_direction = 1,
};

constexpr auto tmotor_foc_param = FOCParam{
    .num_poles = tmotor_num_poles,
    .pi_d = {.kp = 2, .ki = .1f, .ki_limit = 10, .command_max = 10},
    .pi_q = {.kp = 2, .ki = .1f, .ki_limit = 10, .command_max = 10},
    .current_filter_frequency_hz = 40000,
};

// ---------------------------------------------------------------- main loop
// sensors -> the controller of the commanded mode -> the FOC command

constexpr auto tmotor_controller_param = MotorControllerParam{
    .position = {.kp = 20, .kd = .2f, .command_max = 3, .velocity_filter_frequency_hz = 200},
    .velocity = {.kp = .2f, .ki = 20, .ki_limit = 3, .command_max = 3, .output_filter_frequency_hz = 200},
    .acceleration_limit = 1000,
    .rollover = 2 * std::numbers::pi_v<float> * (1 << 24) / 65536,
};

// No output encoder and no torque sensor: the output position is the motor's
// through the gear
struct TMotorSensorStatus {
    float output_position;              // rad
    float torque;                       // Nm
};

struct TMotorSensorPolicy {
    template<typename FastLoopStatus>
    TMotorSensorStatus update(const FastLoopStatus& fast_loop) {
        count = count + 1;
        return {fast_loop.measured.motor_position / tmotor_gear_ratio, 0};
    }
    volatile uint32_t count = 0;        // main loop passes
};

// The host's command, written from the background as frames arrive
struct TMotorHostCommand {
    static inline constinit Latest<obot::MotorCommand> latest{};
    static obot::MotorCommand read() { return latest.read(); }
};

struct TMotorConfig : BasicDefaultConfig {
    constexpr TMotorConfig() {
        fast_loop_frequency = tmotor_pwm_frequency_hz;
        main_loop_frequency = tmotor_main_loop_frequency_hz;
    }
    using FastLoopSensorPolicy = MotorFastLoopSensors<tmotor_sensor_param, TMotorEncoder>;
    using FastLoopControllerType = FOC<tmotor_foc_param, 1.f / tmotor_pwm_frequency_hz>;
    using FastLoopOutputPolicy = MotorPWM<tmotor_pwm_frequency_hz>;
    using MainLoopSensorPolicy = TMotorSensorPolicy;
    using MainLoopControllerPolicy = MotorControllerPolicy<tmotor_controller_param, 1.f / tmotor_main_loop_frequency_hz, TMotorHostCommand>;
};
