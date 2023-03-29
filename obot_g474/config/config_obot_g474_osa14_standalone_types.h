#ifndef UNHUMAN_OBOT_G474_CONFIG_CONFIG_OBOT_G474_OSA14_STANDALONE_TYPES_H_
#define UNHUMAN_OBOT_G474_CONFIG_CONFIG_OBOT_G474_OSA14_STANDALONE_TYPES_H_

// === Necessary for osa14_standalone. ===
#include "../../motorlib/encoder.h"
#include "../../motorlib/hardware_brake.h"
#include "../../motorlib/peripheral/stm32g4/spi_dma.h"
#include "../../motorlib/peripheral/stm32g4/spi_torque.h"
#include "../../motorlib/sensor_multiplex.h"
// TODO: Fix includes. Encoders must be at the end.
#include "../../motorlib/ma732_encoder.h"
#include "../../motorlib/ma782_encoder.h"


using MotorEncoderType = MA782Encoder;
using OutputEncoderType = TorqueSensorMultiplex<SPITorque, MA782Encoder>::SecondarySensor;
using TorqueSensorType = TorqueSensorMultiplex<SPITorque, MA782Encoder>;
using HardwareBrakeType = HardwareBrakeBase;

// === Necessary for obot_g474_motor. ===
#include "../../motorlib/boards/config_obot_g474_osa_types.h"

#endif  // UNHUMAN_OBOT_G474_CONFIG_CONFIG_OBOT_G474_OSA14_STANDALONE_TYPES_H_