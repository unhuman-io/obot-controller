#include "../param/param_obot_g474_can.h"
#include "st_device.h"
#include "../../motorlib/encoder.h"
#include "../../motorlib/torque_sensor.h"
#include "../../motorlib/gpio.h"
#include <algorithm>
#include "../../motorlib/peripheral/stm32g4/pin_config.h"
#define COMMS   COMMS_CAN
#define CAN_NUM CAN::CAN3

using TorqueSensor = TorqueSensorBase;
using MotorEncoder = EncoderBase;
using OutputEncoder = EncoderBase;

struct InitCode {
    InitCode() {
        GPIO_SETL(B, 3, GPIO_MODE::ALT_FUN, GPIO_SPEED::VERY_HIGH, 11); // can3 rx
        GPIO_SETL(B, 4, GPIO_MODE::ALT_FUN, GPIO_SPEED::VERY_HIGH, 11); // can3 tx

        RCC->APB1ENR1 |= RCC_APB1ENR1_FDCANEN;

        
        
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

void config_init() {
    System::api.add_api_variable("can_intrs", new APIUint32(&config::can.interrupt_called_));
}

void config_maintenance() {}