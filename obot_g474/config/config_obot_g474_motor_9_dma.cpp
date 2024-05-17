#include "../param/param_obot_g474.h"
#include "st_device.h"
#include "../../motorlib/qep_encoder.h"
#include "../../motorlib/torque_sensor.h"
#include "../../motorlib/gpio.h"
#include "../../motorlib/peripheral/stm32g4/spi_dma.h"
#include "../../motorlib/sensors/encoders/stm32g4/icpz_dma.h"
#include "../../motorlib/peripheral/stm32g4/spi_debug.h"
#include "../../motorlib/ads1235.h"
#include "../../motorlib/peripheral/stm32g4/pin_config.h"

using TorqueSensor = TorqueSensorBase;
using MotorEncoder = ICPZDMA;
using OutputEncoder = EncoderBase;

#define COMMS  COMMS_USB

struct InitCode {
    InitCode() {
        //SPI3 PZ
        DMAMUX1_Channel0->CCR =  DMA_REQUEST_SPI3_TX;
        DMAMUX1_Channel1->CCR =  DMA_REQUEST_SPI3_RX;
        SPI3->CR1 = SPI_CR1_MSTR | (3 << SPI_CR1_BR_Pos) | SPI_CR1_SSI | SPI_CR1_SSM;    // baud = clock/16 spi mode 0
        SPI3->CR2 = (7 << SPI_CR2_DS_Pos) | SPI_CR2_FRXTH;    // 8 bit

        // SPI3 setup for motor9 board
        GPIO_SETH(C, 13, GPIO_MODE::OUTPUT,  GPIO_SPEED::HIGH, 0);
        GPIO_SETL(B,  3, GPIO_MODE::ALT_FUN, GPIO_SPEED::HIGH, 6);
        GPIO_SETL(B,  4, GPIO_MODE::ALT_FUN, GPIO_SPEED::HIGH, 6);
        GPIO_SETL(B,  5, GPIO_MODE::ALT_FUN, GPIO_SPEED::HIGH, 6);
    }
};

namespace config {
    const uint32_t main_loop_frequency = 10000;    
    const uint32_t pwm_frequency = 50000;
    InitCode init_code;

    GPIO motor_encoder_cs = {*GPIOC, 13, GPIO::OUTPUT};
    SPIDMA spi3_dma = {SPIDMA::SP3, motor_encoder_cs, DMA1_CH1, DMA1_CH2, 0, 100, 0};
    ICPZDMA motor_encoder(spi3_dma, *DMAMUX1_Channel2, *DMAMUX1_Channel3, 3,
        output_start_cs_trigger, output_stop_cs_trigger, ICPZDMA::PZ03S);
    EncoderBase output_encoder;
    TorqueSensorBase torque_sensor;
};

#include "../../motorlib/boards/config_obot_g474_motor.cpp"

void config_init() {
    ICPZ_SET_DEBUG_VARIABLES("m", System::api, config::motor_encoder);
}

void config_maintenance() {}