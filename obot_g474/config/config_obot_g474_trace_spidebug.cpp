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

#define COMMS   COMMS_USB

using TorqueSensor = TorqueSensorBase;
using MotorEncoder = EncoderBase;
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

        GPIO_SETL(A, 3, GPIO_MODE::OUTPUT, GPIO_SPEED::MEDIUM, 0); // CS2
        GPIO_SETL(A, 2, GPIO_MODE::OUTPUT, GPIO_SPEED::MEDIUM, 0); // CS3
        GPIOA->BSRR = GPIO_BSRR_BS2 | GPIO_BSRR_BS3 | GPIO_BSRR_BS4;
    }
};

namespace config {
    const uint32_t main_loop_frequency = 10000;    
    const uint32_t pwm_frequency = 50000;
    InitCode init_code;

    TorqueSensor torque_sensor;
    MotorEncoder motor_encoder;
    OutputEncoder output_encoder;

    GPIO cs1 {*GPIOA, 4, GPIO::OUTPUT};
    GPIO cs2 {*GPIOA, 3, GPIO::OUTPUT};
    GPIO cs3 {*GPIOA, 2, GPIO::OUTPUT};
    // CLK A4
    // MISO A5
    // MOSI A6
    SPIDMA spi_dma1 {SPIDMA::SP1, cs1, DMA1_CH1, DMA1_CH2, 0, 50, 50,
        SPI_CR1_MSTR | (4 << SPI_CR1_BR_Pos) | SPI_CR1_SSI | SPI_CR1_SSM | SPI_CR1_CPHA | SPI_CR1_CPOL, 200};
    SPIDMA spi_dma2 {SPIDMA::SP1, cs2, DMA1_CH1, DMA1_CH2, 0, 50, 50,
        SPI_CR1_MSTR | (4 << SPI_CR1_BR_Pos) | SPI_CR1_SSI | SPI_CR1_SSM | SPI_CR1_CPHA | SPI_CR1_CPOL, 200};
    SPIDMA spi_dma3 {SPIDMA::SP1, cs3, DMA1_CH1, DMA1_CH2, 0, 50, 50,
        SPI_CR1_MSTR | (4 << SPI_CR1_BR_Pos) | SPI_CR1_SSI | SPI_CR1_SSM | SPI_CR1_CPHA | SPI_CR1_CPOL, 200};
    SPIDebug spi_debug1(spi_dma1);
    SPIDebug spi_debug2(spi_dma2);
    SPIDebug spi_debug3(spi_dma3);
};

#include "../../motorlib/boards/config_obot_g474_trace.cpp"

void config_init() {
    SPIDEBUG_SET_DEBUG_API("1", System::api, config::spi_debug1);
    SPIDEBUG_SET_DEBUG_API("2", System::api, config::spi_debug2);
    SPIDEBUG_SET_DEBUG_API("3", System::api, config::spi_debug3);
}

void config_maintenance() {}