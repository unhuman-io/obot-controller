#include "../param/param_obot_g474.h"
#include "../st_device.h"
#include "../../motorlib/peripheral/stm32g4/spi_dma.h"
#include "../../motorlib/aksim2_encoder.h"
#include "../../motorlib/torque_sensor.h"
#include "../../motorlib/gpio.h"
#include "../../motorlib/bmi270.h"
#include "../../motorlib/peripheral/stm32g4/pin_config.h"

using TorqueSensor = TorqueSensorBase;
//using MotorEncoder = Aksim2Encoder<18>;
using MotorEncoder = EncoderBase;
//using OutputEncoder = Aksim2Encoder<18>;
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
           
      SPI1->CR2 = (7 << SPI_CR2_DS_Pos) | SPI_CR2_FRXTH;   // 8 bit
      // ORDER DEPENDANCE SPE set last
      SPI1->CR1 = SPI_CR1_MSTR | (5 << SPI_CR1_BR_Pos) | SPI_CR1_SSI | SPI_CR1_SSM | SPI_CR1_CPOL;    // baud = clock/64
      DMAMUX1_Channel0->CCR =  DMA_REQUEST_SPI3_TX;
      DMAMUX1_Channel1->CCR =  DMA_REQUEST_SPI3_RX;
      DMAMUX1_Channel2->CCR =  DMA_REQUEST_SPI1_TX;
      DMAMUX1_Channel3->CCR =  DMA_REQUEST_SPI1_RX;

      GPIO_SETL(C, 4, GPIO::OUTPUT, GPIO_SPEED::LOW, 0);        // imu cs
      GPIOC->BSRR |= GPIO_BSRR_BS4; // set imu cs

      GPIO_SETL(A, 0, GPIO_MODE::OUTPUT, GPIO_SPEED::LOW, 0);    // motor cs

      // imu
      SPI1->CR1 = SPI_CR1_MSTR | (4 << SPI_CR1_BR_Pos) | SPI_CR1_SSI | SPI_CR1_SSM;    // baud = clock/32
    }
};

namespace config {
    const uint32_t main_loop_frequency = 10000;
    const uint32_t pwm_frequency = 20000;
    InitCode init_code;

    GPIO output_encoder_cs(*GPIOC, 3, GPIO::OUTPUT);
    SPIDMA spi1_dma(*SPI1, output_encoder_cs, *DMA1_Channel3, *DMA1_Channel4);
    //OutputEncoder output_encoder(spi1_dma);
    EncoderBase motor_encoder;
    TorqueSensor torque_sensor;
    GPIO motor_encoder_cs(*GPIOA, 0, GPIO::OUTPUT);
    SPIDMA spi3_dma(*SPI3, motor_encoder_cs, *DMA1_Channel2, *DMA1_Channel3);
    //MotorEncoder motor_encoder(spi3_dma);


    GPIO imu_cs(*GPIOC, 4, GPIO::OUTPUT);
    SPIDMA spi1_dma2(*SPI1, imu_cs, *DMA1_Channel3, *DMA1_Channel4, 40, 40);
    BMI270 imu(spi1_dma2);
    EncoderBase output_encoder;
};

#define SPI1_REINIT_CALLBACK
void spi1_reinit_callback() {
    SPI1->CR1=0;
    SPI1->CR2 = (7 << SPI_CR2_DS_Pos) | SPI_CR2_FRXTH;   // 8 bit
    // ORDER DEPENDANCE SPE set last
    SPI1->CR1 = SPI_CR1_MSTR | (4 << SPI_CR1_BR_Pos) | SPI_CR1_SSI | SPI_CR1_SSM;    // baud = clock/64
    config::spi1_dma2.reinit();
}

#include "../../motorlib/boards/config_obot_g474_motor.cpp"

void config_init() {
    config::spi1_dma2.register_operation_ = config::drv.register_operation_;
    // System::api.add_api_variable("mdiag", new const APIUint8(&config::motor_encoder.diag_.word));
    // System::api.add_api_variable("mdiag_raw", new const APIUint8(&config::motor_encoder.diag_raw_.word));
    // System::api.add_api_variable("mcrc", new const APIUint8(&config::motor_encoder.crc_calc_));
    // System::api.add_api_variable("merr", new APIUint32(&config::motor_encoder.diag_err_count_));
    // System::api.add_api_variable("mwarn", new APIUint32(&config::motor_encoder.diag_warn_count_));
    // System::api.add_api_variable("mcrc_cnt", new APIUint32(&config::motor_encoder.crc_err_count_));
    // System::api.add_api_variable("mraw", new APIUint32(&config::motor_encoder.raw_value_));
    System::api.add_api_variable("imu", new const APICallback([](){ return config::imu.get_string(); }));
    System::api.add_api_variable("ax", new const APICallbackFloat([](){ return config::imu.data_.acc_x*8./pow(2,15); }));
    System::api.add_api_variable("ay", new const APICallbackFloat([](){ return config::imu.data_.acc_y*8./pow(2,15); }));
    System::api.add_api_variable("az", new const APICallbackFloat([](){ return config::imu.data_.acc_z*8./pow(2,15); }));
    System::api.add_api_variable("gx", new const APICallbackFloat([](){ return config::imu.data_.gyr_x*2000.*M_PI/180/pow(2,15); }));
    System::api.add_api_variable("gy", new const APICallbackFloat([](){ return config::imu.data_.gyr_y*2000.*M_PI/180/pow(2,15); }));
    System::api.add_api_variable("gz", new const APICallbackFloat([](){ return config::imu.data_.gyr_z*2000.*M_PI/180/pow(2,15); }));


    GPIO_SETL(A, 4, 1, 0, 0);
    GPIOA->BSRR |= GPIO_BSRR_BS4; // set drv cs

    config::imu.init();
}

FrequencyLimiter imu_rate = {100};

void config_maintenance() {
    if (imu_rate.run()) {
        
        config::imu.read();
    }
}