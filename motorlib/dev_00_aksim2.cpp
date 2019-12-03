#include "aksim2_encoder.h"
#include "usb_communication.h"
#include "../st_device.h"

extern const volatile Param initial_param;

static struct {
    //GPIO motor_encoder_cs = {*GPIOA, 15, GPIO::OUTPUT};
    //Aksim2Encoder motor_encoder = {*SPI3, motor_encoder_cs};
    Encoder motor_encoder = {reinterpret_cast<volatile int32_t *>(&TIM5->CNT)};
    GPIO enable = {*GPIOC, 11, GPIO::OUTPUT};
    PWM motor_pwm = {initial_param.fast_loop_param.pwm_frequency, *const_cast<uint32_t*>(&HRTIM1_TIMD->CMP1xR), 
                          *const_cast<uint32_t*>(&HRTIM1_TIMF->CMP1xR), 
                          *const_cast<uint32_t*>(&HRTIM1_TIME->CMP1xR),
                          *TIM8, enable};
    FastLoop fast_loop = {motor_pwm, motor_encoder};
    LED led = {const_cast<uint16_t*>(reinterpret_cast<volatile uint16_t *>(&TIM3->CCR1)), 
               const_cast<uint16_t*>(reinterpret_cast<volatile uint16_t *>(&TIM3->CCR2)),
               const_cast<uint16_t*>(reinterpret_cast<volatile uint16_t *>(&TIM3->CCR4)), true};
    PIDController controller;
    USBCommunication communication;
    MainLoop main_loop = {controller, communication, led};
} config_items;
