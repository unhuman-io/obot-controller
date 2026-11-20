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

void start_cs_trigger() {
    DMA1_Channel5->CNDTR = 1;
    DMA1_Channel6->CCR |= DMA_CCR_EN;
    DMA1_Channel5->CCR |= DMA_CCR_EN;
    HRTIM1->sTimerxRegs[0].TIMxDIER = HRTIM_TIMDIER_CMP1DE |  HRTIM_TIMDIER_CMP2DE;
}
void stop_cs_trigger_and_wait_cs_high() {
    HRTIM1->sTimerxRegs[0].TIMxDIER = 0;
    // wait for CS high
    us_delay(25);
    while(!(GPIOC->IDR & (1 << 13)));
    DMA1_Channel5->CCR &= ~DMA_CCR_EN;
    DMA1_Channel6->CCR &= ~DMA_CCR_EN;
}

uint32_t gpio_cs_bsrr_r = {1 << (13+16)};
uint32_t gpio_cs_bsrr_s = {1 << 13};
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

        // trigger motor encoder on HRTIM CHA
        const uint32_t period = CPU_FREQUENCY_HZ*8/40000; // requires hrtim mult of 8, pwm 50khz
        // read takes 170*8*5 = 6800 counts
        static_assert(period > 32767);
        static_assert(period < 65536);
        HRTIM1->sTimerxRegs[0].CMP1xR = period - 6800*2.3 + 4000;
        HRTIM1->sTimerxRegs[0].CMP2xR = period - 6800 + 4000;
        HRTIM1->sTimerxRegs[0].PERxR = period; 
        HRTIM1->sTimerxRegs[0].TIMxCR |= HRTIM_TIMCR_PREEN | HRTIM_TIMCR_TRSTU | HRTIM_TIMCR_CONT | 2 << HRTIM_TIMCR_CK_PSC_Pos;

        DMAMUX1_Channel4->CCR = 96; // hrtima
        DMA1_Channel5->CMAR = (uint32_t)&gpio_cs_bsrr_r;
        DMA1_Channel5->CPAR = (uint32_t)&GPIOC->BSRR;
        DMA1_Channel5->CNDTR = 1;
        GPIOC->BSRR = 1 << 13;
        DMA1_Channel5->CCR = DMA_CCR_CIRC | DMA_CCR_DIR | DMA_CCR_EN | DMA_CCR_MINC | DMA_CCR_MSIZE_1 | DMA_CCR_PSIZE_1;

        DMAMUX1_Channel5->CCR = 1; // reqgen0
        DMAMUX1_RequestGenerator0->RGCR = 1 << DMAMUX_RGxCR_GPOL_Pos | 17 << DMAMUX_RGxCR_SIG_ID_Pos | DMAMUX_RGxCR_GE;
        DMA1_Channel6->CMAR = (uint32_t)&gpio_cs_bsrr_s;
        DMA1_Channel6->CPAR = (uint32_t)&GPIOC->BSRR;
        DMA1_Channel6->CNDTR = 1;
        DMA1_Channel6->CCR = DMA_CCR_CIRC | DMA_CCR_DIR | DMA_CCR_EN | DMA_CCR_MINC | DMA_CCR_MSIZE_1 | DMA_CCR_PSIZE_1;

        MASK_SET(SYSCFG->EXTICR[3], SYSCFG_EXTICR4_EXTI13, 2); // EXTI PC13
    }
};

namespace config {
    const uint32_t main_loop_frequency = 10000;    
    const uint32_t pwm_frequency = 40000;
    InitCode init_code;

    GPIO motor_encoder_cs = {*GPIOC, 13, GPIO::OUTPUT};
    SPIDMA spi3_dma = {SPIDMA::SP3, motor_encoder_cs, DMA1_CH1, DMA1_CH2, 0, 100, 0};
    ICPZDMA motor_encoder(spi3_dma, *DMAMUX1_Channel0, *DMAMUX1_Channel1, 13, 
        start_cs_trigger, stop_cs_trigger_and_wait_cs_high, ICPZDMA::PZ03S);

    EncoderBase output_encoder;
    TorqueSensorBase torque_sensor;
};

#include "../../motorlib/boards/config_obot_g474_motor.cpp"

void config_init() {
    ICPZ_SET_DEBUG_VARIABLES("m", System::api, config::motor_encoder);
}

void config_maintenance() {}