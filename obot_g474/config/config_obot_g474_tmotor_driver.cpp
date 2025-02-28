#include "../param/param_obot_g474_can.h"
#include "st_device.h"
#include "../../motorlib/encoder.h"
#include "../../motorlib/torque_sensor.h"
#include "../../motorlib/gpio.h"
#include <algorithm>
#include "../../motorlib/peripheral/stm32g4/pin_config.h"
#define COMMS   COMMS_CAN_USB
#define CAN_ARB_DATA_RATE CAN::ArbitrationBaudRate::ARB_2M, CAN::DataBaudRate::DATA_10M
#define CAN_NUM CAN::CAN3

using TorqueSensor = TorqueSensorBase;
using MotorEncoder = EncoderBase;
using OutputEncoder = EncoderBase;

// note can: sudo ip link set can0 up type can bitrate 2000000 dbitrate 10000000 fd on one-shot on restart-ms 100

struct InitCode {
    InitCode() {
        GPIO_SETH(A, 8, GPIO_MODE::ALT_FUN, GPIO_SPEED::VERY_HIGH, 11); // can3 rx
        GPIO_SETL(B, 4, GPIO_MODE::ALT_FUN, GPIO_SPEED::VERY_HIGH, 11); // can3 tx

        RCC->APB1ENR1 |= RCC_APB1ENR1_FDCANEN;

      // if defined(INTERFACE_BBS)
      // PA0 TCS
      GPIO_SETL(A, 0, GPIO_MODE::OUTPUT, GPIO_SPEED::MEDIUM, 0); 
      GPIOA->BSRR = GPIO_BSRR_BS0;
      GPIOC->BSRR = GPIO_BSRR_BS3;  // hdr17 (1CS2), output encoder cs
        
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