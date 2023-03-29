#ifndef UNHUMAN_OBOT_G474_CONFIG_CONFIG_OBOT_G474_MOTOR_AKSIM_TYPES_H_
#define UNHUMAN_OBOT_G474_CONFIG_CONFIG_OBOT_G474_MOTOR_AKSIM_TYPES_H_

// === Necessary for motor_aksim. ===
// TODO: Fix includes. spi_dma.h must come before aksim2_encoder.h.
#include "../../motorlib/peripheral/stm32g4/spi_dma.h"
// spi_dma.h must come before aksim2_encoder.h.
#include "../../motorlib/aksim2_encoder.h"
#include "../../motorlib/encoder.h"
#include "../../motorlib/hardware_brake.h"
#include "../../motorlib/motor_messages/motor_messages.h"
#include "../../motorlib/peripheral/stm32g4/qia128_uart.h"
#include "../../motorlib/sensor_multiplex.h"
#include "../../motorlib/torque_sensor.h"

#ifndef MOTOR_ENCODER_BITS
#define MOTOR_ENCODER_BITS 18
#endif

#ifndef OUTPUT_ENCODER_BITS
#define OUTPUT_ENCODER_BITS 18
#endif

// using MotorEncoderType = EncoderBase;
using MotorEncoderType = Aksim2Encoder<MOTOR_ENCODER_BITS>;
// using TorqueSensorType = TorqueSensorBase;
using TorqueSensorType = QIA128_UART;
// using TorqueSensorType = TorqueSensorMultiplex<QIA128, Aksim2Encoder<18>>;
using HardwareBrakeType = HardwareBrakeBase;
// using OutputEncoderType = EncoderBase;
using OutputEncoderType = Aksim2Encoder<OUTPUT_ENCODER_BITS>;
// using OutputEncoderType = TorqueSensorMultiplex<QIA128,
// Aksim2Encoder<18>>::SecondarySensor;

// === Necessary for obot_g474_motor. ===
#include "../../motorlib/boards/config_obot_g474_motor_types.h"

#endif  // UNHUMAN_OBOT_G474_CONFIG_CONFIG_OBOT_G474_MOTOR_AKSIM_TYPES_H_