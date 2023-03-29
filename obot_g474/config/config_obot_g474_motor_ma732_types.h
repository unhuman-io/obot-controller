#ifndef UNHUMAN_OBOT_G474_CONFIG_CONFIG_OBOT_G474_MOTOR_MA732_TYPES_H_
#define UNHUMAN_OBOT_G474_CONFIG_CONFIG_OBOT_G474_MOTOR_MA732_TYPES_H_

// === Necessary for motor_ma732. ===
#include "../../motorlib/encoder.h"
#include "../../motorlib/hardware_brake.h"
#include "../../motorlib/ma732_encoder.h"
#include "../../motorlib/torque_sensor.h"

using MotorEncoderType = MA732Encoder;
using OutputEncoderType = EncoderBase;
using TorqueSensorType = TorqueSensorBase;
using HardwareBrakeType = HardwareBrakeBase;

// === Necessary for obot_g474_motor. ===
#include "../../motorlib/boards/config_obot_g474_motor_types.h"

#endif  // UNHUMAN_OBOT_G474_CONFIG_CONFIG_OBOT_G474_MOTOR_MA732_TYPES_H_