#include "../param/param_obot_g474_aksim.h"
#include "st_device.h"
#include "../../motorlib/peripheral/stm32g4/spi_dma.h"
#include "../../motorlib/gpio.h"
#include "../../motorlib/encoder.h"
#include "../../motorlib/temperature_sensor.h"
#include "../../motorlib/max11270.h"
#include "../../motorlib/peripheral/stm32g4/pin_config.h"
#define COMMS   COMMS_USB

using TorqueSensor = MAX11270;
using MotorEncoder = EncoderBase;
using OutputEncoder = EncoderBase;

struct InitCode {
    InitCode() {
      SPI3->CR2 = (7 << SPI_CR2_DS_Pos) | SPI_CR2_FRXTH;   // 8 bit
      // ORDER DEPENDANCE SPE set last
      SPI3->CR1 = SPI_CR1_MSTR | (6 << SPI_CR1_BR_Pos) | SPI_CR1_SSI | SPI_CR1_SSM;    // baud = clock/64
           
      DMAMUX1_Channel0->CCR =  DMA_REQUEST_SPI3_TX;
      DMAMUX1_Channel1->CCR =  DMA_REQUEST_SPI3_RX;



      GPIO_SETL(D, 2, GPIO_MODE::OUTPUT, GPIO_SPEED::VERY_HIGH, 0);   // PA0-> motor encoder cs

    }
};

namespace config {
    const uint32_t main_loop_frequency = 10000;
    const uint32_t pwm_frequency = 25000;
    InitCode init_code;

    GPIO motor_encoder_cs(*GPIOD, 2, GPIO::OUTPUT);
    SPIDMA spi3_dma(*SPI3, motor_encoder_cs, *DMA1_Channel1, *DMA1_Channel2);
    TorqueSensor torque_sensor(spi3_dma);
    EncoderBase motor_encoder;
    OutputEncoder output_encoder;
};

// #define SPI1_REINIT_CALLBACK
// void spi1_reinit_callback() {
//     SPI1->CR1=0;
//     SPI1->CR2 = (7 << SPI_CR2_DS_Pos) | SPI_CR2_FRXTH;   // 8 bit
//     // ORDER DEPENDANCE SPE set last
//     SPI1->CR1 = SPI_CR1_MSTR | (5 << SPI_CR1_BR_Pos) | SPI_CR1_SSI | SPI_CR1_SSM | SPI_CR1_CPOL;    // baud = clock/64
//     config::spi1_dma.reinit();
// }

#include "../../motorlib/boards/config_obot_g474_motor.cpp"


void config_init() {

    //config::output_encoder.spi_dma_.register_operation_ = config::drv.register_operation_;

    // System::api.add_api_variable("traw", new const APIUint32(&config::torque_sensor.raw_));
    // System::api.add_api_variable("twait_error", new const APIUint32(&config::torque_sensor.wait_error_));
    // System::api.add_api_variable("tread_error", new const APIUint32(&config::torque_sensor.read_error_));
    // System::api.add_api_variable("tread_len", new const APIUint32(&config::torque_sensor.read_len_));
    // System::api.add_api_variable("tcrc_error", new const APIUint32(&config::torque_sensor.crc_error_));
    // System::api.add_api_variable("tcrc_calc", new const APIUint8(&config::torque_sensor.crc_calc_));
    // System::api.add_api_variable("tcrc_read", new const APIUint8(&config::torque_sensor.crc_read_));
    // System::api.add_api_variable("tfull_raw", new const APIUint32(&config::torque_sensor.full_raw_));
    System::api.add_api_variable("traw", new const APICallback([](){ return u32_to_hex(config::torque_sensor.raw_value_); }));
}

void config_maintenance() {
    // round_robin_logger.log_data(TORQUE_SENSOR_CRC_INDEX, config::torque_sensor.crc_error_);
    // round_robin_logger.log_data(TORQUE_SENSOR_ERROR_INDEX, config::torque_sensor.read_error_);
}
