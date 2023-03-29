#ifndef UNHUMAN_FABULAB_F446_CONFIG_FABULAB_F446_TYPES_H_
#define UNHUMAN_FABULAB_F446_CONFIG_FABULAB_F446_TYPES_H_

// === Necessary for fabulab_f446 (instance). ===
#include "../motorlib/encoder.h"
#include "../motorlib/hardware_brake.h"
#include "../motorlib/phony_encoder.h"
#include "../motorlib/qep_encoder.h"
#include "../motorlib/torque_sensor.h"

using MotorEncoderType = PhonyEncoder;
using OutputEncoderType = QEPEncoder;
using TorqueSensorType = TorqueSensorBase;
using HardwareBrakeType = HardwareBrakeBase;

// === Necessary for fabulab_f446 (board). ===
#include "../motorlib/driver.h"
#include "../motorlib/peripheral/stm32f4/pwm_en.h"
#include "../motorlib/peripheral/stm32f4/otg_usb.h"
#include "../motorlib/usb_communication.h"

using CommunicationType = USBCommunication<USB_OTG>;
using DriverType = DriverBase;
using PWMType = PWM_EN;

// === Default type definitions. ===
#include "../motorlib/controller/impedance_controller.h"
#include "../motorlib/controller/joint_position_controller.h"
#include "../motorlib/controller/position_controller.h"
#include "../motorlib/controller/state_controller.h"
#include "../motorlib/controller/torque_controller.h"
#include "../motorlib/controller/velocity_controller.h"

using PositionControllerType = PositionController;
using TorqueControllerType = TorqueController;
using ImpedanceControllerType = ImpedanceController;
using VelocityControllerType = VelocityController;
using StateControllerType = StateController;
using JointPositionControllerType = JointPositionController;

#endif  // UNHUMAN_FABULAB_F446_CONFIG_FABULAB_F446_TYPES_H_