#ifndef ST_DEVICE_H
#define ST_DEVICE_H

#include "stm32f446xx.h"
#define CPU_FREQUENCY_HZ    180e6
#define I_A_DR  ADC3->JDR1
#define I_B_DR  ADC2->JDR1
#define I_C_DR  ADC1->JDR1
#define V_BUS_DR ADC1->DR
#define V_A_DR  ADC3->JDR3
#define V_B_DR  ADC2->JDR3
#define V_C_DR  ADC1->JDR3
#define MPS_I_A_DR  ADC3->JDR2
#define MPS_I_B_DR  ADC2->JDR2
#define MPS_I_C_DR  ADC1->JDR2

#endif
