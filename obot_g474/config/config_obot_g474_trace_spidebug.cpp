#include "../param/param_obot_g474.h"
#include "st_device.h"
#include "../../motorlib/encoder.h"
#include "../../motorlib/torque_sensor.h"
#include "../../motorlib/gpio.h"
#include <algorithm>
#include "../../motorlib/peripheral/stm32g4/pin_config.h"
#include "../../motorlib/peripheral/stm32g4/spi_dma.h"
#include "../../motorlib/peripheral/stm32g4/spi_debug.h"
#include "../../motorlib/gpio.h"
#include "../../motorlib/sensors/encoders/stm32g4/a17803.h"

#define COMMS   COMMS_USB

using TorqueSensor = TorqueSensorBase;
using MotorEncoder = A17803;
//using MotorEncoder = EncoderBase;
using OutputEncoder = EncoderBase;

struct InitCode {
    InitCode() {
        // trace pins enable
        RCC->AHB2ENR |= RCC_AHB2ENR_GPIOEEN;
        GPIO_SETL(E, 2, GPIO_MODE::ALT_FUN, GPIO_SPEED::VERY_HIGH, 0);
        GPIO_SETL(E, 3, GPIO_MODE::ALT_FUN, GPIO_SPEED::VERY_HIGH, 0);
        GPIO_SETL(E, 4, GPIO_MODE::ALT_FUN, GPIO_SPEED::VERY_HIGH, 0);
        GPIO_SETL(E, 5, GPIO_MODE::ALT_FUN, GPIO_SPEED::VERY_HIGH, 0);
        GPIO_SETL(E, 6, GPIO_MODE::ALT_FUN, GPIO_SPEED::VERY_HIGH, 0);
        //ETM->CR |= ETM_CR_ETMEN;
        uint32_t *etmcr = (uint32_t *)0xE0041000;
        *etmcr |= 1 << 11;
        uint32_t *etmteevr = (uint32_t *)0xE0041020;
        *etmteevr = 0x000037ef; // ON

        DMAMUX1_Channel0->CCR =  DMA_REQUEST_SPI1_TX;
        DMAMUX1_Channel1->CCR =  DMA_REQUEST_SPI1_RX;

        RCC->AHB1ENR |= RCC_AHB1ENR_CRCEN; // enable CRC peripheral

    }
};

namespace config {
    const uint32_t main_loop_frequency = 10000;    
    const uint32_t pwm_frequency = 50000;
    InitCode init_code;

    TorqueSensor torque_sensor;
    OutputEncoder output_encoder;

    GPIO cs {*GPIOA, 4, GPIO::OUTPUT};
    // CLK A4
    // MISO A5
    // MOSI A6
    SPIDMA spi_dma {SPIDMA::SP1, cs, DMA1_CH1, DMA1_CH2, 0, 50, 50,
        SPI_CR1_MSTR | (4 << SPI_CR1_BR_Pos) | SPI_CR1_SSI | SPI_CR1_SSM | SPI_CR1_CPHA | SPI_CR1_CPOL};
    SPIDebug spi_debug(spi_dma);
    //A17803 encoder(spi_dma);
    MotorEncoder motor_encoder(spi_dma);
    //MotorEncoder motor_encoder;
};

#include "../../motorlib/boards/config_obot_g474_trace.cpp"

void config_init() {
    SPIDEBUG_SET_DEBUG_API(, System::api, config::spi_debug);
    A17803_SET_DEBUG_API(, System::api, config::motor_encoder);
}

void config_maintenance() {}