#include "../param/param_obot_g474.h"
#include "../st_device.h"
#include "../../motorlib/peripheral/stm32g4/spi_dma.h"
#include "../../motorlib/aksim2_encoder.h"
#include "../../motorlib/torque_sensor.h"
#include "../../motorlib/gpio.h"

using TorqueSensor = TorqueSensorBase;
using MotorEncoder = Aksim2Encoder;
//using MotorEncoder = EncoderBase;
//using OutputEncoder = Aksim2Encoder;
using OutputEncoder = EncoderBase;

extern "C" void SystemClock_Config();
void pin_config_obot_g474_motor_r0();

struct InitCode {
    InitCode() {
      SystemClock_Config();
      pin_config_obot_g474_motor_r0();
      SPI3->CR2 = (7 << SPI_CR2_DS_Pos) | SPI_CR2_FRXTH;   // 8 bit
      // ORDER DEPENDANCE SPE set last
      SPI3->CR1 = SPI_CR1_MSTR | (5 << SPI_CR1_BR_Pos) | SPI_CR1_SSI | SPI_CR1_SSM | SPI_CR1_CPOL;    // baud = clock/64
           
      // SPI1->CR2 = (15 << SPI_CR2_DS_Pos);   // 16 bit
      // // ORDER DEPENDANCE SPE set last
      // SPI1->CR1 = SPI_CR1_MSTR | (3 << SPI_CR1_BR_Pos) | SPI_CR1_SSI | SPI_CR1_SSM | SPI_CR1_SPE;    // baud = clock/16
      DMAMUX1_Channel0->CCR =  DMA_REQUEST_SPI3_TX;
      DMAMUX1_Channel1->CCR =  DMA_REQUEST_SPI3_RX;
      // DMAMUX1_Channel2->CCR =  DMA_REQUEST_SPI1_TX;
      // DMAMUX1_Channel3->CCR =  DMA_REQUEST_SPI1_RX;
    }
};

namespace config {
    const uint32_t main_loop_frequency = 10000;
    const uint32_t pwm_frequency = 20000;
    InitCode init_code;

    GPIO motor_encoder_cs(*GPIOA, 0, GPIO::OUTPUT);
    SPIDMA spi3_dma(*SPI3, motor_encoder_cs, *DMA1_Channel1, *DMA1_Channel2);
    Aksim2Encoder motor_encoder(spi3_dma);
    //EncoderBase motor_encoder;
    TorqueSensor torque_sensor;
    GPIO output_encoder_cs(*GPIOC, 3, GPIO::OUTPUT);
    // SPIDMA spi1_dma(*SPI1, output_encoder_cs, *DMA1_Channel3, *DMA1_Channel4);
    // Aksim2Encoder output_encoder(spi1_dma);
    EncoderBase output_encoder;
};

#include "../../motorlib/boards/config_obot_g474_motor.cpp"

void config_init() {
    System::api.add_api_variable("mdiag", new const APIUint8(&config::motor_encoder.diag_.word));
    System::api.add_api_variable("mdiag_raw", new const APIUint8(&config::motor_encoder.diag_raw_.word));
    System::api.add_api_variable("mcrc", new const APIUint8(&config::motor_encoder.crc_calc_));
    System::api.add_api_variable("merr", new APIUint32(&config::motor_encoder.diag_err_count_));
    System::api.add_api_variable("mwarn", new APIUint32(&config::motor_encoder.diag_warn_count_));
    System::api.add_api_variable("mcrc_cnt", new APIUint32(&config::motor_encoder.crc_err_count_));
}

void config_maintenance() {}