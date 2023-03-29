#include "../../motorlib/gpio.h"
#include "../../motorlib/peripheral/stm32g4/spi_debug.h"
#include "../../motorlib/peripheral/stm32g4/spi_dma.h"
#include "../param/param_obot_g474_motor40.h"
#include "../st_device.h"
#include "config_obot_g474_motor_40_osa14_types.h"


extern "C" void SystemClock_Config();
void pin_config_obot_g474_motor_40();

struct InitCode {
  InitCode() {
    SystemClock_Config();
    pin_config_obot_g474_motor_40();

    // SPI3 PZ
    DMAMUX1_Channel0->CCR = DMA_REQUEST_SPI3_TX;
    DMAMUX1_Channel1->CCR = DMA_REQUEST_SPI3_RX;
    SPI3->CR1 = SPI_CR1_MSTR | (3 << SPI_CR1_BR_Pos) | SPI_CR1_SSI |
                SPI_CR1_SSM;  // baud = clock/16 spi mode 0
    SPI3->CR2 = (7 << SPI_CR2_DS_Pos) | SPI_CR2_FRXTH;  // 8 bit
  }
};

namespace config {
const uint32_t main_loop_frequency = 10000;
const uint32_t pwm_frequency = 50000;
InitCode init_code;

GPIO motor_encoder_cs = {*GPIOD, 2, GPIO::OUTPUT};
SPIDMA spi_dma = {*SPI3, motor_encoder_cs, *DMA1_Channel1, *DMA1_Channel2};
ICPZ motor_encoder(spi_dma, ICPZ::PZ03S);
SPIDebug spi_debug(spi_dma);

GPIO torque_sensor_cs(*GPIOA, 4, GPIO::OUTPUT);
SPIDMA spi_dma2(*SPI1, torque_sensor_cs, *DMA1_Channel3, *DMA1_Channel4, 50,
                50);
ADS1235 torque_sensor(spi_dma2);

EncoderBase output_encoder;
};  // namespace config

#include "../../motorlib/boards/config_obot_g474_motor_40.cpp"

void config_init() {
  System::api.add_api_variable(
      "spi",
      new APICallback([]() { return config::spi_debug.read(); },
                      [](std::string s) { config::spi_debug.write(s); }));

  // System::api.add_api_variable("torque1", new const
  // APIFloat(&config::torque_sensor.torque1_));
  // System::api.add_api_variable("torque2", new const
  // APIFloat(&config::torque_sensor.torque2_));
  // System::api.add_api_variable("torque_decimation", new
  // APIUint16(&config::torque_sensor.decimation_));
}

void config_maintenance() {}