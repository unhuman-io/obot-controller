#ifndef UNHUMAN_OBOT_G474_CONFIG_CONFIG_OBOT_G474_MOTOR_WHEEL_TYPES_H_
#define UNHUMAN_OBOT_G474_CONFIG_CONFIG_OBOT_G474_MOTOR_WHEEL_TYPES_H_

// === Necessary for motor_wheel. ===
#include "../../motorlib/encoder.h"
#include "../../motorlib/hardware_brake.h"
// TODO: Fix includes. spi_dma.h must be included before icpz.h
#include "../../motorlib/peripheral/stm32g4/spi_dma.h"
// spi_dma.h must be included before icpz.h
#include "../../motorlib/icpz.h"
#include "../../motorlib/torque_sensor.h"

using MotorEncoderType = ICPZ;
using OutputEncoderType = EncoderBase;
using TorqueSensorType = TorqueSensorBase;
using HardwareBrakeType = HardwareBrakeBase;

// === Necessary for obot_g474_motor. ===
#include "../../motorlib/boards/config_obot_g474_motor_types.h"

#endif  // UNHUMAN_OBOT_G474_CONFIG_CONFIG_OBOT_G474_MOTOR_WHEEL_TYPES_H_