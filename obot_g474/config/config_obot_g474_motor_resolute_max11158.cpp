#include "../param/param_obot_g474.h"
#include "st_device.h"
#include "../../motorlib/qep_encoder.h"
#include "../../motorlib/torque_sensor.h"
#include "../../motorlib/gpio.h"

#include "../../motorlib/peripheral/stm32g4/spi_dma.h"
#include "../../motorlib/sensor_multiplex.h"
#include "../../motorlib/max11158.h"
#include "../../motorlib/sensors/encoders/resolute_encoder.h"
#include "../../motorlib/sensors/encoders/stm32g4/resolute_dma_encoder.h"
#include "../../motorlib/peripheral/stm32g4/pin_config.h"
#define COMMS   COMMS_USB

using TorqueSensor = TorqueSensorMultiplex<MAX11158, ResoluteEncoder>;
using MotorEncoder = ResoluteEncoder;
using OutputEncoder = TorqueSensor::SecondarySensor;

struct InitCode {
    InitCode() {
      SPI1->CR2 = (7 << SPI_CR2_DS_Pos) | SPI_CR2_FRXTH;   // 8 bit
      // ORDER DEPENDANCE SPE set last
      SPI1->CR1 = SPI_CR1_MSTR | (4 << SPI_CR1_BR_Pos) | SPI_CR1_SSI | SPI_CR1_SSM | SPI_CR1_CPOL | SPI_CR1_CPHA;    // baud = clock/32
      DMAMUX1_Channel0->CCR =  DMA_REQUEST_SPI3_TX;
      DMAMUX1_Channel1->CCR =  DMA_REQUEST_SPI3_RX;
      DMAMUX1_Channel2->CCR =  DMA_REQUEST_SPI1_TX;
      DMAMUX1_Channel3->CCR =  DMA_REQUEST_SPI1_RX;
              GPIO_SETL(C, 0, GPIO_MODE::OUTPUT, GPIO_SPEED::VERY_HIGH, 0); // main loop scope
        GPIO_SETL(C, 1, GPIO_MODE::OUTPUT, GPIO_SPEED::VERY_HIGH, 0); // fast loop scope
        GPIO_SETL(C, 2, GPIO_MODE::OUTPUT, GPIO_SPEED::VERY_HIGH, 0); // usb int scope

      GPIO_SETL(A, 0, GPIO_MODE::OUTPUT, GPIO_SPEED::VERY_HIGH, 0);   // PA0 TCS on BBS
      GPIOA->BSRR = GPIO_BSRR_BS0;

      GPIO_SETL(D, 2, GPIO_MODE::OUTPUT, GPIO_SPEED::VERY_HIGH, 0);   // PD2-> motor encoder cs
      GPIOD->BSRR = GPIO_BSRR_BS2;

      // Fixed high value on SPI1 MOSI, required by MAX11158, ok for resolute
      GPIO_SETL(A, 7, GPIO_MODE::OUTPUT, GPIO_SPEED::VERY_HIGH, 0);
      GPIOA->BSRR = GPIO_BSRR_BS7;
    }
};

inline void motor_start_cs_trigger() {
    HRTIM1->sTimerxRegs[0].TIMxDIER = HRTIM_TIMDIER_CMP1DE;
}
    
inline void motor_stop_cs_trigger() {
    HRTIM1->sTimerxRegs[0].TIMxDIER = 0;
    // wait for CS high
    while(!(GPIOD->IDR & 0x4));
}

namespace config {
    const uint32_t main_loop_frequency = 10000;    
    const uint32_t pwm_frequency = 15000; // max resolute read frequency 30 kHz
    InitCode init_code;

    GPIO motor_encoder_cs(*GPIOD, 2, GPIO::OUTPUT);
    SPIDMA spi3_dma(SPIDMA::SP3, motor_encoder_cs, DMA1_CH1, DMA1_CH2, 0, 100, 100,
        SPI_CR1_MSTR | (4 << SPI_CR1_BR_Pos) | SPI_CR1_SSI | SPI_CR1_SSM | SPI_CR1_CPOL | SPI_CR1_CPHA);
    ResoluteEncoder motor_encoder(spi3_dma);
    //ResoluteDMAEncoder motor_encoder(spi3_dma, *DMAMUX1_Channel0, *DMAMUX1_Channel1, 2, motor_start_cs_trigger, motor_stop_cs_trigger);

    GPIO output_encoder_cs(*GPIOC, 3, GPIO::OUTPUT);
    SPIDMA spi1_dma(SPIDMA::SP1, output_encoder_cs, DMA1_CH3, DMA1_CH4, 0, 100, 100,
    SPI_CR1_MSTR | (4 << SPI_CR1_BR_Pos) | SPI_CR1_SSI | SPI_CR1_SSM | SPI_CR1_CPOL | SPI_CR1_CPHA);
    ResoluteEncoder output_encoder1(spi1_dma);

    GPIO torque_sensor_cs(*GPIOA, 0, GPIO::OUTPUT);
    SPIDMA spi1_dma2(SPIDMA::SP1, torque_sensor_cs, DMA1_CH3, DMA1_CH4, 0, 100, 100,
        SPI_CR1_MSTR | 6 << SPI_CR1_BR_Pos | SPI_CR1_SSI | SPI_CR1_SSM);
    MAX11158 torque_sensor1(spi1_dma2);

    TorqueSensor torque_sensor(torque_sensor1, output_encoder1);
    OutputEncoder &output_encoder = torque_sensor.secondary();
    
};

#define SPI1_REINIT_CALLBACK
void spi1_reinit_callback() {
   SPI1->CR1=0;
    SPI1->CR2 = (7 << SPI_CR2_DS_Pos) | SPI_CR2_FRXTH;   // 8 bit
    // ORDER DEPENDANCE SPE set last
    SPI1->CR1 = SPI_CR1_MSTR | (4 << SPI_CR1_BR_Pos) | SPI_CR1_SSI | SPI_CR1_SSM | SPI_CR1_CPOL | SPI_CR1_CPHA;    // baud = clock/64
    config::spi1_dma.reinit();
}

#include "../../motorlib/boards/config_obot_g474_motor.cpp"

void config_init() {
    RESOLUTE_SET_DEBUG_VARIABLES("m", System::api, config::motor_encoder);
    RESOLUTE_SET_DEBUG_VARIABLES("o", System::api, config::output_encoder1);
    MAX11158_SET_DEBUG_VARIABLES("t", System::api, config::torque_sensor1);
}

void config_maintenance() {}