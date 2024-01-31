#include "../param/param_obot_g474.h"
#include "st_device.h"
#include "../../motorlib/encoder.h"
#include "../../motorlib/torque_sensor.h"
#include "../../motorlib/gpio.h"
#include <algorithm>
#include "../../motorlib/peripheral/stm32g4/pin_config.h"
#define COMMS   COMMS_UART
#define COMMS_UART_BAUDRATE 115200
#define COMMS_UART_NUMBER 2

using TorqueSensor = TorqueSensorBase;
using MotorEncoder = EncoderBase;
using OutputEncoder = EncoderBase;

struct InitCode {
    InitCode() {
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

#include "../../motorlib/boards/config_obot_g474_motor.cpp"

void config_init() {}

void config_maintenance() {}