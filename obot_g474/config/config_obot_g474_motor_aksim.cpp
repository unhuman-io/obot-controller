#include "../param/param_obot_g474.h"
#include "../st_device.h"
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

extern "C" void SystemClock_Config();
void pin_config_obot_g474_motor_r0();

// setup for R4

struct InitCode {
    InitCode() {
      SystemClock_Config();
      pin_config_obot_g474_motor_r0();
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

      // gpio out
      GPIO_SETL(A, 1, GPIO::OUTPUT, GPIO_SPEED::VERY_HIGH, 0);
      // gpio in
      GPIO_SETL(A, 2, GPIO::INPUT, GPIO_SPEED::VERY_HIGH, 0);
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
    SPIDMA spi1_dma(*SPI1, output_encoder_cs, *DMA1_Channel3, *DMA1_Channel4);
    OutputEncoder output_encoder(spi1_dma);
    //EncoderBase output_encoder;
};

#define SPI1_REINIT_CALLBACK
void spi1_reinit_callback() {
    SPI1->CR1=0;
    SPI1->CR2 = (7 << SPI_CR2_DS_Pos) | SPI_CR2_FRXTH;   // 8 bit
    // ORDER DEPENDANCE SPE set last
    SPI1->CR1 = SPI_CR1_MSTR | (5 << SPI_CR1_BR_Pos) | SPI_CR1_SSI | SPI_CR1_SSM | SPI_CR1_CPOL;    // baud = clock/64
    config::spi1_dma.reinit();
}

#include "../../motorlib/boards/config_obot_g474_motor.cpp"

namespace config {
    PT1000 motor_temperature(A1_DR);
};

void config_init() {
    config::output_encoder.spi_dma_.register_operation_ = config::drv.register_operation_;
    System::api.add_api_variable("mdiag", new const APIUint8(&config::motor_encoder.diag_.word));
    System::api.add_api_variable("mdiag_raw", new const APIUint8(&config::motor_encoder.diag_raw_.word));
    System::api.add_api_variable("mcrc", new const APIUint8(&config::motor_encoder.crc_calc_));
    System::api.add_api_variable("merr", new APIUint32(&config::motor_encoder.diag_err_count_));
    System::api.add_api_variable("mwarn", new APIUint32(&config::motor_encoder.diag_warn_count_));
    System::api.add_api_variable("mcrc_cnt", new APIUint32(&config::motor_encoder.crc_err_count_));
    System::api.add_api_variable("mraw", new APIUint32(&config::motor_encoder.raw_value_));
    System::api.add_api_variable("mtemp", new const APICallbackFloat([](){ return config::motor_temperature.read(); }));
    System::api.add_api_variable("oerr", new APIUint32(&config::output_encoder.diag_err_count_));
    System::api.add_api_variable("owarn", new APIUint32(&config::output_encoder.diag_warn_count_));
    System::api.add_api_variable("ocrc_cnt", new APIUint32(&config::output_encoder.crc_err_count_));
}

FrequencyLimiter temp_rate_motor = {10};
void config_maintenance() {
    if(temp_rate_motor.run()) {
        config::motor_temperature.read();
        if (config::motor_temperature.get_temperature() > 100) {
            config::main_loop.status_.error.motor_temperature = true;
        }
    }
    if(config::motor_encoder.crc_err_count_ > 100 || config::motor_encoder.diag_err_count_ > 100 ||
        config::motor_encoder.diag_warn_count_ > 10000) {
            config::main_loop.status_.error.motor_encoder = true;
    }
    if(config::output_encoder.crc_err_count_ > 100 || config::output_encoder.diag_err_count_ > 100 ||
        config::output_encoder.diag_warn_count_ > 10000) {
            config::main_loop.status_.error.output_encoder = true;
    }

}