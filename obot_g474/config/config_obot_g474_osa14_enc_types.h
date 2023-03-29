#ifndef UNHUMAN_OBOT_G474_CONFIG_CONFIG_OBOT_G474_OSA14_ENC_TYPES_H_
#define UNHUMAN_OBOT_G474_CONFIG_CONFIG_OBOT_G474_OSA14_ENC_TYPES_H_

// === Necessary for osa14_enc. ===
#include "../../motorlib/encoder.h"
#include "../../motorlib/hardware_brake.h"
#include "../../motorlib/peripheral/stm32g4/spi_dma.h"
#include "../../motorlib/peripheral/stm32g4/spi_torque.h"
#include "../../motorlib/ma732_encoder.h"
#include "../../motorlib/qep_encoder.h"



using MotorEncoderType = QEPEncoder;
using OutputEncoderType = MA732Encoder;
using TorqueSensorType = SPITorque;
using HardwareBrakeType = HardwareBrakeBase;

// === Necessary for obot_g474_motor. ===
#include "../../motorlib/boards/config_obot_g474_osa_types.h"

#endif  // UNHUMAN_OBOT_G474_CONFIG_CONFIG_OBOT_G474_OSA14_ENC_TYPES_H_