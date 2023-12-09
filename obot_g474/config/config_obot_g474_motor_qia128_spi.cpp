#include "../param/param_obot_g474.h"
#include "st_device.h"
#include "../../motorlib/peripheral/stm32g4/spi_dma.h"
#include "../../motorlib/aksim2_encoder.h"
#include "../../motorlib/torque_sensor.h"
#include "../../motorlib/gpio.h"
#include "../../motorlib/temperature_sensor.h"
#include "../../motorlib/qia128.h"
#include "../../motorlib/peripheral/stm32g4/pin_config.h"
#include "../../motorlib/sensor_multiplex.h"

//using TorqueSensor = QIA128; 
//using TorqueSensor = TorqueSensorBase;
using MotorEncoder = Aksim2Encoder<18>;
//using MotorEncoder = EncoderBase;
//using OutputEncoder = Aksim2Encoder<18>;
//using OutputEncoder = EncoderBase;

using TorqueSensor = TorqueSensorMultiplex<QIA128, Aksim2Encoder<18>>;
using OutputEncoder = TorqueSensor::SecondarySensor;

struct InitCode {
    InitCode() {
      SPI3->CR2 = (7 << SPI_CR2_DS_Pos) | SPI_CR2_FRXTH;   // 8 bit
      // ORDER DEPENDANCE SPE set last
      SPI3->CR1 = SPI_CR1_MSTR | (5 << SPI_CR1_BR_Pos) | SPI_CR1_SSI | SPI_CR1_SSM | SPI_CR1_CPOL;    // baud = clock/64
           
      SPI1->CR2 = (7 << SPI_CR2_DS_Pos) | SPI_CR2_FRXTH;   // 8 bit
      // ORDER DEPENDANCE SPE set last
      SPI1->CR1 = SPI_CR1_MSTR | (5 << SPI_CR1_BR_Pos) | SPI_CR1_SSI | SPI_CR1_SSM | SPI_CR1_CPOL;    // baud = clock/64
      SPI1->CR1 = SPI_CR1_MSTR | (6 << SPI_CR1_BR_Pos) | SPI_CR1_SSI | SPI_CR1_SSM;    // baud = clock/128 qia128
      GPIO_SETH(B, 10, GPIO::OUTPUT, GPIO_SPEED::MEDIUM, 0);
      DMAMUX1_Channel0->CCR =  DMA_REQUEST_SPI3_TX;
      DMAMUX1_Channel1->CCR =  DMA_REQUEST_SPI3_RX;
      DMAMUX1_Channel2->CCR =  DMA_REQUEST_SPI1_TX;
      DMAMUX1_Channel3->CCR =  DMA_REQUEST_SPI1_RX;
    }
};

namespace config {
    const uint32_t main_loop_frequency = 10000;
    const uint32_t pwm_frequency = 20000;
    InitCode init_code;

    GPIO motor_encoder_cs(*GPIOD, 2, GPIO::OUTPUT);
    SPIDMA spi3_dma(*SPI3, motor_encoder_cs, *DMA1_Channel1, *DMA1_Channel2);
    MotorEncoder motor_encoder(spi3_dma);
    //EncoderBase motor_encoder;
    
    GPIO output_encoder_cs(*GPIOC, 3, GPIO::OUTPUT);
    GPIO torque_sensor_cs(*GPIOB, 10, GPIO::OUTPUT);
    // SPIDMA spi1_dma(*SPI1, output_encoder_cs, *DMA1_Channel3, *DMA1_Channel4);
    // SPIDMA spi1_dma2(*SPI1, torque_sensor_cs, *DMA1_Channel3, *DMA1_Channel4);
    SPIDMA spi1_dma(*SPI1, torque_sensor_cs, *DMA1_Channel3, *DMA1_Channel4);
    SPIDMA spi1_dma2(*SPI1, output_encoder_cs, *DMA1_Channel3, *DMA1_Channel4); // temporary switch
    Aksim2Encoder<18> output_encoder_direct(spi1_dma);
    QIA128 torque_sensor_direct(spi1_dma2);
//    TorqueSensor torque_sensor;
    TorqueSensor torque_sensor(torque_sensor_direct, output_encoder_direct, 5);
    OutputEncoder &output_encoder = torque_sensor.secondary();
    //EncoderBase output_encoder;
};

#include "../../motorlib/boards/config_obot_g474_motor.cpp"

namespace config {
    PT1000 motor_temperature(A1_DR);
};

void config_init() {
    System::api.add_api_variable("mdiag", new const APIUint8(&config::motor_encoder.diag_.word));
    System::api.add_api_variable("mdiag_raw", new const APIUint8(&config::motor_encoder.diag_raw_.word));
    System::api.add_api_variable("mcrc", new const APIUint8(&config::motor_encoder.crc_calc_));
    System::api.add_api_variable("merr", new APIUint32(&config::motor_encoder.diag_err_count_));
    System::api.add_api_variable("mwarn", new APIUint32(&config::motor_encoder.diag_warn_count_));
    System::api.add_api_variable("mcrc_cnt", new APIUint32(&config::motor_encoder.crc_err_count_));
    System::api.add_api_variable("mraw", new APIUint32(&config::motor_encoder.raw_value_));
    System::api.add_api_variable("mtemp", new const APICallbackFloat([](){ return config::motor_temperature.read(); }));
}

FrequencyLimiter temp_rate_motor = {10};
void config_maintenance() {
    if(temp_rate_motor.run()) {
        config::motor_temperature.read();
    }

}
