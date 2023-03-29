#include "../../motorlib/gpio.h"
#include "../../motorlib/peripheral/stm32g4/pin_config.h"
#include "../param/param_obot_g474.h"
#include "../st_device.h"
#include "config_obot_g474_motor_hall_types.h"

extern "C" void SystemClock_Config();
void pin_config_obot_g474_motor_r0();

struct InitCode {
  InitCode() {
    SystemClock_Config();
    pin_config_obot_g474_motor_r0();

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
TorqueSensorBase torque_sensor;
EncoderBase output_encoder;
};  // namespace config

#include "../../motorlib/boards/config_obot_g474_motor.cpp"

void config_init() {}

void config_maintenance() {}