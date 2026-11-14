#pragma once
#include "../config1_default.h"
import device.can_init;
import device.adc_init;
import device.dac_init;
import device.opamp_init;
import device.irq_init;
import device.tim_init;

// Interference test bench. All six opamps run as followers with the internal
// output, each read by its own ADC on the same HRTIM trigger:
//   OPAMP1  DAC3 channel 1   ADC1 IN13      OPAMP4  DAC4 channel 1   ADC5 IN5
//   OPAMP2  PB0, pulled up   ADC2 IN16      OPAMP5  DAC4 channel 2   ADC5 IN3
//   OPAMP3  DAC3 channel 2   ADC3 IN13      OPAMP6  DAC3 channel 1   ADC4 IN17
// OPAMP2 has no DAC input on the 64 pin package. ADC5 converts one of its two
// opamps at a time.
constexpr auto config1_dac_channel = DACChannelInit{
    .enable = DACEnable::ENABLED,
    .output = DACOutput::INTERNAL,
    .value = dac_range / 2,             // VREF+ / 2
};
constexpr auto config1_dac_init = DACInit{
    .dac3 = {.ch1 = config1_dac_channel, .ch2 = config1_dac_channel},
    .dac4 = {.ch1 = config1_dac_channel, .ch2 = config1_dac_channel},
};

// The output pins (VOUT) stay free, apart from PB1 during the OPAMP3 calibration
// and PA8, the OPAMP5 one, which the interference source below drives
constexpr auto config1_follower = [](OPAMPInput input) consteval {
    return OPAMPAmplifierInit{
        .enable = OPAMPEnable::ENABLED,
        .input = input,
        .mode = OPAMPMode::FOLLOWER,
        .high_speed = OPAMPEnable::ENABLED,
        .internal_output = OPAMPEnable::ENABLED,
    };
};
constexpr auto config1_opamp_init = OPAMPInit{
    .opamp1 = config1_follower(OPAMPInput::VINP3),
    .opamp2 = config1_follower(OPAMPInput::VINP2),
    .opamp3 = config1_follower(OPAMPInput::VINP3),
    .opamp4 = config1_follower(OPAMPInput::VINP3),
    .opamp5 = config1_follower(OPAMPInput::VINP3),
    .opamp6 = config1_follower(OPAMPInput::VINP3),
};

// Every converter takes one conversion per HRTIM ADC trigger 1, which
// config1_default_hrtim_init puts on timer F cmp3. ADC3 raises the interrupt
// that runs the fast loop.
constexpr auto config1_adc_init = []() consteval {
    constexpr auto converter = [](uint8_t channel) consteval {
        ADCConverterInit c = {
            .enable = ADCEnable::ENABLED,
            .clock = ADCClock::HCLK_DIV4,                 // 42.5 MHz
            .trigger = ADCTrigger::HRTIM_TRG1,
            .trigger_edge = ADCTriggerEdge::RISING,
            .overwrite = ADCEnable::ENABLED,              // DR always holds the latest conversion
        };
        c.sequence[0] = channel;
        c.sample_time[channel] = ADCSampleTime::CYCLES_12_5;   // 294 ns, the opamp output needs 200 ns
        return c;
    };
    ADCInit init{};
    init.adc1 = converter(13);
    init.adc2 = converter(16);
    init.adc3 = converter(13);
    init.adc3.end_of_conversion_interrupt = ADCEnable::ENABLED;
    init.adc4 = converter(17);
    init.adc5 = converter(5);
    return init;
}();

// Interference source 1: HRTIM timer A output 1 on PA8, which a jumper
// connects to PB1, the OPAMP3 output pin. PB1 itself is not driven. Timer A
// runs one high resolution tick (184 ps) longer than the sample period, so
// its edges land one tick later in each conversion and the pattern repeats
// every 5441 samples. The output rises at the roll-over and falls a third of
// a period later: the conversions see only the falling edge for a third of
// the pattern, then only the rising edge.
// The sync pulse (SCOUT) from the master timer is set up as well but not
// routed to a pin: PB1 with alternate function 13 would bring it out, and
// drive against PA8 through the jumper.
constexpr uint16_t config1_sample_ticks = 170 * 32;
constexpr auto config1_hrtim_init = []() consteval {
    HRTIMInit init = config1_default_hrtim_init();
    init.a = {
        .enable = HRTIMEnable::ENABLED,
        .continuous = HRTIMEnable::ENABLED,
        .period = config1_sample_ticks + 1,
        .cmp1 = config1_sample_ticks / 3,
        .out1_set = HRTIMEvent::PERIOD,
        .out1_reset = HRTIMEvent::CMP1,
        .out1 = HRTIMEnable::ENABLED,
    };
    init.master = {
        .enable = HRTIMEnable::ENABLED,
        .continuous = HRTIMEnable::ENABLED,
        .period = 171 * 32,
        .cmp1 = 171 * 16,
        .sync_out = HRTIMSyncOut::POSITIVE_PULSE,
        .sync_source = HRTIMSyncSource::MASTER_CMP1,
    };
    return init;
}();

// Interference source 2: TIM3 with the same 171 clock period, as the time
// base for DMA writes that toggle any other pin. The DMA is set up from the
// debugger.
constexpr auto config1_tim_init = TIMInit{
    .tim3 = {
        .enable = TIMEnable::ENABLED,
        .period = 171,
    },
};

// The fast loop reports one converter, picked from the debugger
volatile int adc_select = 3;
volatile int adc_reading = 0;
struct SpecialEncoder {
    int update() {
        int reading = adc_read<3>();    // always read: it clears the interrupt request
        switch (adc_select) {
            case 1: reading = adc_read<1>(); break;
            case 2: reading = adc_read<2>(); break;
            case 4: reading = adc_read<4>(); break;
            case 5: reading = adc_read<5>(); break;
        }
        adc_reading = reading;
        return reading;
    }
};

struct Config1_1SensorStatus {
    int adc_reading;
};

struct Config1_1SensorPolicy : Config1SensorPolicy {
    Config1_1SensorStatus update() {
        Config1SensorPolicy::update();
        count += 2;
        adc_reading = ::adc_reading;    // the fast loop's value: a second read of the ADC from here disturbs its conversions
        return {adc_reading};
    }
    volatile int adc_reading = 0;
};

struct Config1 : DefaultConfig1 {
    using FastLoopSensorPolicy = SpecialEncoder;
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
    init.a[8].af = GPIOAF::AF13;             // HRTIM1 CHA1, switched in after the opamp calibration
    init.b[0].pullupd = GPIOPullUpD::PULL_UP;   // the OPAMP2 input
    return init;
}();

constexpr auto config1_rcc_init = []() consteval {
    return RCCInit{
        .dmamux1 = RCCEnable::ENABLED,
        .dma1 = RCCEnable::ENABLED,
        .dac4 = RCCEnable::ENABLED,
        .dac3 = RCCEnable::ENABLED,
        .adc345 = RCCEnable::ENABLED,
        .adc12 = RCCEnable::ENABLED,
        .gpioe = RCCEnable::ENABLED,
        .gpiod = RCCEnable::ENABLED,
        .gpioc = RCCEnable::ENABLED,
        .gpiob = RCCEnable::ENABLED,
        .gpioa = RCCEnable::ENABLED,
        .fdcan = RCCEnable::ENABLED,
        .tim3 = RCCEnable::ENABLED,
        .hrtim1 = RCCEnable::ENABLED,
        .syscfg = RCCEnable::ENABLED};    // clocks the opamps
}();

// no spare pin to toggle on this board
inline void config1_toggle() {}

// The fast loop runs in the ADC3 interrupt, once per conversion. Its read of
// the result (SpecialEncoder) clears the request.
void fast_loop_isr();
constexpr auto config1_irq_init = []() consteval {
    IRQInit init{};
    init.irq[ADC3_IRQn] = {.handler = fast_loop_isr};
    return init;
}();

// Nothing to poll. The main loop waits here for the next ADC3 interrupt, so it
// runs once per conversion and in step with it: a free running main loop
// beats with the sample clock and shows up as lines in the ADC spectrum.
inline void config1_poll() {
    __WFI();
}

constexpr auto config1_rcc_clock_init = RCCClockInit{.fdcan_clock = RCCFDCANClock::PCLK1};   // 170 MHz

constexpr auto config1_can_init = CANInit{.kernel_clock_hz = config1_rcc_clock_init.cpu_frequency_hz};

// OPAMP3 and ADC3 are calibrated; the others only serve the interference test.
// The calibrations are coroutines on a scheduler local to this function, so
// they run in parallel. More init tasks can join them here.
inline void config1_device_calibrate() {
    CycleScheduler sched;
    auto adc_task = adc_calibrate_async<3>(sched);
    auto opamp_task = opamp_calibrate_async<3>(sched);
    while (!adc_task.is_done() || !opamp_task.is_done()) {
        sched.poll();
    }
}

inline void config1_device_init() {
    device_dac_config<config1_dac_init>();
    device_opamp_config<config1_opamp_init>();
    device_adc_config<config1_adc_init>();
    config1_device_calibrate();
    device_tim_config<config1_tim_init>();
    gpio_mode(gpio_port<'A'>(), 8, GPIOMode::ALTERNATE);   // PA8 drives only after the calibration, which puts the opamp output on PB1 and the jumper
}
