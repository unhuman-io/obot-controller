#include "../param/param_obot_g474.h"
#include "../st_device.h"
#include "../../motorlib/peripheral/stm32g4/spi_dma.h"
#include "../../motorlib/icpz.h"
#include "../../motorlib/torque_sensor.h"
#include "../../motorlib/gpio.h"
#include "../../motorlib/ma732_encoder.h"
#include "../../motorlib/peripheral/stm32g4/ams_encoder.h"
#include "../../motorlib/peripheral/stm32g4/pin_config.h"

using TorqueSensor = TorqueSensorBase;
using MotorEncoder = ICPZ;
//using MotorEncoder = EncoderBase;
//using OutputEncoder = AMSEncoder;
using OutputEncoder = EncoderBase;

extern "C" void SystemClock_Config();
void pin_config_obot_g474_motor_r0();

struct InitCode {
    InitCode() {
      SystemClock_Config();
      pin_config_obot_g474_motor_r0();
      GPIO_SETL(C, 3, GPIO_MODE::OUTPUT, GPIO_SPEED::HIGH, 0); // spi 1 cs
      MASK_SET(GPIOA->PUPDR, GPIO_PUPDR_PUPD6, GPIO_PULL::UP); // miso
      GPIO_SETL(A, 4, 1, 0, 0);
      GPIOA->BSRR = GPIO_ODR_OD4;
      SPI1->CR1 = 0;
      SPI1->CR2 = (15 << SPI_CR2_DS_Pos);   // 16 bit
      // ORDER DEPENDANCE SPE set last
      SPI1->CR1 = SPI_CR1_MSTR | (4 << SPI_CR1_BR_Pos) | SPI_CR1_CPHA | SPI_CR1_SSI | SPI_CR1_SSM | SPI_CR1_SPE;    // baud = clock/32
    
      
        //SPI3 PZ
        DMAMUX1_Channel0->CCR =  DMA_REQUEST_SPI3_TX;
        DMAMUX1_Channel1->CCR =  DMA_REQUEST_SPI3_RX;
        SPI3->CR1 = SPI_CR1_MSTR | (3 << SPI_CR1_BR_Pos) | SPI_CR1_SSI | SPI_CR1_SSM | SPI_CR1_SPE;    // baud = clock/16 spi mode 0
        SPI3->CR2 = (7 << SPI_CR2_DS_Pos) | SPI_CR2_FRXTH;    // 8 bit   
    }
};

namespace config {
    const uint32_t main_loop_frequency = 10000;    
    const uint32_t pwm_frequency = 50000;
    InitCode init_code;

    GPIO motor_encoder_cs(*GPIOD, 2, GPIO::OUTPUT);
    SPIDMA spi3dma(*SPI3, motor_encoder_cs, *DMA1_Channel1, *DMA1_Channel2);
    ICPZ motor_encoder(spi3dma);
    //MotorEncoder motor_encoder;
    TorqueSensor torque_sensor;
    GPIO output_encoder_cs(*GPIOC, 3, GPIO::OUTPUT);
    //AMSEncoder output_encoder(*SPI1, output_encoder_cs);
    OutputEncoder output_encoder;
};

#include "config_obot_g474_motor.cpp"

float get_motor_temperature(float v) {
    float r = v/4096;
    float R = 10/(1/r-1);
    float inv_T = 0.0003011*std::log(R)+0.00265;
    float T = 1/inv_T - 273.15;
    return T;
}

float Tmotor = 0;
extern float T;

void config_init() {
    System::api.add_api_variable("Tmotor", new const APICallbackFloat([](){ return get_motor_temperature(A3_DR); }));
    config::main_loop.reserved0_ = reinterpret_cast<uint32_t *>(&config::main_loop.position_controller_.controller_.error_);
    config::main_loop.reserved1_ = reinterpret_cast<uint32_t *>(&T);
}

void config_maintenance() {
    Tmotor = get_motor_temperature(A3_DR);
    if (Tmotor> 110) {
        //config::main_loop.status_.error.motor_temperature = 1;
    }
}