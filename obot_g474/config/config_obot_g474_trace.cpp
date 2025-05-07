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

// #define I_A_DR  ADC3->JDR1
// #define I_B_DR  ADC4->JDR1
// #define I_C_DR  ADC5->JDR1
// #define I_A0_DR  ADC3->DR
// #define I_B0_DR  ADC4->DR
// #define I_C0_DR  ADC5->DR
// #define V_BUS_DR ADC1->DR
// #define V_REF_DR ADC1->JDR2
// #define V_TEMP_DR ADC1->JDR1
// #define A1_DR ADC1->JDR3
// #define A2_DR ADC1->JDR4
// #define A3_DR ADC2->JDR1

// #define TSENSE ADC2->JDR2
// #define TSENSE2 ADC2->JDR3

// #define I5V ADC3->JDR3
// #define I_BUS_DR ADC5->JDR3

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

#include "../../motorlib/boards/config_obot_g474_traceb.cpp"

void config_init() {}

void config_maintenance() {}