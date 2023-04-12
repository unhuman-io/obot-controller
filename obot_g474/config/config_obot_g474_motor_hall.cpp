#include "../param/param_obot_g474.h"
#include "../st_device.h"
#include "../../motorlib/torque_sensor.h"
#include "../../motorlib/gpio.h"
#include "../../motorlib/hall.h"
#include "../../motorlib/peripheral/stm32g4/pin_config.h"
#include "../../motorlib/peripheral/stm32g4/hrpwm.h"

using TorqueSensor = TorqueSensorBase;
using MotorEncoder = HallEncoder;
using OutputEncoder = EncoderBase;
using PWM = HRPWM3;

extern "C" void SystemClock_Config();
void pin_config_obot_g474_motor_r0();

struct InitCode {
    InitCode() {
      SystemClock_Config();
      pin_config_obot_g474_motor_r0();

              // driver fault and enable
        GPIO_SETL(B, 2, GPIO_MODE::OUTPUT, GPIO_SPEED::LOW, 0);  // drv enable
        GPIO_SETL(B, 1, GPIO_MODE::INPUT, GPIO_SPEED::LOW, 0);  // drv fault
        MASK_SET(GPIOB->PUPDR, GPIO_PUPDR_PUPD1, GPIO_PULL::UP);
        GPIOB->BSRR = GPIO_BSRR_BS2;
      GPIO_SETH(B, 14, GPIO_MODE::ALT_FUN, GPIO_SPEED::VERY_HIGH, 13); // hrtim1 chd1
        GPIO_SETH(B, 15, GPIO_MODE::ALT_FUN, GPIO_SPEED::VERY_HIGH, 13); // hrtim1 chd2
        GPIO_SETH(A, 8, GPIO_MODE::ALT_FUN, GPIO_SPEED::VERY_HIGH, 13);  // hrtim1 cha1
        GPIO_SETH(A, 9, GPIO_MODE::ALT_FUN, GPIO_SPEED::VERY_HIGH, 13);  // hrtim1 cha2, pwm enable is managed through hrtim



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

    HRPWM3 motor_pwm(pwm_frequency, *HRTIM1, 3, 3, 0, 1000, 0);

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