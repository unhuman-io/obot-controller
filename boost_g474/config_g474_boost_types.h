#ifndef UNHUMAN_BOOST_G474_CONFIG_G474_BOOST_TYPES_H_
#define UNHUMAN_BOOST_G474_CONFIG_G474_BOOST_TYPES_H_

// === Necessary for boost_g474 (board). ===
#include "../motorlib/driver.h"
#include "../motorlib/peripheral/stm32g4/hrpwm.h"
#include "../motorlib/peripheral/usb.h"
#include "../motorlib/usb_communication.h"

using CommunicationType = USBCommunication<USB1>;
using DriverType = DriverBase;
using PWMType = HRPWM;

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

#endif  // UNHUMAN_BOOST_G474_CONFIG_G474_BOOST_TYPES_H_