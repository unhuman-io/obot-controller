#include "../param/param_obot_g474_can.h"
#include "st_device.h"
#include "../../motorlib/encoder.h"
#include "../../motorlib/torque_sensor.h"


#define COMMS   COMMS_CAN

using TorqueSensor = TorqueSensorBase;
using MotorEncoder = EncoderBase;
using OutputEncoder = EncoderBase;

struct InitCode {
    InitCode() {}
};

namespace config {
    const uint32_t main_loop_frequency = 10000;    
    const uint32_t pwm_frequency = 30000;
    InitCode init_code;

    TorqueSensor torque_sensor;
    OutputEncoder output_encoder;
    MotorEncoder motor_encoder;
};

#include "../../motorlib/boards/config_obot_g474_trace.cpp"

void config_init() {}

void config_maintenance() {}