module;
#include <cstdint>
#include <numbers>
#include "../motorlib/task.h"   // device header, get_clock, CycleScheduler
#include "../motorlib/motor_messages/motor_messages.h"
#include "basic_config.h"

export module tmotor_driver;
export import device.rcc_init;
export import device.gpio_init;
export import device.hrtim_init;
export import device.adc_init;
export import device.opamp_init;
export import device.tim_init;
export import device.can_init;
export import device.irq_init;
export import device.spi_init;
export import device.drv8323_init;
export import device.system;
export import control.motor_controller;

// tmotor_driver: the obot3 counterpart of
// obot_g474/config/config_obot_g474_tmotor_driver.cpp. Only what this
// configuration uses of the board is set up:
//   - CAN FD on FDCAN3 (PA8 rx, PB4 tx), 1 Mbps / 8 Mbps
//   - an MA732 motor encoder on SPI3, chip select PD2
//   - the three phase pwm on HRTIM timers D, F and E
//   - the DRV8323S gate driver on SPI1, chip select PA4, enable PC13, fault PC14
//   - phase currents and bus voltage through the opamps to the ADCs
//   - the led on TIM4
//   - the unused chip selects parked high
//   - 50 kHz pwm and fast loop, 10 kHz main loop
// The output encoder is the motor encoder through the 6:1 gear; there is no
// torque sensor. It is linked behind the can bootloader and answers the obot
// protocol on the can address the bootloader uses, see tmotor_driver.cpp.
export {

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

constexpr auto tmotor_rcc_init = RCCInit{
    .adc345 = RCCEnable::ENABLED,
    .adc12 = RCCEnable::ENABLED,
    .gpiod = RCCEnable::ENABLED,
    .gpioc = RCCEnable::ENABLED,
    .gpiob = RCCEnable::ENABLED,
    .gpioa = RCCEnable::ENABLED,
    .fdcan = RCCEnable::ENABLED,
    .spi3 = RCCEnable::ENABLED,
    .tim4 = RCCEnable::ENABLED,       // led
    .hrtim1 = RCCEnable::ENABLED,
    .spi1 = RCCEnable::ENABLED,
    .syscfg = RCCEnable::ENABLED};    // clocks the opamps

// Pins that are not mentioned stay analog, which is also what the ADC and
// opamp inputs need: those only get a label.
constexpr auto tmotor_gpio_init = []() consteval {
    constexpr auto alternate = []<std::size_t N>(auto& pin, GPIOAF af, const char (&function)[N]) consteval {
        pin.mode = GPIOMode::ALTERNATE;
        pin.speed = GPIOSpeed::VERY_HIGH;
        pin.af = af;
        pin.pullupd = GPIOPullUpD::NO_PULL;
        pin.function = function;
    };
    constexpr auto output = []<std::size_t N>(auto& pin, GPIOSet level, const char (&function)[N]) consteval {
        pin.mode = GPIOMode::OUTPUT;
        pin.bsrr = level;
        pin.function = function;
    };
    GPIOInit init = get_default_gpio_init();    // swd on PA13, PA14 and PB3

    alternate(init.a[8], GPIOAF::AF11, "fdcan3 rx");
    alternate(init.b[4], GPIOAF::AF11, "fdcan3 tx");    // taken from the jtag

    alternate(init.c[10], GPIOAF::AF6, "spi3 clk");
    alternate(init.c[11], GPIOAF::AF6, "spi3 hido");
    alternate(init.c[12], GPIOAF::AF6, "spi3 hodi");
    output(init.d[2], GPIOSet::SET, "spi3 cs motor encoder");
    init.d[2].speed = GPIOSpeed::VERY_HIGH;

    // motor pwm, phases a, b, c
    alternate(init.b[14], GPIOAF::AF13, "hrtim1 chd1");
    alternate(init.b[15], GPIOAF::AF13, "hrtim1 chd2");
    alternate(init.c[6], GPIOAF::AF13, "hrtim1 chf1");
    alternate(init.c[7], GPIOAF::AF13, "hrtim1 chf2");
    alternate(init.c[8], GPIOAF::AF3, "hrtim1 che1");
    alternate(init.c[9], GPIOAF::AF3, "hrtim1 che2");

    // gate driver
    alternate(init.a[5], GPIOAF::AF5, "spi1 clk");
    alternate(init.a[6], GPIOAF::AF5, "spi1 hido");
    alternate(init.a[7], GPIOAF::AF5, "spi1 hodi");
    init.a[6].pullupd = GPIOPullUpD::PULL_UP;           // the driver's output is open drain
    output(init.a[4], GPIOSet::SET, "spi1 cs drv8323s");
    init.a[4].speed = GPIOSpeed::VERY_HIGH;
    output(init.c[13], GPIOSet::RESET, "drv enable");   // low until tmotor_device_init
    init.c[14].mode = GPIOMode::INPUT;
    init.c[14].pullupd = GPIOPullUpD::PULL_UP;
    init.c[14].function = "drv fault, active low";

    output(init.a[0], GPIOSet::SET, "torque sensor cs");    // not used: parked high
    output(init.c[3], GPIOSet::SET, "output encoder cs");

    alternate(init.b[6], GPIOAF::AF2, "tim4 ch1 led r");
    alternate(init.b[7], GPIOAF::AF2, "tim4 ch2 led g");
    alternate(init.b[8], GPIOAF::AF2, "tim4 ch3 led b");

    init.a[3].function = "vbus, opamp1 vinp1";
    init.b[13].function = "ia, opamp3 vinp1";
    init.b[12].function = "ib, opamp6 vinp0";
    init.b[11].function = "ic, opamp4 vinp2";
    return init;
}();

// Followers on a pin with the internal output. They stay on the factory trim:
// opamp_calibrate_async drives the VOUT pin, and PB11 and PB12 are each
// other's inputs.
constexpr auto tmotor_opamp_init = []() consteval {
    constexpr auto follower = [](OPAMPInput input) consteval {
        return OPAMPAmplifierInit{
            .enable = OPAMPEnable::ENABLED,
            .input = input,
            .mode = OPAMPMode::FOLLOWER,
            .high_speed = OPAMPEnable::ENABLED,
            .internal_output = OPAMPEnable::ENABLED,
        };
    };
    return OPAMPInit{
        .opamp1 = follower(OPAMPInput::VINP1),   // PA3  vbus -> ADC1 IN13
        .opamp3 = follower(OPAMPInput::VINP1),   // PB13 ia   -> ADC3 IN13
        .opamp4 = follower(OPAMPInput::VINP2),   // PB11 ic   -> ADC5 IN5
        .opamp6 = follower(OPAMPInput::VINP0),   // PB12 ib   -> ADC4 IN17
    };
}();

// HRTIM. Timers D, F and E are the pwm of phases a, b and c: center aligned,
// output 1 goes active on cmp1 on the way up and inactive on the way down, so
// the crest is the center of the on time and a lower cmp1 is a higher duty.
// Output 2 is the complement, with dead time. The roll-over is at the valley
// only, so a new cmp1 takes effect once per pwm period. The outputs are not
// enabled here: that is up to TMotorPWM.
// Timer F also places the ADC triggers:
//   trigger 1 on cmp4, just after the valley = the center of the off time,
//             where the low side shunts carry the phase current
//   trigger 3 on cmp3, just before the crest, for the zero current readings
// The master timer is the main loop's time base: its cmp1 raises the
// interrupt, and its roll-over is the update event of the ADC triggers.
constexpr uint16_t tmotor_pwm_period = hrtim_period(tmotor_pwm_frequency_hz, 0, HRTIMMode::CENTER_ALIGNED);
constexpr uint16_t tmotor_adc_trigger_offset = 64;  // hrtim ticks, the smallest compare value that works
constexpr uint16_t tmotor_deadtime_ticks = 68;      // 50 ns, in hrtim clock / 4 * 32 ticks
constexpr uint8_t tmotor_master_prescaler = 7;      // 42.5 MHz: 10 kHz is 4250 counts
constexpr auto tmotor_hrtim_init = []() consteval {
    const HRTIMTimerInit pwm = {
        .enable = HRTIMEnable::ENABLED,
        .mode = HRTIMMode::CENTER_ALIGNED,
        .rollover = HRTIMRollover::VALLEY,
        .continuous = HRTIMEnable::ENABLED,
        .preload = HRTIMEnable::ENABLED,
        .update_on_reset = HRTIMEnable::ENABLED,
        .period = tmotor_pwm_period,
        .cmp1 = static_cast<uint16_t>(tmotor_pwm_period / 2),   // 50 % on all phases: 0 V
        .out1_set = HRTIMEvent::CMP1,
        .deadtime = HRTIMEnable::ENABLED,
        .deadtime_rising = tmotor_deadtime_ticks,
        .deadtime_falling = tmotor_deadtime_ticks,
    };
    HRTIMInit init{};
    init.d = pwm;
    init.e = pwm;
    init.f = pwm;
    init.f.adc_rollover = HRTIMRollover::CREST;
    init.f.cmp3 = tmotor_pwm_period - tmotor_adc_trigger_offset - 1;
    init.f.cmp4 = tmotor_adc_trigger_offset;
    init.adc_trigger1 = HRTIMADCTrigger::TIMF_CMP4;
    init.adc_trigger3 = HRTIMADCTrigger::TIMF_CMP3;
    init.master = {
        .enable = HRTIMEnable::ENABLED,
        .prescaler = tmotor_master_prescaler,
        .continuous = HRTIMEnable::ENABLED,
        .preload = HRTIMEnable::ENABLED,
        .update_on_period = HRTIMEnable::ENABLED,
        .cmp1_interrupt = HRTIMEnable::ENABLED,           // the main loop
        .period = hrtim_period(tmotor_main_loop_frequency_hz, tmotor_master_prescaler),
        .cmp1 = 400,
        .sync_out = HRTIMSyncOut::POSITIVE_PULSE,
        .sync_source = HRTIMSyncSource::MASTER_START,
    };
    return init;
}();

// ADC. The current sense converters take two sets of readings per pwm period:
//   injected, HRTIM trigger 1: the phase currents. The channel is converted
//     twice and the first result is used (errata). The end of the ADC5
//     sequence is the fast loop interrupt.
//   regular, HRTIM trigger 3: the same channel with the low side off, for the
//     zero current offset
// ADC1 regular follows the bus voltage; its injected sequence, which software
// starts with adc_start_injected<1>(), is the temperature sensor and vrefint.
//
//         regular       injected
//   ADC1  IN13 vbus     IN16 temperature, IN18 vrefint
//   ADC3  IN13 ia       IN13 ia, IN13
//   ADC4  IN17 ib       IN17 ib, IN17
//   ADC5  IN5 ic        IN5 ic, IN5
constexpr auto tmotor_adc_init = []() consteval {
    constexpr auto converter = []() consteval {
        return ADCConverterInit{
            .enable = ADCEnable::ENABLED,
            .clock = ADCClock::HCLK_DIV4,                 // 42.5 MHz
            .vrefint = ADCEnable::ENABLED,
            .overwrite = ADCEnable::ENABLED,              // DR always holds the latest conversion
        };
    };
    constexpr auto current_sense = [converter](uint8_t opamp_channel) consteval {
        ADCConverterInit c = converter();
        c.trigger = ADCTrigger::HRTIM_TRG3;
        c.trigger_edge = ADCTriggerEdge::RISING;
        c.sequence[0] = opamp_channel;
        c.injected_trigger = ADCInjectedTrigger::ADC345_HRTIM_TRG1;
        c.injected_trigger_edge = ADCTriggerEdge::RISING;
        c.injected_sequence_length = 2;
        c.injected_sequence[0] = opamp_channel;
        c.injected_sequence[1] = opamp_channel;
        c.sample_time[opamp_channel] = ADCSampleTime::CYCLES_12_5;   // 294 ns, the opamp output needs 200 ns
        return c;
    };

    ADCInit init{};
    init.adc1 = converter();
    init.adc1.temperature_sensor = ADCEnable::ENABLED;
    init.adc1.trigger = ADCTrigger::HRTIM_TRG1;
    init.adc1.trigger_edge = ADCTriggerEdge::RISING;
    init.adc1.sequence[0] = 13;
    init.adc1.sample_time[13] = ADCSampleTime::CYCLES_12_5;
    init.adc1.injected_sequence_length = 2;
    init.adc1.injected_sequence[0] = 16;
    init.adc1.injected_sequence[1] = 18;
    init.adc1.sample_time[16] = ADCSampleTime::CYCLES_247_5;    // 5.8 us, the internal channels need 5
    init.adc1.sample_time[18] = ADCSampleTime::CYCLES_247_5;
    init.adc3 = current_sense(13);
    init.adc4 = current_sense(17);
    init.adc5 = current_sense(5);
    init.adc5.injected_end_of_sequence_interrupt = ADCEnable::ENABLED;
    return init;
}();

// The readings by name, adc counts
inline uint32_t tmotor_ia() { return adc_read_injected<3>(0); }
inline uint32_t tmotor_ib() { return adc_read_injected<4>(0); }
inline uint32_t tmotor_ic() { return adc_read_injected<5>(0); }
inline uint32_t tmotor_vbus() { return adc_read<1>(); }
inline uint32_t tmotor_temperature() { return adc_read_injected<1>(0); }
inline uint32_t tmotor_vrefint() { return adc_read_injected<1>(1); }

// For the handlers of the two loops: the fast loop runs at the end of the
// ADC5 injected sequence, when the phase currents of this pwm period are in;
// the main loop on the HRTIM master cmp1
inline void tmotor_fast_loop_irq_clear() { adc_injected_end_of_sequence_clear<5>(); }
inline void tmotor_main_loop_irq_clear() { hrtim_master_cmp1_interrupt_clear(); }

// TIM4 is the led pwm, 0 (off) to 65535 on each color
constexpr auto tmotor_tim_init = []() consteval {
    constexpr TIMChannelInit led = {
        .output = TIMEnable::ENABLED,
        .mode = TIMChannelMode::PWM1,
        .preload = TIMEnable::ENABLED,
    };
    return TIMInit{
        .tim4 = {
            .enable = TIMEnable::ENABLED,
            .ch1 = led,
            .ch2 = led,
            .ch3 = led,
        },
    };
}();
inline void tmotor_led(uint16_t red, uint16_t green, uint16_t blue) {
    tim_compare<4, 1>(red);
    tim_compare<4, 2>(green);
    tim_compare<4, 3>(blue);
}

constexpr auto tmotor_can_init = CANInit{
    .instance = 2,                              // FDCAN3
    .kernel_clock_hz = tmotor_rcc_clock_init.cpu_frequency_hz,
    .nominal_bitrate_hz = 1'000'000,
    .data_bitrate_hz = 8'000'000,
    .standard_filter_count = 5,
};
constexpr int tmotor_can_ascii_fifo = 1, tmotor_can_command_fifo = 0;

// SPI1 for the gate driver: 16 bit frames at pclk / 256 = 664 kHz, sampled on
// the falling edge, chip select on PA4
// SPI3 for the MA732: 16 bit frames at pclk / 16 = 10.6 MHz, chip select on PD2
constexpr auto tmotor_spi_init = SPIInit{
    .spi1 = {
        .enable = SPIEnable::ENABLED,
        .baud_rate = SPIBaudRate::PCLK_DIV256,
        .clock_phase = SPIClockPhase::SECOND_EDGE,
        .data_bits = 16,
    },
    .spi3 = {
        .enable = SPIEnable::ENABLED,
        .baud_rate = SPIBaudRate::PCLK_DIV16,
        .data_bits = 16,
    },
};

// The DRV8323S gate driver, as the drv_regs of
// obot_g474/param/param_obot_g474_tmotor_driver.c
constexpr auto tmotor_drv8323_init = DRV8323Init{
    .pwm_mode = DRV8323PWMMode::PWM_6X,
    .high_side_source_current = 15,                 // 1 A
    .high_side_sink_current = 15,                   // 2 A
    .cycle_by_cycle = DRV8323Enable::DISABLED,
    .drive_time = DRV8323DriveTime::NS_4000,
    .low_side_source_current = 7,                   // 0.57 A
    .low_side_sink_current = 15,                    // 2 A
    .ocp_retry = DRV8323OCPRetry::MS_4,
    .dead_time = DRV8323DeadTime::NS_50,
    .ocp_mode = DRV8323OCPMode::LATCHED,
    .ocp_deglitch = DRV8323OCPDeglitch::US_2,
    .vds_level = 4,                                 // 0.31 V
    .bidirectional = DRV8323Enable::ENABLED,
    .csa_gain = DRV8323CSAGain::X5,
    .sense_level = DRV8323SenseLevel::V_0_25,
};
static_assert(get_drv8323_reg(tmotor_drv8323_init, 0) == 0x000 && get_drv8323_reg(tmotor_drv8323_init, 1) == 0x3FF &&
    get_drv8323_reg(tmotor_drv8323_init, 2) == 0x37F && get_drv8323_reg(tmotor_drv8323_init, 3) == 0x004 &&
    get_drv8323_reg(tmotor_drv8323_init, 4) == 0x200, "the register values of the obot_g474 tmotor_driver");

// How the driver is reached
struct TMotorDRV8323IO {
    static void wait_ns_400() {
        for (int i = 0; i < 100; i++) __asm__ volatile("");     // more than 68 cpu cycles
    }
    static uint16_t transfer(uint16_t word) {
        gpio_clear(gpio_port<'A'>(), 4);
        wait_ns_400();                              // 50 ns needed
        const uint16_t reply = spi_transfer<1>(word);
        wait_ns_400();
        gpio_set(gpio_port<'A'>(), 4);
        wait_ns_400();                              // chip select high between frames
        return reply;
    }
    static void enable(bool on) {
        if (on) gpio_set(gpio_port<'C'>(), 13);
        else gpio_clear(gpio_port<'C'>(), 13);
    }
    static void delay_us(uint32_t us) {
        const uint32_t start = get_clock();
        while (get_clock() - start < us * (cpu_frequency_hz / 1'000'000));
    }
};

// Result of the driver's configuration: 0 once every register has read back
// as written. While it isn't, the driver is disabled and TMotorPWM keeps the
// outputs off.
inline constinit volatile uint32_t tmotor_drv8323_errors = ~0u;
inline bool tmotor_drv8323_fault() { return !gpio_read(gpio_port<'C'>(), 14); }
inline uint32_t tmotor_drv8323_status() { return drv8323_status<TMotorDRV8323IO>(); }

// Everything past the clocks and pins, in order: tmotor_analog_init(), the
// ADC calibration, tmotor_device_init(), and then the interrupts. The
// calibration is in tmotor_driver.cpp: gcc 15 fails on its coroutines when
// they are instantiated in a module.
inline void tmotor_analog_init() {
    device_opamp_config<tmotor_opamp_init>();
    device_adc_config<tmotor_adc_init>();
}

inline void tmotor_device_init() {
    device_tim_config<tmotor_tim_init>();
    device_spi_config<tmotor_spi_init>();
    // the gate driver comes up with the pwm outputs off: its inputs are low
    const uint32_t drv8323_errors = drv8323_config<tmotor_drv8323_init, TMotorDRV8323IO>();
    if (drv8323_errors != 0) drv8323_disable<TMotorDRV8323IO>();
    tmotor_drv8323_errors = drv8323_errors;
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

constexpr float tmotor_num_poles = 14;                  // electrical revolutions per motor revolution
constexpr auto tmotor_foc_param = FOCParam{
    .num_poles = tmotor_num_poles,
    .pi_d = {.kp = 2, .ki = .1f, .ki_limit = 10, .command_max = 10},
    .pi_q = {.kp = 2, .ki = .1f, .ki_limit = 10, .command_max = 10},
    .current_filter_frequency_hz = 40000,
};

struct TMotorMeasurement {
    uint32_t timestamp = 0;                 // cpu cycles
    float i_a = 0, i_b = 0, i_c = 0;        // A
    float vbus = 0;                         // V
    int32_t motor_encoder = 0;              // counts
    float motor_electrical_position = 0;    // rad at the motor, from the electrical zero, for the FOC
    float motor_position = 0;               // rad
    float motor_velocity = 0;               // rad/s
};

// Phase currents, bus voltage and the MA732 motor encoder: a 16 bit angle
// per frame, summed over its roll-over. The frame takes 1.5 us and is waited
// for.
struct TMotorFastLoopSensors {
    static constexpr float adc_gain = -3.3f / 4096 / (.033f * drv8323_csa_gain(tmotor_drv8323_init));   // A per count from mid scale: 33 mohm shunts through the driver's amplifiers
    static constexpr float vbus_gain = 3.3f / 4096 * (215 + 13.7f) / 13.7f;    // V per count, nominal 3.3 V reference
    static constexpr int32_t encoder_cpr = 65536;
    static constexpr int32_t encoder_dir = -1;                      // sign of the motor position
    static constexpr int32_t encoder_rollover = 1 << 24;            // counts: the position wraps at +-rollover
    static constexpr int32_t phase_mode = -1;                       // two motor wires swapped
    static constexpr float rad_per_count = 2 * std::numbers::pi_v<float> / encoder_cpr;
    // the electrical zero sits half an electrical revolution from where a phase lock holds the rotor
    static constexpr int32_t electrical_zero_offset = encoder_cpr / static_cast<int32_t>(tmotor_num_poles) / 2;

    int32_t read_encoder() {
        gpio_clear(gpio_port<'D'>(), 2);                // cs low, 80 ns before the clock
        for (int i = 0; i < 4; i++) __asm__ volatile("");
        const uint16_t angle = spi_transfer<3>(0);
        gpio_set(gpio_port<'D'>(), 2);
        const int32_t difference = static_cast<int16_t>(angle - last_angle);
        last_angle = angle;
        return difference;
    }

    TMotorMeasurement update() {
        TMotorMeasurement m;
        m.timestamp = get_clock();
        m.i_a = adc_gain * (static_cast<int32_t>(tmotor_ia()) - 2048);
        m.i_b = adc_gain * (static_cast<int32_t>(tmotor_ib()) - 2048);
        m.i_c = adc_gain * (static_cast<int32_t>(tmotor_ic()) - 2048);
        m.vbus = vbus_gain * tmotor_vbus();

        const int32_t difference = read_encoder();
        motor_encoder += difference;
        motor_encoder_wrap = wrap1(motor_encoder_wrap + difference, encoder_rollover);
        m.motor_encoder = motor_encoder;
        m.motor_position = encoder_dir * rad_per_count * motor_encoder_wrap;
        m.motor_velocity = encoder_dir * rad_per_count * tmotor_pwm_frequency_hz * difference;
        m.motor_electrical_position = phase_mode * rad_per_count * (motor_encoder_wrap - electrical_zero - electrical_zero_offset);
        return m;
    }

    // During a phase lock the rotor is held at a known electrical angle
    void feedback(const FOCStatus& foc) {
        if (foc.mode == FOCMode::PHASE_LOCK) electrical_zero = motor_encoder_wrap;
    }

    uint16_t last_angle = 0;
    int32_t motor_encoder = 0;          // counts, wraps with the int32
    int32_t motor_encoder_wrap = 0;     // counts, wraps at the rollover
    int32_t electrical_zero = 0;        // counts, until a phase lock measures it
};

// The power stage: phase voltages to the compare values of timers D, F and E,
// and the state of their outputs. They stay off while the gate driver is not
// configured.
// A pulse is at least 1 us at either end, which the driver and the current
// sense need.
struct TMotorPWM {
    static constexpr float period = tmotor_pwm_period;
    static constexpr float min_pulse_ticks = 2 * 1000 * 1.36f;      // 1 us in hrtim clock / 4 * 32 ticks, both edges
    static constexpr float pwm_min = min_pulse_ticks, pwm_max = period - min_pulse_ticks;
    static constexpr float vbus_min = 7;                            // V: below this the voltage scale is held
    static constexpr HRTIMTimer phase_a = HRTIMTimer::D, phase_b = HRTIMTimer::F, phase_c = HRTIMTimer::E;
    static constexpr uint32_t high_sides = hrtim_output_mask(phase_a, true, false) | hrtim_output_mask(phase_b, true, false) |
        hrtim_output_mask(phase_c, true, false);
    static constexpr uint32_t low_sides = hrtim_output_mask(phase_a, false, true) | hrtim_output_mask(phase_b, false, true) |
        hrtim_output_mask(phase_c, false, true);

    void update(const FOCStatus& foc, const TMotorMeasurement& measured) {
        const FOCMode wanted = tmotor_drv8323_errors == 0 ? foc.mode : FOCMode::OPEN;
        if (wanted != mode) set_mode(wanted);
        const float v_to_pwm = period / (measured.vbus > vbus_min ? measured.vbus : vbus_min);
        hrtim_compare1(phase_a, fsat2(foc.v_a * v_to_pwm + period / 2, pwm_min, pwm_max));
        hrtim_compare1(phase_b, fsat2(foc.v_b * v_to_pwm + period / 2, pwm_min, pwm_max));
        hrtim_compare1(phase_c, fsat2(foc.v_c * v_to_pwm + period / 2, pwm_min, pwm_max));
    }

    void set_mode(FOCMode new_mode) {
        switch (new_mode) {
            case FOCMode::OPEN:
                hrtim_output_disable(high_sides | low_sides);
                break;
            case FOCMode::BRAKE:
                hrtim_output_disable(high_sides);
                hrtim_output_enable(low_sides);
                hrtim_update_disable();
                for (HRTIMTimer timer : {phase_a, phase_b, phase_c}) {
                    hrtim_output1_events(timer, HRTIMEvent::NONE, HRTIMEvent::SOFTWARE);    // high side inactive: low side on
                }
                hrtim_update_enable();
                break;
            default:    // pwm
                hrtim_update_disable();
                for (HRTIMTimer timer : {phase_a, phase_b, phase_c}) {
                    hrtim_output1_events(timer, HRTIMEvent::CMP1, HRTIMEvent::NONE);
                }
                hrtim_update_enable();
                hrtim_output_enable(high_sides | low_sides);
                break;
        }
        mode = new_mode;
    }

    FOCMode mode = FOCMode::OPEN;
};

// ---------------------------------------------------------------- main loop
// sensors -> the controller of the commanded mode -> the FOC command

constexpr auto tmotor_controller_param = MotorControllerParam{
    .position = {.kp = 20, .kd = .2f, .command_max = 3, .velocity_filter_frequency_hz = 200},
    .velocity = {.kp = .2f, .ki = 20, .ki_limit = 3, .command_max = 3, .output_filter_frequency_hz = 200},
    .acceleration_limit = 1000,
    .rollover = TMotorFastLoopSensors::rad_per_count * TMotorFastLoopSensors::encoder_rollover,
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
    using FastLoopSensorPolicy = TMotorFastLoopSensors;
    using FastLoopControllerType = FOC<tmotor_foc_param, 1.f / tmotor_pwm_frequency_hz>;
    using FastLoopOutputPolicy = TMotorPWM;
    using MainLoopSensorPolicy = TMotorSensorPolicy;
    using MainLoopControllerPolicy = MotorControllerPolicy<tmotor_controller_param, 1.f / tmotor_main_loop_frequency_hz, TMotorHostCommand>;
};

}
