#include "../../motorlib/gpio.h"
#include "../../motorlib/peripheral/stm32g4/pin_config.h"
#include "../../motorlib/temperature_sensor.h"
#include "../param/param_obot_g474.h"
#include "../st_device.h"
#include "config_obot_g474_motor_aksim_types.h"

// #pragma message XSTR(MOTOR_ENCODER_BITS)
// #pragma message XSTR(OUTPUT_ENCODER_BITS)

#define END_TRIGGER_MOTOR_ENCODER

extern "C" void SystemClock_Config();
void pin_config_obot_g474_motor_r0();

// setup for R4

struct InitCode {
  InitCode() {
    SystemClock_Config();
    pin_config_obot_g474_motor_r0();
    SPI3->CR2 = (7 << SPI_CR2_DS_Pos) | SPI_CR2_FRXTH;  // 8 bit
    // ORDER DEPENDANCE SPE set last
    SPI3->CR1 = SPI_CR1_MSTR | (5 << SPI_CR1_BR_Pos) | SPI_CR1_SSI |
                SPI_CR1_SSM | SPI_CR1_CPOL;  // baud = clock/64

    SPI1->CR2 = (7 << SPI_CR2_DS_Pos) | SPI_CR2_FRXTH;  // 8 bit
    // ORDER DEPENDANCE SPE set last
    SPI1->CR1 = SPI_CR1_MSTR | (5 << SPI_CR1_BR_Pos) | SPI_CR1_SSI |
                SPI_CR1_SSM | SPI_CR1_CPOL;  // baud = clock/64
    DMAMUX1_Channel0->CCR = DMA_REQUEST_SPI3_TX;
    DMAMUX1_Channel1->CCR = DMA_REQUEST_SPI3_RX;
    DMAMUX1_Channel2->CCR = DMA_REQUEST_SPI1_TX;
    DMAMUX1_Channel3->CCR = DMA_REQUEST_SPI1_RX;

    // uart
    //   RCC->APB1ENR2 |= RCC_APB1ENR2_LPUART1EN;
    //   MASK_SET(RCC->CCIPR, RCC_CCIPR_LPUART1SEL, 1); // sysclk:
    //   LPUART1->BRR = 256*CPU_FREQUENCY_HZ/320000;
    //   LPUART1->CR3 = 2 << USART_CR3_RXFTCFG_Pos; // 4 bytes fifo threshold
    //   LPUART1->CR1 = USART_CR1_FIFOEN | USART_CR1_TE | USART_CR1_RE |
    //   USART_CR1_UE; GPIO_SETL(C, 0, GPIO_MODE::ALT_FUN, GPIO_SPEED::LOW, 8);
    //   GPIO_SETL(C, 1, GPIO_MODE::ALT_FUN, GPIO_SPEED::LOW, 8);

    // uart5
    RCC->APB1ENR1 |= RCC_APB1ENR1_UART5EN;
    MASK_SET(RCC->CCIPR, RCC_CCIPR_UART5SEL, 1);  // sysclk:
    UART5->BRR = CPU_FREQUENCY_HZ / 320000;
    UART5->CR3 = 2 << USART_CR3_RXFTCFG_Pos;  // 4 bytes fifo threshold
    UART5->CR1 = USART_CR1_FIFOEN | USART_CR1_TE | USART_CR1_RE | USART_CR1_UE;
    GPIO_SETH(C, 12, GPIO_MODE::ALT_FUN, GPIO_SPEED::LOW, 5);
    GPIO_SETL(D, 2, GPIO_MODE::ALT_FUN, GPIO_SPEED::LOW, 5);
    // gpio out
    GPIO_SETL(A, 1, GPIO::OUTPUT, GPIO_SPEED::VERY_HIGH, 0);
    // gpio in
    GPIO_SETL(A, 2, GPIO::INPUT, GPIO_SPEED::VERY_HIGH, 0);
  }
};

namespace config {
const uint32_t main_loop_frequency = 10000;
const uint32_t pwm_frequency = 25000;
InitCode init_code;

GPIO motor_encoder_cs(*GPIOD, 2, GPIO::OUTPUT);
SPIDMA spi3_dma(*SPI3, motor_encoder_cs, *DMA1_Channel1, *DMA1_Channel2);
MotorEncoderType motor_encoder(spi3_dma);

GPIO output_encoder_cs(*GPIOC, 3, GPIO::OUTPUT);
SPIDMA spi1_dma(*SPI1, output_encoder_cs, *DMA1_Channel3, *DMA1_Channel4);
OutputEncoderType output_encoder(spi1_dma);
// EncoderBase output_encoder;
// QIA128_UART torque_sensor(*LPUART1);
TorqueSensorType torque_sensor(*UART5);
};  // namespace config

#define SPI1_REINIT_CALLBACK
void spi1_reinit_callback() {
  SPI1->CR1 = 0;
  SPI1->CR2 = (7 << SPI_CR2_DS_Pos) | SPI_CR2_FRXTH;  // 8 bit
  // ORDER DEPENDANCE SPE set last
  SPI1->CR1 = SPI_CR1_MSTR | (5 << SPI_CR1_BR_Pos) | SPI_CR1_SSI | SPI_CR1_SSM |
              SPI_CR1_CPOL;  // baud = clock/64
  config::spi1_dma.reinit();
}

#include "../../motorlib/boards/config_obot_g474_motor.cpp"

namespace config {
PT1000 motor_temperature(A1_DR);
MAX31875 ambient_temperature(i2c1, 7);
};  // namespace config

void config_init() {
  config::output_encoder.spi_dma_.register_operation_ =
      config::drv.register_operation_;
  System::api.add_api_variable(
      "mdiag", new const APIUint8(&config::motor_encoder.diag_.word));
  System::api.add_api_variable(
      "mdiag_raw", new const APIUint8(&config::motor_encoder.diag_raw_.word));
  System::api.add_api_variable(
      "mcrc", new const APIUint8(&config::motor_encoder.crc_calc_));
  System::api.add_api_variable(
      "merr", new APIUint32(&config::motor_encoder.diag_err_count_));
  System::api.add_api_variable(
      "mwarn", new APIUint32(&config::motor_encoder.diag_warn_count_));
  System::api.add_api_variable(
      "mcrc_cnt", new APIUint32(&config::motor_encoder.crc_err_count_));
  System::api.add_api_variable(
      "mraw", new APIUint32(&config::motor_encoder.raw_value_));
  System::api.add_api_variable(
      "mrawh", new const APICallback([]() {
        return u32_to_hex(config::motor_encoder.raw_value_);
      }));
  System::api.add_api_variable("Tmotor", new const APICallbackFloat([]() {
                                 return config::motor_temperature.read();
                               }));
  System::api.add_api_variable("Tambient", new const APICallbackFloat([]() {
                                 return config::ambient_temperature.read();
                               }));
  System::api.add_api_variable(
      "oerr", new APIUint32(&config::output_encoder.diag_err_count_));
  System::api.add_api_variable(
      "owarn", new APIUint32(&config::output_encoder.diag_warn_count_));
  System::api.add_api_variable(
      "ocrc_cnt", new APIUint32(&config::output_encoder.crc_err_count_));
  System::api.add_api_variable("brr", new APIUint32(&LPUART1->BRR));
  System::api.add_api_variable("cr1", new APIUint32(&LPUART1->CR1));
  System::api.add_api_variable("isr", new APIUint32(&LPUART1->ISR));
  System::api.add_api_variable(
      "traw", new const APIUint32(&config::torque_sensor.raw_));
  System::api.add_api_variable(
      "twait_error", new const APIUint32(&config::torque_sensor.wait_error_));
  System::api.add_api_variable(
      "tread_error", new const APIUint32(&config::torque_sensor.read_error_));
  System::api.add_api_variable(
      "tread_len", new const APIUint32(&config::torque_sensor.read_len_));
  System::api.add_api_variable(
      "tcrc_error", new const APIUint32(&config::torque_sensor.crc_error_));
  System::api.add_api_variable(
      "tcrc_calc", new const APIUint8(&config::torque_sensor.crc_calc_));
  System::api.add_api_variable(
      "tcrc_read", new const APIUint8(&config::torque_sensor.crc_read_));
  System::api.add_api_variable(
      "tfull_raw", new const APIUint32(&config::torque_sensor.full_raw_));
}

#define MOTOR_TEMPERATURE_INDEX 0
#define BOARD_TEMPERATURE_INDEX 1
#define BUS_VOLTAGE_INDEX 2
#define BUS_CURRENT_INDEX 3
#define MOTOR_POWER_INDEX 4
#define AMBIENT_TEMPERATURE_INDEX 5

FrequencyLimiter temp_rate_motor = {10};
void config_maintenance() {
  if (temp_rate_motor.run()) {
    config::motor_temperature.read();
    round_robin_logger.log_data(MOTOR_TEMPERATURE_INDEX,
                                config::motor_temperature.get_temperature());
    if (config::motor_temperature.get_temperature() > 100) {
      config::main_loop.status_.error.motor_temperature = true;
    }
    config::ambient_temperature.read();
    round_robin_logger.log_data(AMBIENT_TEMPERATURE_INDEX,
                                config::ambient_temperature.get_temperature());
  }
  if (config::motor_encoder.crc_err_count_ > 100 ||
      config::motor_encoder.diag_err_count_ > 100 ||
      config::motor_encoder.diag_warn_count_ > pow(2, 31)) {
    config::main_loop.status_.error.motor_encoder = true;
  }
  if (config::output_encoder.crc_err_count_ > 100 ||
      config::output_encoder.diag_err_count_ > 100 ||
      config::output_encoder.diag_warn_count_ > pow(2, 31)) {
    config::main_loop.status_.error.output_encoder = true;
  }
  round_robin_logger.log_data(BOARD_TEMPERATURE_INDEX,
                              config::board_temperature.get_temperature());
  round_robin_logger.log_data(BUS_VOLTAGE_INDEX,
                              config::main_loop.status_.fast_loop.vbus);
  float bus_current = config::main_loop.status_.power /
                      config::main_loop.status_.fast_loop.vbus;
  round_robin_logger.log_data(BUS_CURRENT_INDEX, bus_current);
  round_robin_logger.log_data(MOTOR_POWER_INDEX,
                              config::main_loop.status_.fast_loop.power);
}