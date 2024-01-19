#include "../param/param_obot_g474.h"
#include "st_device.h"
#include "../../motorlib/torque_sensor.h"
#include "../../motorlib/gpio.h"
#include "../../motorlib/hall.h"
#include "../../motorlib/peripheral/stm32g4/pin_config.h"
#define COMMS   COMMS_USB

using TorqueSensor = TorqueSensorBase;
using MotorEncoder = HallEncoder;
using OutputEncoder = EncoderBase;


struct InitCode {
    InitCode() {
      // doing weird stuff registers directly
      GPIO_SETL(A, 0, GPIO::INPUT, GPIO_SPEED::VERY_HIGH, 0);
      GPIO_SETL(A, 1, GPIO::INPUT, GPIO_SPEED::VERY_HIGH, 0);
      GPIO_SETL(A, 2, GPIO::INPUT, GPIO_SPEED::VERY_HIGH, 0);
    }
};

namespace config {
    const uint32_t main_loop_frequency = 10000;    
    const uint32_t pwm_frequency = 50000;
    InitCode init_code;

    GPIO gpio_a(*GPIOA, 0);
    GPIO gpio_b(*GPIOA, 1);
    GPIO gpio_c(*GPIOA, 2);
    HallEncoder motor_encoder(gpio_a, gpio_b, gpio_c);
    TorqueSensor torque_sensor;
    EncoderBase output_encoder;
};

#include "../../motorlib/boards/config_obot_g474_motor.cpp"

void config_init() {

}

void config_maintenance() {}