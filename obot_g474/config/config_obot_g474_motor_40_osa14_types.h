#ifndef UNHUMAN_OBOT_G474_CONFIG_CONFIG_OBOT_G474_OSA14_TYPES_H_
#define UNHUMAN_OBOT_G474_CONFIG_CONFIG_OBOT_G474_OSA14_TYPES_H_

// === Necessary for osa14. ===
#include "../../motorlib/encoder.h"
#include "../../motorlib/hardware_brake.h"
#include "../../motorlib/peripheral/stm32g4/spi_dma.h"
// TODO: Fix includes. ads1235.h and icpz.h must be at end.
#include "../../motorlib/ads1235.h"
#include "../../motorlib/icpz.h"


using MotorEncoderType = ICPZ;
using OutputEncoderType = EncoderBase;
using TorqueSensorType = ADS1235;
using HardwareBrakeType = HardwareBrakeBase;

// === Necessary for obot_g474_motor. ===
#include "../../motorlib/boards/config_obot_g474_motor_40_types.h"

#endif  // UNHUMAN_OBOT_G474_CONFIG_CONFIG_OBOT_G474_OSA14_TYPES_H_