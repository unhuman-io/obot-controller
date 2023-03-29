#ifndef UNHUMAN_OBOT_G474_CONFIG_CONFIG_OBOT_G474_MOTOR_ADS1235_2_TYPES_H_
#define UNHUMAN_OBOT_G474_CONFIG_CONFIG_OBOT_G474_MOTOR_ADS1235_2_TYPES_H_

// === Necessary for motor_ads1235_2. ===
// TODO: Fix includes. spi_dma.h must come before ads1235_2.h.
#include "../../motorlib/peripheral/stm32g4/spi_dma.h"
// spi_dma.h must come before ads1235_2.h.
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

#endif  // UNHUMAN_OBOT_G474_CONFIG_CONFIG_OBOT_G474_MOTOR_ADS1235_2_TYPES_H_