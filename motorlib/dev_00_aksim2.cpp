#include "aksim2_encoder.h"
#include "usb_communication.h"
#include "../st_device.h"
#include "qep_encoder.h"
#include "peripheral/stm32g4/hrpwm.h"

extern const volatile Param initial_param;

static struct {
    //GPIO motor_encoder_cs = {*GPIOA, 15, GPIO::OUTPUT};
    //Aksim2Encoder motor_encoder = {*SPI3, motor_encoder_cs};
    QEPEncoder motor_encoder = {*TIM5};
    GPIO enable = {*GPIOC, 11, GPIO::OUTPUT};
    HRPWM motor_pwm = {initial_param.fast_loop_param.pwm_frequency, *HRTIM1, 3, 5, 4};
    FastLoop fast_loop = {motor_pwm, motor_encoder};
    LED led = {const_cast<uint16_t*>(reinterpret_cast<volatile uint16_t *>(&TIM3->CCR1)), 
               const_cast<uint16_t*>(reinterpret_cast<volatile uint16_t *>(&TIM3->CCR2)),
               const_cast<uint16_t*>(reinterpret_cast<volatile uint16_t *>(&TIM3->CCR4)), true};
    PIDController controller;
    USBCommunication communication = {usb_};
    MainLoop main_loop = {controller, communication, led};
} config_items;
