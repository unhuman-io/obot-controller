#ifndef UNHUMAN_OBOT_G474_CONFIG_CONFIG_OBOT_G474_H2_TYPES_H_
#define UNHUMAN_OBOT_G474_CONFIG_CONFIG_OBOT_G474_H2_TYPES_H_

// === Necessary for h2. ===
#include "../../motorlib/encoder.h"
#include "../../motorlib/hardware_brake.h"
#include "../../motorlib/peripheral/stm32g4/spi_dma.h"
#include "../../motorlib/torque_sensor.h"
// TODO: Fix includes. Encoders must be at the end.
#include "../../motorlib/ma782_encoder.h"


using MotorEncoderType = MA782Encoder;
using OutputEncoderType = EncoderBase;
using TorqueSensorType = TorqueSensorBase;
using HardwareBrakeType = HardwareBrakeBase;

// === Necessary for obot_g474_h2. ===
#include "../../motorlib/driver_mps.h"
#include "../../motorlib/peripheral/stm32g4/hrpwm.h"
#include "../../motorlib/peripheral/usb.h"
#include "../../motorlib/usb_communication.h"

using CommunicationType = USBCommunication<USB1>;
using DriverType = DriverMPS;
using PWMType = HRPWM3;

// === Default type definitions. ===
#include "../../motorlib/controller/impedance_controller.h"
#include "../../motorlib/controller/joint_position_controller.h"
#include "../../motorlib/controller/position_controller.h"
#include "../../motorlib/controller/state_controller.h"
#include "../../motorlib/controller/torque_controller.h"
#include "../../motorlib/controller/velocity_controller.h"

using PositionControllerType = PositionController;
using TorqueControllerType = TorqueController;
using ImpedanceControllerType = ImpedanceController;
using VelocityControllerType = VelocityController;
using StateControllerType = StateController;
using JointPositionControllerType = JointPositionController;

#endif  // UNHUMAN_OBOT_G474_CONFIG_CONFIG_OBOT_G474_H2_TYPES_H_