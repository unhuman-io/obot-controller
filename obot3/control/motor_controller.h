#pragma once
// The main loop's controllers, from motorlib/controller and the mode handling
// of motorlib/main_loop.h: a host command (obot::MotorCommand) selects the
// mode, the controller of that mode turns the measurements into a current,
// and the result is the fast loop's FOC command.
#include <cstdint>
#include "../motorlib/motor_messages/motor_messages.h"
#include "foc.h"

struct MotorControllerParam {
    PIDParam position{};                        // rad -> A
    float position_velocity_limit = 0;          // rad/s on the desired position, 0: none
    float position_desired_filter_hz = 0;
    PIDParam velocity{};                        // rad/s -> A
    float acceleration_limit = 0;               // rad/s^2 on the desired velocity, 0: none
    PIDParam torque{};                          // Nm -> A
    PIDParam impedance{};                       // rad -> Nm, kp and kd are the default stiffness and damping
    float rollover = 0;                         // rad: the motor position wraps at +-rollover, 0: it doesn't
};

template<MotorControllerParam param, float dt>
class PositionController {
 public:
    constexpr PositionController() = default;
    void init(float motor_position) {
        controller_.init(motor_position);
        desired_filter_.init(motor_position);
    }
    float step(const obot::MotorCommand& command, float motor_position) {
        const float position_desired = desired_filter_.update(command.position_desired);
        return controller_.step(position_desired, command.velocity_desired, motor_position, velocity_limit) + command.current_desired;
    }
 private:
    static constexpr float velocity_limit = param.position_velocity_limit == 0 ? control_infinity : param.position_velocity_limit;
    PIDController controller_{dt, param.position};
    SecondOrderLowPassFilter desired_filter_{dt, param.position_desired_filter_hz};
};

template<MotorControllerParam param, float dt>
class VelocityController {
 public:
    constexpr VelocityController() = default;
    void init(float motor_position) {
        controller_.init(0);    // can only switch into this mode at zero velocity without glitch
        last_motor_position_ = motor_position;
    }
    float step(const obot::MotorCommand& command, float motor_position) {
        const float difference = param.rollover == 0 ? motor_position - last_motor_position_ :
            wrap1_diff(motor_position, last_motor_position_, param.rollover);
        last_motor_position_ = motor_position;
        const float velocity_measured = velocity_filter_.update(difference / dt);
        return controller_.step(command.velocity_desired, 0, velocity_measured, acceleration_limit) + command.current_desired;
    }
 private:
    static constexpr float acceleration_limit = param.acceleration_limit == 0 ? control_infinity : param.acceleration_limit;
    float last_motor_position_ = 0;
    PIDController controller_{dt, param.velocity};
    FirstOrderLowPassFilter velocity_filter_{dt, param.velocity.velocity_filter_frequency_hz};
};

template<MotorControllerParam param, float dt>
class TorqueController {
 public:
    constexpr TorqueController() = default;
    void init(float torque) { controller_.init(torque); }
    float step(const obot::MotorCommand& command, float torque) {
        return controller_.step(command.torque_desired, 0, torque) + command.current_desired;
    }
    float step(float torque_desired, float torque_dot_desired, float torque) {
        return controller_.step(torque_desired, torque_dot_desired, torque);
    }
 private:
    PIDController controller_{dt, param.torque};
};

// Stiffness and damping around a position give a torque, which the torque
// controller turns into a current
template<MotorControllerParam param, float dt>
class ImpedanceController {
 public:
    constexpr ImpedanceController() = default;
    void init(float motor_position, float torque) {
        impedance_controller_.init(motor_position);
        torque_controller_.init(torque);
        last_torque_desired_ = 0;
    }
    float step(const obot::MotorCommand& command_raw, float motor_position, float torque) {
        const obot::ImpedanceCommand& command = command_raw.impedance;
        impedance_controller_.kp_ = command.stiffness == 0 ? param.impedance.kp : command.stiffness;
        impedance_controller_.kd_ = command.damping == 0 ? param.impedance.kd : command.damping;
        const float torque_desired = impedance_controller_.step(command.position_desired, command.velocity_desired, motor_position) +
            command.torque_desired;
        const float torque_dot_desired = (torque_desired - last_torque_desired_) / dt + command.torque_dot_desired;
        last_torque_desired_ = torque_desired;
        return torque_controller_.step(torque_desired, torque_dot_desired, torque) + command.current_desired;
    }
 private:
    PIDController impedance_controller_{dt, param.impedance};
    TorqueController<param, dt> torque_controller_{};
    float last_torque_desired_ = 0;
};

// The floats a mode uses must be finite for a command to be taken
constexpr bool motor_command_valid(const obot::MotorCommand& command) {
    switch (command.mode_desired) {
        case obot::CURRENT:
        case obot::PHASE_LOCK:
            return is_finite(command.current_desired);
        case obot::POSITION:
            return is_finite(command.position_desired) && is_finite(command.velocity_desired) && is_finite(command.current_desired);
        case obot::VELOCITY:
            return is_finite(command.velocity_desired) && is_finite(command.current_desired);
        case obot::TORQUE:
            return is_finite(command.torque_desired) && is_finite(command.current_desired);
        case obot::IMPEDANCE:
            return is_finite(command.impedance.position_desired) && is_finite(command.impedance.velocity_desired) &&
                is_finite(command.impedance.torque_desired) && is_finite(command.impedance.current_desired) &&
                is_finite(command.impedance.torque_dot_desired) && is_finite(command.impedance.stiffness) &&
                is_finite(command.impedance.damping);
        case obot::VOLTAGE:
            return is_finite(command.voltage.voltage_desired);
        default:
            return true;
    }
}

// The main loop's controller policy. HostCommand::read() is the newest
// obot::MotorCommand from the host. The status needs
//   fast_loop.measured: timestamp, motor_position, motor_velocity, motor_encoder
//   fast_loop.controller: a FOCStatus
//   sensors: output_position, torque
// Modes: open, damped, current, position, velocity, torque, impedance,
// voltage and phase lock. Any other mode is open. An invalid command is
// dropped and the previous one stays.
template<MotorControllerParam param, float dt, typename HostCommand>
struct MotorControllerPolicy {
    template<typename Status>
    typename Status::ExpectedCommand update(const Status& s) {
        const obot::MotorCommand host_command = HostCommand::read();
        if (motor_command_valid(host_command)) {
            command = host_command;
        } else {
            invalid_command_count = invalid_command_count + 1;
        }

        const float motor_position = s.fast_loop.measured.motor_position;
        const float torque = s.sensors.torque;
        if (command.mode_desired != mode) {
            switch (command.mode_desired) {
                case obot::POSITION: position_controller.init(motor_position); break;
                case obot::VELOCITY: velocity_controller.init(motor_position); break;
                case obot::TORQUE: torque_controller.init(torque); break;
                case obot::IMPEDANCE: impedance_controller.init(motor_position, torque); break;
            }
            mode = command.mode_desired;
        }

        FOCCommand foc{};
        switch (mode) {
            case obot::DAMPED:
                foc.mode = FOCMode::BRAKE;
                break;
            case obot::CURRENT:
                foc.mode = FOCMode::CURRENT;
                foc.i_q = command.current_desired;
                break;
            case obot::POSITION:
                foc.mode = FOCMode::CURRENT;
                foc.i_q = position_controller.step(command, motor_position);
                break;
            case obot::VELOCITY:
                foc.mode = FOCMode::CURRENT;
                foc.i_q = velocity_controller.step(command, motor_position);
                break;
            case obot::TORQUE:
                foc.mode = FOCMode::CURRENT;
                foc.i_q = torque_controller.step(command, torque);
                break;
            case obot::IMPEDANCE:
                foc.mode = FOCMode::CURRENT;
                foc.i_q = impedance_controller.step(command, motor_position, torque);
                break;
            case obot::VOLTAGE:
                foc.mode = FOCMode::VOLTAGE;
                foc.v_q = command.voltage.voltage_desired;
                break;
            case obot::PHASE_LOCK:
                foc.mode = FOCMode::PHASE_LOCK;
                foc.i_d = command.current_desired;
                break;
            default:    // open, and the modes that aren't here
                foc.mode = FOCMode::OPEN;
                break;
        }

        // for the host, in the obot status layout
        obot::MotorStatusRegular host_status{};
        host_status.mcu_timestamp = s.fast_loop.measured.timestamp;
        host_status.host_timestamp_received = command.host_timestamp;
        host_status.motor_position = motor_position;
        host_status.joint_position = s.sensors.output_position;
        host_status.iq = s.fast_loop.controller.i_q;
        host_status.torque = torque;
        host_status.flags.mode = foc.mode == FOCMode::OPEN ? static_cast<uint8_t>(obot::OPEN) : mode;
        host_status.iq_desired = foc.i_q;
        host_status.motor_velocity = s.fast_loop.measured.motor_velocity;
        host_status.motor_encoder = s.fast_loop.measured.motor_encoder;
        status.write(host_status);

        return {foc};
    }

    obot::MotorCommand command{};
    uint8_t mode = obot::OPEN;
    volatile uint32_t invalid_command_count = 0;
    PositionController<param, dt> position_controller{};
    VelocityController<param, dt> velocity_controller{};
    TorqueController<param, dt> torque_controller{};
    ImpedanceController<param, dt> impedance_controller{};
    Latest<obot::MotorStatusRegular> status{};
};
