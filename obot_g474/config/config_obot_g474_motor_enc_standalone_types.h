#ifndef UNHUMAN_OBOT_G474_CONFIG_CONFIG_OBOT_G474_MOTOR_ENC_STANDALONE_TYPES_H_
#define UNHUMAN_OBOT_G474_CONFIG_CONFIG_OBOT_G474_MOTOR_ENC_STANDALONE_TYPES_H_

// === Necessary for motor_enc_standalone. ===
#include "../../motorlib/ads1235_2.h"
#include "../../motorlib/encoder.h"
#include "../../motorlib/hardware_brake.h"
#include "../../motorlib/qep_encoder.h"

using MotorEncoderType = QEPEncoder;
using OutputEncoderType = EncoderBase;
using TorqueSensorType = ADS1235_2;
using HardwareBrakeType = HardwareBrakeBase;

// === Necessary for obot_g474_motor. ===
#include "../../motorlib/boards/config_obot_g474_motor_types.h"

#endif  // UNHUMAN_OBOT_G474_CONFIG_CONFIG_OBOT_G474_MOTOR_ENC_STANDALONE_TYPES_H_