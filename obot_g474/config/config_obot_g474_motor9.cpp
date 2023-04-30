#include "../param/param_obot_g474.h"
#include "../st_device.h"
#include "../../motorlib/torque_sensor.h"
#include "../../motorlib/gpio.h"
#include "../../motorlib/peripheral/stm32g4/pin_config.h"
#include "../../motorlib/peripheral/stm32g4/hrpwm.h"
#include "../../motorlib/encoder.h"

using TorqueSensor = TorqueSensorBase;
using OutputEncoder = EncoderBase;

extern "C" void SystemClock_Config();
void pin_config_obot_g474_motor9_r0();

struct InitCode {
    InitCode() {
      SystemClock_Config();
      pin_config_obot_g474_motor9_r0();
    }
};

namespace config {
    const uint32_t main_loop_frequency = 10000;    
    const uint32_t pwm_frequency = 50000;
    InitCode init_code;

    TorqueSensor torque_sensor;
    EncoderBase output_encoder;
};

#include "../../motorlib/boards/config_obot_g474_motor9.cpp"

void config_init() {

}

void config_maintenance() {}