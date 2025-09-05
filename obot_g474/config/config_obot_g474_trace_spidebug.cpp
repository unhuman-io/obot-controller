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
#include "../../motorlib/sensors/encoders/stm32g4/a17803_dma.h"

#define COMMS   COMMS_USB

using TorqueSensor = TorqueSensorBase;
//using MotorEncoder = A17803;
//using MotorEncoder = EncoderBase;
using MotorEncoder = A17803_DMA<SPIDMA>;
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
        GPIO_SETL(A, 0, GPIO_MODE::OUTPUT, GPIO_SPEED::MEDIUM, 0); // dummy
        GPIOA->BSRR = GPIO_BSRR_BS3 | GPIO_BSRR_BS0 | GPIO_BSRR_BS4;
        MASK_SET(SYSCFG->EXTICR[0], SYSCFG_EXTICR1_EXTI0, 0); // EXTI PA0

        HRTIM1->sTimerxRegs[0].CMP1xR = 100;
        HRTIM1->sTimerxRegs[0].CMP2xR = 45333*.33;
        HRTIM1->sTimerxRegs[0].CMP3xR = 45333*.66;
        HRTIM1->sTimerxRegs[0].TIMxCR2 = 0;// doesn't matter 2 << HRTIM_TIMCR2_ROM_Pos; // on period
        HRTIM1->sTimerxRegs[0].PERxR = 45333;
        HRTIM1->sTimerxRegs[0].TIMxCR |= HRTIM_TIMCR_PREEN | HRTIM_TIMCR_TRSTU | HRTIM_TIMCR_CONT | 2 << HRTIM_TIMCR_CK_PSC_Pos;

        DMAMUX1_Channel2->CCR = 96; // hrtima
        DMAMUX1_Channel3->CCR = 1; // reqgen0
        DMAMUX1_RequestGenerator0->RGCR = 1 << DMAMUX_RGxCR_GPOL_Pos | 17 << DMAMUX_RGxCR_SIG_ID_Pos | DMAMUX_RGxCR_GE;

        RCC->AHB1ENR |= RCC_AHB1ENR_CRCEN; // enable CRC peripheral

    }
};

void start_cs_trigger() {
    HRTIM1->sTimerxRegs[0].TIMxDIER = HRTIM_TIMDIER_CMP1DE |  HRTIM_TIMDIER_CMP2DE |  HRTIM_TIMDIER_CMP3DE ;
}
void stop_cs_trigger() {
    HRTIM1->sTimerxRegs[0].TIMxDIER = 0;
    // wait for CS high
    while(!(GPIOA->IDR & 0x0001));
}

namespace config {
    const uint32_t main_loop_frequency = 10000;    
    const uint32_t pwm_frequency = 30000;
    InitCode init_code;

    TorqueSensor torque_sensor;
    OutputEncoder output_encoder;

    GPIO cs1 {*GPIOA, 4, GPIO::OUTPUT};
    GPIO cs2 {*GPIOA, 3, GPIO::OUTPUT};
    // CLK A4
    // MISO A5
    // MOSI A6
    SPIDMA spi_dma1 {SPIDMA::SP1, cs1, DMA1_CH1, DMA1_CH2, 0, 50, 50,
        SPI_CR1_MSTR | (4 << SPI_CR1_BR_Pos) | SPI_CR1_SSI | SPI_CR1_SSM | SPI_CR1_CPHA | SPI_CR1_CPOL, 200};
    SPIDMA spi_dma2 {SPIDMA::SP1, cs2, DMA1_CH1, DMA1_CH2, 0, 50, 50,
        SPI_CR1_MSTR | (4 << SPI_CR1_BR_Pos) | SPI_CR1_SSI | SPI_CR1_SSM | SPI_CR1_CPHA | SPI_CR1_CPOL, 200};
    SPIDebug spi_debug(spi_dma1);

    //A17803_DMA a17803_dma(spi_dma1, spi_dma2);
    //A17803 encoder(spi_dma1);
    //MotorEncoder motor_encoder(spi_dma1);
    //MotorEncoder motor_encoder;
    MotorEncoder motor_encoder(spi_dma1, spi_dma2, start_cs_trigger, stop_cs_trigger);
};

#include "../../motorlib/boards/config_obot_g474_trace.cpp"

void config_init() {
    SPIDEBUG_SET_DEBUG_API(, System::api, config::spi_debug);
    A17803_DMA_SET_DEBUG_API(, System::api, config::motor_encoder);
}

void config_maintenance() {}