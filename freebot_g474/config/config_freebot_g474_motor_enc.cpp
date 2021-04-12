#include "../param/param_freebot_g474.h"
#include "../st_device.h"
#include "../../motorlib/qep_encoder.h"
#include "../../motorlib/torque_sensor.h"
#include "../../motorlib/gpio.h"

using TorqueSensor = TorqueSensorBase;
using MotorEncoder = QEPEncoder;
using OutputEncoder = EncoderBase;

extern "C" void SystemClock_Config();
void pin_config_freebot_g474_motor_r0();

struct InitCode {
    InitCode() {
      SystemClock_Config();
      pin_config_freebot_g474_motor_r0();
    }
};

namespace config {
    const uint32_t main_loop_frequency = 10000;    
    const uint32_t pwm_frequency = 50000;
    InitCode init_code;

    QEPEncoder motor_encoder = {*TIM2};
    TorqueSensor torque_sensor;
    OutputEncoder output_encoder;
};

#include "config_freebot_g474_motor.cpp"

void config_init() {}
void config_maintenance() {}