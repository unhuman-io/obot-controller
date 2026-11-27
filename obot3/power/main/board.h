#pragma once
// The power board's main controller, STM32G564RET6: what is on which pin, and
// the clocks. From obot-cad/electronics/controller/design.py (G5PORTS), the
// alternate functions and ADC channels from the datasheet DS14920 rev 1.
//
//   PA2  USART2 TX  -> radio module (LINK_G2W)      PA3  USART2 RX  <- radio module (LINK_W2G)
//   PB12 FDCAN1 RX, PB13 FDCAN1 TX, PB14 CAN_STB (high = transceiver asleep; pulled up on the board)
//   PG2  I2C2 SCL, PG3 I2C2 SDA: three INA228 power monitors; PD12 their ALERT lines (open drain, not used yet)
//   PA0  ADC1 IN1: motor bus voltage, 2.2 M / 100 k
//   PC0, PC1 the computer outputs' current monitors (ADC IN6, IN7): not read, the power monitors measure the same
//   PC6  BUS_EN -> the bus eFuse's enable through 1k        PC7  BUS_EN_SNS: that node read back
//   PB5  PGD: bus eFuse power good                          PA8  OUT_PG: computer output eFuses good (both)
//   PB7  OUT12_ON -> the 12 V output's eFuse                PC13 LED_M, yellow, lit when low
//   PB10, PB11, PB15 battery data (USART3 + one line): the pack's protocol is not known yet; left alone
#include <cstdint>
import device.gpio_init;
import device.rcc_init;
import device.can_init;
import device.irq_init;
import device.adc_init;
import power.g5_uart;
import power.g5_i2c;
import power.ina228;

// 280 MHz from the 40 MHz crystal (40 / 5 = 8 MHz, x 70 = 560 MHz VCO, / 2), as
// the other stm32g564 firmware: the device library is built for that. This
// controller measures and switches and would do with a tenth of it. FDCAN1
// gets 80 MHz (PLL1 Q); the UART and the I2C run from APB1 at 70 MHz.
inline constexpr uint32_t power_cpu_hz = 280'000'000;
static_assert(static_cast<uint32_t>(CPU_FREQUENCY_HZ) == power_cpu_hz);
inline constexpr uint32_t power_apb1_hz = power_cpu_hz / 4;

inline constexpr auto power_rcc_clock_init = RCCClockInit{
    .cpu_frequency_hz = power_cpu_hz,
    .pll_source = RCCPLLSource::HSE,
    .hse_frequency_hz = 40'000'000,
    .pll1_q_frequency_hz = 80'000'000,          // fdcan kernel clock: 1 Mbps = 80 tq, 8 Mbps = 10 tq
    .fdcan_clock = RCCFDCANClock::PLL1Q,
    .adc_clock = RCCADCClock::BUS,
    .adc_clock_div = RCCADCDiv::DIV16,          // 17.5 MHz
    .apb1_div_log2 = 2};

inline constexpr auto power_rcc_init = RCCInit{.adc12 = RCCEnable::ENABLED,
                                               .gpiog = RCCEnable::ENABLED,
                                               .gpiod = RCCEnable::ENABLED,
                                               .gpioc = RCCEnable::ENABLED,
                                               .gpiob = RCCEnable::ENABLED,
                                               .gpioa = RCCEnable::ENABLED,
                                               .i2c2 = RCCEnable::ENABLED,
                                               .usart2 = RCCEnable::ENABLED,
                                               .fdcan = RCCEnable::ENABLED};

inline constexpr auto power_gpio_init = []() consteval {
    GPIOInit init = get_default_gpio_init();
    init.a[2] = {.net = "LINK_G2W", .function = "usart2 tx", .mode = GPIOMode::ALTERNATE, .af = GPIOAF::AF7};
    init.a[3] = {.net = "LINK_W2G", .function = "usart2 rx", .mode = GPIOMode::ALTERNATE, .pullupd = GPIOPullUpD::PULL_UP, .af = GPIOAF::AF7};
    init.a[8] = {.net = "OUT_PG", .mode = GPIOMode::INPUT};
    init.b[5] = {.net = "PGD", .mode = GPIOMode::INPUT};
    init.b[7] = {.net = "OUT12_ON", .mode = GPIOMode::OUTPUT, .bsrr = GPIOSet::RESET};
    init.b[12] = {.net = "CAN_RXD", .function = "fdcan1 rx", .mode = GPIOMode::ALTERNATE, .af = GPIOAF::AF13};
    init.b[13] = {.net = "CAN_TXD", .function = "fdcan1 tx", .mode = GPIOMode::ALTERNATE, .speed = GPIOSpeed::VERY_HIGH, .af = GPIOAF::AF13};
    init.b[14] = {.net = "CAN_STB", .mode = GPIOMode::OUTPUT, .bsrr = GPIOSet::RESET};       // transceiver awake
    init.c[6] = {.net = "BUS_EN", .mode = GPIOMode::OUTPUT, .bsrr = GPIOSet::RESET};
    init.c[7] = {.net = "BUS_EN_SNS", .mode = GPIOMode::INPUT};
    init.c[13] = {.net = "LED_M", .mode = GPIOMode::OUTPUT, .bsrr = GPIOSet::SET};          // off
    init.d[12] = {.net = "PM_ALERT", .mode = GPIOMode::INPUT};
    init.g[2] = {.net = "PM_SCL", .function = "i2c2 scl", .mode = GPIOMode::ALTERNATE, .otype = GPIOOType::OPEN_DRAIN, .af = GPIOAF::AF4};
    init.g[3] = {.net = "PM_SDA", .function = "i2c2 sda", .mode = GPIOMode::ALTERNATE, .otype = GPIOOType::OPEN_DRAIN, .af = GPIOAF::AF4};
    return init;
}();

inline void power_bus_enable(bool on) { on ? gpio_set(gpio_port<'C'>(), 6) : gpio_clear(gpio_port<'C'>(), 6); }
inline void power_output12(bool on) { on ? gpio_set(gpio_port<'B'>(), 7) : gpio_clear(gpio_port<'B'>(), 7); }
inline void power_led(bool lit) { lit ? gpio_clear(gpio_port<'C'>(), 13) : gpio_set(gpio_port<'C'>(), 13); }
inline bool power_bus_good() { return gpio_read(gpio_port<'B'>(), 5); }
inline bool power_bus_enable_sensed() { return gpio_read(gpio_port<'C'>(), 7); }
inline bool power_outputs_good() { return gpio_read(gpio_port<'A'>(), 8); }

inline constexpr auto power_can_init = CANInit{.kernel_clock_hz = power_rcc_clock_init.pll1_q_frequency_hz, .standard_filter_count = 2};
inline constexpr auto power_link_init = UARTInit{.instance = 2, .kernel_clock_hz = power_apb1_hz, .baud = 115'200, .receive_interrupt = true};
inline constexpr auto power_i2c_init = I2CInit{.instance = 2, .kernel_clock_hz = power_apb1_hz, .speed_hz = 400'000,
                                              .timeout_cycles = power_cpu_hz / 1000};       // 1 ms a byte: 40 bit times

// The motor bus voltage: ADC1 converts channel 1 again and again, the newest
// result is read when it is wanted. The divider is megohms, so the sampling
// time is the longest there is (the 100 nF at the pin does the rest).
inline constexpr auto power_adc_init = []() consteval {
    ADCInit init{};
    init.adc1.enable = ADCEnable::ENABLED;
    init.adc1.continuous = ADCEnable::ENABLED;
    init.adc1.overwrite = ADCEnable::ENABLED;
    init.adc1.sequence[0] = 1;
    init.adc1.sample_time[1] = ADCSampleTime::CYCLES_848_5;
    return init;
}();
// 12 bits of 3.0 V behind 2.2 M / 100 k, in V / 100
inline constexpr uint16_t power_bus_cv(uint32_t code) { return static_cast<uint16_t>(code * 300 * 23 / 4095); }

// The power monitors on I2C2: battery (3 mOhm, the shunt of the bus eFuse),
// 12 V output (2 mOhm), 5 V output (5 mOhm)
inline constexpr ina228::Monitor power_monitors[] = {{0x40, 3000, false}, {0x41, 2000, true}, {0x44, 5000, true}};

// On the CAN bus this is an obot node like the motor controllers: it answers
// the enum request and sends its status, ten a second without being asked.
inline constexpr uint32_t power_can_address = 0x70;
