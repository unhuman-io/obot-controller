#pragma once
// The obot_g474 motor board, as far as the device.* init modules can describe
// it. This is the obot3 counterpart of
// motorlib/boards/pin_config_obot_g474_motor.h: a configuration starts from
// these defaults and overrides what it wires differently, see tmotor_driver.h.
#include "basic_config.h"
import device.rcc_init;
import device.gpio_init;
import device.hrtim_init;
import device.adc_init;
import device.opamp_init;
import device.tim_init;

constexpr auto motor_default_rcc_init = []() consteval {
    return RCCInit{
        .adc345 = RCCEnable::ENABLED,
        .adc12 = RCCEnable::ENABLED,
        .gpiod = RCCEnable::ENABLED,
        .gpioc = RCCEnable::ENABLED,
        .gpiob = RCCEnable::ENABLED,
        .gpioa = RCCEnable::ENABLED,
        .tim4 = RCCEnable::ENABLED,       // led
        .hrtim1 = RCCEnable::ENABLED,
        .syscfg = RCCEnable::ENABLED};    // clocks the opamps
};

// Pin helpers. Pins that are not mentioned stay analog, which is also what the
// ADC and opamp inputs need: those only get a label.
constexpr auto motor_pin_alternate = []<std::size_t N>(auto& pin, GPIOAF af, const char (&function)[N]) consteval {
    pin.mode = GPIOMode::ALTERNATE;
    pin.speed = GPIOSpeed::VERY_HIGH;
    pin.af = af;
    pin.function = function;
};
constexpr auto motor_pin_output = []<std::size_t N>(auto& pin, GPIOSet level, const char (&function)[N]) consteval {
    pin.mode = GPIOMode::OUTPUT;
    pin.speed = GPIOSpeed::VERY_HIGH;
    pin.bsrr = level;
    pin.function = function;
};

constexpr auto motor_default_gpio_init = []() consteval {
    GPIOInit init = get_default_gpio_init();    // swd on PA13, PA14 and PB3

    // quadrature encoder on TIM2, pulled down against noise
    motor_pin_alternate(init.a[0], GPIOAF::AF1, "tim2 qepa");
    motor_pin_alternate(init.a[1], GPIOAF::AF1, "tim2 qepb");
    motor_pin_alternate(init.a[2], GPIOAF::AF1, "tim2 qepi");
    init.a[0].pullupd = GPIOPullUpD::PULL_DOWN;
    init.a[1].pullupd = GPIOPullUpD::PULL_DOWN;
    init.a[2].pullupd = GPIOPullUpD::PULL_DOWN;

    // drv8323s gate driver
    motor_pin_output(init.a[4], GPIOSet::SET, "spi1 cs drv8323s");
    motor_pin_alternate(init.a[5], GPIOAF::AF5, "spi1 clk");
    motor_pin_alternate(init.a[6], GPIOAF::AF5, "spi1 hido");
    motor_pin_alternate(init.a[7], GPIOAF::AF5, "spi1 hodi");
    init.a[6].pullupd = GPIOPullUpD::PULL_UP;
    motor_pin_output(init.c[3], GPIOSet::NO_SET, "spi1 cs2");
    init.c[3].speed = GPIOSpeed::MEDIUM;
    motor_pin_output(init.c[13], GPIOSet::RESET, "drv enable");   // low: the gate driver is off
    init.c[13].speed = GPIOSpeed::LOW;
    init.c[14].mode = GPIOMode::INPUT;
    init.c[14].pullupd = GPIOPullUpD::PULL_UP;
    init.c[14].function = "drv fault";

    // motor pwm, phases a, b, c
    motor_pin_alternate(init.b[14], GPIOAF::AF13, "hrtim1 chd1");
    motor_pin_alternate(init.b[15], GPIOAF::AF13, "hrtim1 chd2");
    motor_pin_alternate(init.c[6], GPIOAF::AF13, "hrtim1 chf1");
    motor_pin_alternate(init.c[7], GPIOAF::AF13, "hrtim1 chf2");
    motor_pin_alternate(init.c[8], GPIOAF::AF3, "hrtim1 che1");
    motor_pin_alternate(init.c[9], GPIOAF::AF3, "hrtim1 che2");

    // usb
    init.a[10].mode = GPIOMode::INPUT;
    init.a[10].function = "usb exti";
    init.a[11].function = "usb dm";
    init.a[12].function = "usb dp";

    // led
    motor_pin_alternate(init.b[6], GPIOAF::AF2, "tim4 ch1 led r");
    motor_pin_alternate(init.b[7], GPIOAF::AF2, "tim4 ch2 led g");
    motor_pin_alternate(init.b[8], GPIOAF::AF2, "tim4 ch3 led b");

    // motor encoder
    motor_pin_alternate(init.c[10], GPIOAF::AF6, "spi3 clk");
    motor_pin_alternate(init.c[11], GPIOAF::AF6, "spi3 hido");
    motor_pin_alternate(init.c[12], GPIOAF::AF6, "spi3 hodi");
    motor_pin_output(init.d[2], GPIOSet::SET, "spi3 cs");

    // i2c1, open drain. PA15 is taken from the jtag.
    motor_pin_alternate(init.a[15], GPIOAF::AF4, "i2c1 scl");
    motor_pin_alternate(init.b[9], GPIOAF::AF4, "i2c1 sda");
    for (auto* pin : {&init.a[15], &init.b[9]}) {
        pin->speed = GPIOSpeed::LOW;
        pin->otype = GPIOOType::OPEN_DRAIN;
        pin->pullupd = GPIOPullUpD::PULL_UP;
    }

    // analog
    init.a[3].function = "vbus, opamp1 vinp1";
    init.b[13].function = "ia, opamp3 vinp1";
    init.b[12].function = "ib, opamp6 vinp0";
    init.b[11].function = "ic, opamp4 vinp2";
    init.b[0].function = "tsense, opamp2 vinp2";
    init.b[2].function = "tsense2, adc2 in12";
    init.c[0].function = "a1, adc1 in6";
    init.c[1].function = "a2, adc1 in7";
    init.c[2].function = "a3, adc2 in8";
    init.c[5].function = "v5v, adc2 in11";
    init.b[1].function = "i5v, adc3 in1";
    init.a[8].function = "i48v, adc5 in1";
    return init;
};

// All of the board's opamps are followers on a pin with the internal output.
// They stay on the factory trim: opamp_calibrate_async drives the VOUT pin,
// and those are in use here (PB1 is i5v; PB11 and PB12 are each other's inputs).
constexpr auto motor_follower = [](OPAMPInput input) consteval {
    return OPAMPAmplifierInit{
        .enable = OPAMPEnable::ENABLED,
        .input = input,
        .mode = OPAMPMode::FOLLOWER,
        .high_speed = OPAMPEnable::ENABLED,
        .internal_output = OPAMPEnable::ENABLED,
    };
};
constexpr auto motor_default_opamp_init = []() consteval {
    return OPAMPInit{
        .opamp1 = motor_follower(OPAMPInput::VINP1),   // PA3  vbus     -> ADC1 IN13
        .opamp2 = motor_follower(OPAMPInput::VINP2),   // PB0  tsense   -> ADC2 IN16
        .opamp3 = motor_follower(OPAMPInput::VINP1),   // PB13 ia       -> ADC3 IN13
        .opamp4 = motor_follower(OPAMPInput::VINP2),   // PB11 ic       -> ADC5 IN5
        .opamp6 = motor_follower(OPAMPInput::VINP0),   // PB12 ib       -> ADC4 IN17
    };
};

// HRTIM. Timers D, F and E are the pwm of phases a, b and c: center aligned,
// output 1 goes active on cmp1 on the way up and inactive on the way down, so
// the crest is the center of the on time and a lower cmp1 is a higher duty.
// Output 2 is the complement, with dead time. The roll-over is at the valley
// only, so a new cmp1 takes effect once per pwm period. The outputs are not
// enabled here: that is up to the controller.
// Timer F also places the ADC triggers:
//   trigger 1 on cmp4, just after the valley = the center of the off time,
//             where the low side shunts carry the phase current
//   trigger 3 on cmp3, just before the crest, for the zero current readings
// The master timer is the main loop's time base: its cmp1 raises the
// interrupt, and its roll-over is the update event of the ADC triggers.
constexpr uint16_t motor_adc_trigger_offset = 64;   // hrtim ticks, the smallest compare value that works
constexpr uint16_t motor_deadtime_ticks = 68;       // 50 ns, in hrtim clock / 4 * 32 ticks
constexpr uint8_t motor_master_prescaler = 7;       // 42.5 MHz: 10 kHz is 4250 counts
constexpr auto motor_default_hrtim_init = [](uint32_t pwm_frequency_hz, uint32_t main_loop_frequency_hz) consteval {
    const uint16_t pwm_period = hrtim_period(pwm_frequency_hz, 0, HRTIMMode::CENTER_ALIGNED);
    const HRTIMTimerInit pwm = {
        .enable = HRTIMEnable::ENABLED,
        .mode = HRTIMMode::CENTER_ALIGNED,
        .rollover = HRTIMRollover::VALLEY,
        .continuous = HRTIMEnable::ENABLED,
        .preload = HRTIMEnable::ENABLED,
        .update_on_reset = HRTIMEnable::ENABLED,
        .period = pwm_period,
        .cmp1 = static_cast<uint16_t>(pwm_period / 2),   // 50 % on all phases: 0 V
        .out1_set = HRTIMEvent::CMP1,
        .deadtime = HRTIMEnable::ENABLED,
        .deadtime_rising = motor_deadtime_ticks,
        .deadtime_falling = motor_deadtime_ticks,
    };
    HRTIMInit init{};
    init.d = pwm;
    init.e = pwm;
    init.f = pwm;
    init.f.adc_rollover = HRTIMRollover::CREST;
    init.f.cmp3 = pwm_period - motor_adc_trigger_offset - 1;
    init.f.cmp4 = motor_adc_trigger_offset;
    init.adc_trigger1 = HRTIMADCTrigger::TIMF_CMP4;
    init.adc_trigger3 = HRTIMADCTrigger::TIMF_CMP3;
    init.master = {
        .enable = HRTIMEnable::ENABLED,
        .prescaler = motor_master_prescaler,
        .continuous = HRTIMEnable::ENABLED,
        .preload = HRTIMEnable::ENABLED,
        .update_on_period = HRTIMEnable::ENABLED,
        .cmp1_interrupt = HRTIMEnable::ENABLED,           // the main loop
        .period = hrtim_period(main_loop_frequency_hz, motor_master_prescaler),
        .cmp1 = 400,
        .sync_out = HRTIMSyncOut::POSITIVE_PULSE,
        .sync_source = HRTIMSyncSource::MASTER_START,
    };
    return init;
};

// ADC. The current sense converters take two sets of readings per pwm period:
//   injected, HRTIM trigger 1: the phase currents. The opamp channel is
//     converted twice and the first result is used (errata). The end of the
//     ADC5 sequence is the fast loop interrupt.
//   regular, HRTIM trigger 3: the same channel with the low side off, for the
//     zero current offset
// ADC1 regular follows the bus voltage. The slow channels are injected
// sequences that software starts: adc_start_injected<1>() and <2>().
//
//         regular       injected
//   ADC1  IN13 vbus     IN16 temperature, IN18 vrefint, IN6 a1, IN7 a2
//   ADC2  -             IN8 a3, IN16 tsense, IN12 tsense2, IN11 v5v
//   ADC3  IN13 ia       IN13 ia, IN13, IN1 i5v
//   ADC4  IN17 ib       IN17 ib, IN17
//   ADC5  IN5 ic        IN5 ic, IN5, IN1 i48v
constexpr auto motor_default_adc_init = []() consteval {
    constexpr auto converter = []() consteval {
        return ADCConverterInit{
            .enable = ADCEnable::ENABLED,
            .clock = ADCClock::HCLK_DIV4,                 // 42.5 MHz
            .vrefint = ADCEnable::ENABLED,
            .overwrite = ADCEnable::ENABLED,              // DR always holds the latest conversion
        };
    };
    constexpr auto slow = [](ADCConverterInit& c, std::initializer_list<uint8_t> channels) consteval {
        for (uint8_t channel : channels) {
            c.injected_sequence[c.injected_sequence_length++] = channel;
            c.sample_time[channel] = ADCSampleTime::CYCLES_247_5;   // 5.8 us, the internal channels need 5
        }
    };
    constexpr auto current_sense = [converter](uint8_t opamp_channel, int extra_channel = -1) consteval {
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
        if (extra_channel >= 0) {
            c.injected_sequence[c.injected_sequence_length++] = extra_channel;
            c.sample_time[extra_channel] = ADCSampleTime::CYCLES_12_5;
        }
        return c;
    };

    ADCInit init{};
    init.adc1 = converter();
    init.adc1.temperature_sensor = ADCEnable::ENABLED;
    init.adc1.trigger = ADCTrigger::HRTIM_TRG1;
    init.adc1.trigger_edge = ADCTriggerEdge::RISING;
    init.adc1.sequence[0] = 13;
    init.adc1.sample_time[13] = ADCSampleTime::CYCLES_12_5;
    slow(init.adc1, {16, 18, 6, 7});
    init.adc2 = converter();
    init.adc2.sequence[0] = 8;                  // regular group unused
    slow(init.adc2, {8, 16, 12, 11});
    init.adc3 = current_sense(13, 1);
    init.adc4 = current_sense(17);
    init.adc5 = current_sense(5, 1);
    init.adc5.injected_end_of_sequence_interrupt = ADCEnable::ENABLED;
    return init;
};

// For the handlers of the two loops
inline void motor_fast_loop_irq_clear() { adc_injected_end_of_sequence_clear<5>(); }
inline void motor_main_loop_irq_clear() { hrtim_master_cmp1_interrupt_clear(); }

// The readings by name
inline uint32_t motor_ia() { return adc_read_injected<3>(0); }
inline uint32_t motor_ib() { return adc_read_injected<4>(0); }
inline uint32_t motor_ic() { return adc_read_injected<5>(0); }
inline uint32_t motor_vbus() { return adc_read<1>(); }
inline uint32_t motor_temperature() { return adc_read_injected<1>(0); }
inline uint32_t motor_vrefint() { return adc_read_injected<1>(1); }

// The calibrations are coroutines on a scheduler local to this function, so
// the five converters run in parallel
inline void motor_adc_calibrate() {
    CycleScheduler sched;
    auto adc1 = adc_calibrate_async<1>(sched);
    auto adc2 = adc_calibrate_async<2>(sched);
    auto adc3 = adc_calibrate_async<3>(sched);
    auto adc4 = adc_calibrate_async<4>(sched);
    auto adc5 = adc_calibrate_async<5>(sched);
    while (!adc1.is_done() || !adc2.is_done() || !adc3.is_done() || !adc4.is_done() || !adc5.is_done()) {
        sched.poll();
    }
}

// TIM4 is the led pwm, 0 (off) to 65535 on each color
constexpr auto motor_default_tim_init = []() consteval {
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
};
inline void motor_led(uint16_t red, uint16_t green, uint16_t blue) {
    tim_compare<4, 1>(red);
    tim_compare<4, 2>(green);
    tim_compare<4, 3>(blue);
}
