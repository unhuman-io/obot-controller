#ifndef UNHUMAN_OBOT_G474_CONFIG_CONFIG_OBOT_G474_MOTOR_QIA128_SPI_TYPES_H_
#define UNHUMAN_OBOT_G474_CONFIG_CONFIG_OBOT_G474_MOTOR_QIA128_SPI_TYPES_H_

// === Necessary for motor_qia128_spi. ===
#include "../../motorlib/aksim2_encoder.h"
#include "../../motorlib/encoder.h"
#include "../../motorlib/hardware_brake.h"
#include "../../motorlib/qia128.h"
#include "../../motorlib/sensor_multiplex.h"
#include "../../motorlib/torque_sensor.h"


// using MotorEncoderType = EncoderBase;
using MotorEncoderType = Aksim2Encoder<18>;
// using TorqueSensorType = TorqueSensorBase;
// using TorqueSensorType = QIA128;
using TorqueSensorType = TorqueSensorMultiplex<QIA128, Aksim2Encoder<18>>;
using HardwareBrakeType = HardwareBrakeBase;
// using OutputEncoderType = EncoderBase;
// using OutputEncoderType = Aksim2Encoder<18>;
using OutputEncoderType = TorqueSensorMultiplex<QIA128, Aksim2Encoder<18>>::SecondarySensor;

// === Necessary for obot_g474_motor. ===
#include "../../motorlib/boards/config_obot_g474_motor_types.h"

#endif  // UNHUMAN_OBOT_G474_CONFIG_CONFIG_OBOT_G474_MOTOR_QIA128_SPI_TYPES_H_