#include "usb_communication.h"
#include "../st_device.h"
#include "qep_encoder.h"
#include "peripheral/stm32g4/hrpwm.h"
#include "peripheral/stm32g4/spi_encoder.h"
#include "peripheral/stm32g4/ams_encoder.h"
#include "hall.h"

extern const volatile Param initial_param;

static struct {
    QEPEncoder output_encoder = {*TIM5};
    // GPIO motor_encoder_cs = {*GPIOA, 15, GPIO::OUTPUT};
    // SPIEncoder motor_encoder = {*SPI3, motor_encoder_cs};
    GPIO hall_a = {*GPIOC, 0, GPIO::INPUT};
    GPIO hall_b = {*GPIOC, 1, GPIO::INPUT};
    GPIO hall_c = {*GPIOC, 2, GPIO::INPUT};
    HallEncoder motor_encoder = {hall_a, hall_b, hall_c};
    //AMSEncoder motor_encoder = {*SPI3, motor_encoder_cs};
    GPIO enable = {*GPIOC, 11, GPIO::OUTPUT};
    HRPWM motor_pwm = {static_cast<uint32_t>(initial_param.fast_loop_param.pwm_frequency), *HRTIM1, 3, 5, 4};
    FastLoop fast_loop = {motor_pwm, motor_encoder};
    LED led = {const_cast<uint16_t*>(reinterpret_cast<volatile uint16_t *>(&TIM4->CCR1)), 
               const_cast<uint16_t*>(reinterpret_cast<volatile uint16_t *>(&TIM4->CCR2)),
               const_cast<uint16_t*>(reinterpret_cast<volatile uint16_t *>(&TIM4->CCR3))};
    PIDDeadbandController controller = {1.0/10000};
    USBCommunication communication = {usb_};
    MainLoop main_loop = {controller, communication, led, output_encoder};
} config_items;
