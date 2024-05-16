#include "../param/param_obot_g474_aksim.h"
#include "st_device.h"
#include "../../motorlib/peripheral/stm32g4/spi_dma.h"
#include "../../motorlib/gpio.h"
#include "../../motorlib/encoder.h"
#include "../../motorlib/temperature_sensor.h"
#include "../../motorlib/max11158.h"
#include "../../motorlib/peripheral/stm32g4/pin_config.h"
#define COMMS   COMMS_USB

using TorqueSensor = MAX11158;
using MotorEncoder = EncoderBase;
using OutputEncoder = EncoderBase;



struct InitCode {
    InitCode() {
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

      
      // ORDER DEPENDANCE SPE set last
      SPI1->CR1 = SPI_CR1_MSTR | (6 << SPI_CR1_BR_Pos) | SPI_CR1_SSI | SPI_CR1_SSM;    // baud = clock/64

      GPIO_SETL(A, 0, GPIO_MODE::OUTPUT, GPIO_SPEED::VERY_HIGH, 0);   // PA0-> motor encoder cs
      //MASK_SET(GPIOC->PUPDR, GPIO_PUPDR_PUPD10, GPIO_PULL::UP);       // keep motor sclk high when spi disabled

      // gpio out
      GPIO_SETL(A, 1, GPIO::OUTPUT, GPIO_SPEED::VERY_HIGH, 0);
      // gpio in
      GPIO_SETL(A, 2, GPIO::INPUT, GPIO_SPEED::VERY_HIGH, 0);
      

      // PA0 TCS
      GPIO_SETL(A, 0, GPIO_MODE::OUTPUT, GPIO_SPEED::MEDIUM, 0); 
      GPIOA->BSRR = GPIO_BSRR_BS0;
      GPIOC->BSRR = GPIO_BSRR_BS3;  // hdr17 (1CS2), output encoder cs
    }
};

namespace config {
    const uint32_t main_loop_frequency = 10000;
    const uint32_t pwm_frequency = 25000;
    InitCode init_code;

    GPIO torque_sensor_cs(*GPIOA, 0, GPIO::OUTPUT);
    SPIDMA spi1_dma(*SPI1, torque_sensor_cs, *DMA1_Channel3, *DMA1_Channel4);
    TorqueSensor torque_sensor(spi1_dma);
    EncoderBase motor_encoder;
    OutputEncoder output_encoder;
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


void config_init() {
    config::torque_sensor.set_debug_variables("t", System::api);
}

void config_maintenance() {

}
