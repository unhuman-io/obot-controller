#include "../param/param_obot_g474.h"
#include "st_device.h"
#include "../../motorlib/encoder.h"
#include "../../motorlib/torque_sensor.h"
#include "../../motorlib/gpio.h"
#include <algorithm>
#include "../../motorlib/peripheral/stm32g4/pin_config.h"
#define COMMS   COMMS_USB

using TorqueSensor = TorqueSensorBase;
using MotorEncoder = EncoderBase;
using OutputEncoder = EncoderBase;

uint32_t gpio_bsrr[2] = {4, 4<<16};

struct InitCode {
    InitCode() {
        GPIO_SETH(A, 9, GPIO_MODE::ALT_FUN, GPIO_SPEED::VERY_HIGH, 13); // hrtim1 cha2
        GPIO_SETH(A, 8, GPIO_MODE::ALT_FUN, GPIO_SPEED::VERY_HIGH, 13); // hrtim1 cha1
        HRTIM1->sTimerxRegs[0].CMP1xR = 32*4;
        HRTIM1->sTimerxRegs[0].TIMxCR2 = HRTIM_TIMCR2_UDM;
        HRTIM1->sTimerxRegs[0].TIMxCR |= HRTIM_TIMCR_PREEN | HRTIM_TIMCR_TRSTU | HRTIM_TIMCR_CONT;
        HRTIM1->sTimerxRegs[0].TIMxCR |= 0;
        HRTIM1->sTimerxRegs[0].PERxR = 54400;
        HRTIM1->sTimerxRegs[0].SETx2R = HRTIM_SET2R_CMP1;
        //->sTimerxRegs[0].RSTx2R = HRTIM_RST2R_PER;
        // need to set  HRTIM_MCR_TACEN

        ADC4->CFGR |= ADC_CFGR_DMAEN | ADC_CFGR_DMACFG;
        DMAMUX1_Channel0->CCR = 38;
        DMA1_Channel1->CMAR = (uint32_t) &gpio_bsrr;
        DMA1_Channel1->CPAR = (uint32_t) &GPIOC->BSRR;
        DMA1_Channel1->CNDTR = 2;
        DMA1_Channel1->CCR = DMA_CCR_CIRC | DMA_CCR_DIR | DMA_CCR_EN | DMA_CCR_MINC | DMA_CCR_MSIZE_1 | DMA_CCR_PSIZE_1;
    }
};

namespace config {
    const uint32_t main_loop_frequency = 10000;    
    const uint32_t pwm_frequency = 50000;
    InitCode init_code;

    MotorEncoder motor_encoder;
    TorqueSensor torque_sensor;
    OutputEncoder output_encoder;
};

#include "../../motorlib/boards/config_obot_g474_motor.cpp"

void config_init() {}

void config_maintenance() {}