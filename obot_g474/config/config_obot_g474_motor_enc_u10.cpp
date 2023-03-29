#include "../../motorlib/gpio.h"
#include "../../motorlib/ma732_encoder.h"
#include "../../motorlib/qep_encoder.h"
#include "../../motorlib/torque_sensor.h"
#include "../param/param_obot_g474.h"
#include "../st_device.h"

using TorqueSensor = TorqueSensorBase;
using MotorEncoder = QEPEncoder;
using OutputEncoder = QEPEncoder;

extern "C" void SystemClock_Config();
void pin_config_obot_g474_motor_r0();

struct InitCode {
  InitCode() {
    SystemClock_Config();
    pin_config_obot_g474_motor_r0();
    SPI3->CR2 = (15 << SPI_CR2_DS_Pos);  // 16 bit
    // ORDER DEPENDANCE SPE set last
    SPI3->CR1 = SPI_CR1_MSTR | (3 << SPI_CR1_BR_Pos) | SPI_CR1_SSI |
                SPI_CR1_SSM | SPI_CR1_SPE;  // baud = clock/16
  }
};

namespace config {
const uint32_t main_loop_frequency = 10000;
const uint32_t pwm_frequency = 50000;
InitCode init_code;

QEPEncoder motor_encoder(*TIM2);
TorqueSensor torque_sensor;
QEPEncoder output_encoder(*TIM2);
};  // namespace config

#include "../../motorlib/boards/config_obot_g474_motor.cpp"

void config_init() {
  System::api.add_api_variable(
      "index_count", new APIUint32(&config::motor_encoder.index_count_));
}

void config_maintenance() {}