#ifndef ST_DEVICE_H
#define ST_DEVICE_H

#include "stm32g474xx.h"
#include "stm32g4xx_hal.h"
#define CPU_FREQUENCY_HZ    170e6
#define I_A_DR  ADC5->JDR1
#define I_B_DR  ADC4->JDR1
#define I_C_DR  ADC3->JDR1
#define V_BUS_DR ADC1->DR

#endif
