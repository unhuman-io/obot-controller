#pragma once
#include "../motorlib/control_fun.h"
#include <string>
#include "../motorlib/logger.h"

class TensionProgram {
 public:
    enum State {OFF, LOW_VELOCITY, START_VELOCITY, TORQUE, END, ERROR};
    const std::string state_string[5] = {"OFF", "LOW_VELOCITY", "START_VELOCITY", "TORQUE", "ERROR"};
    std::string get_state() const { return state_string[state_]; }
    void loop() {
        if (config::main_loop.first_command_received()) {
            // exit this program
            if (command_received_ == false) {
                System::log("received external command, tension program exiting");
                command_received_ = true;
            }
            return;
        }

        t_ms_++;
        t_ms_state_++;
        status_ = config::main_loop.get_status();
        config::gpio1.update();     // for deboucing
        config::gpio2.update();
        float dt = (status_.fast_loop.timestamp - last_timestamp_)*(1.0/CPU_FREQUENCY_HZ);
        float velocity = (status_.fast_loop.motor_position.position - last_motor_position_)/dt;
        last_timestamp_ = status_.fast_loop.timestamp;
        last_motor_position_ = status_.fast_loop.motor_position.position;
        float velocity_filtered = velocity_filter_.update(velocity);
        float torque_filtered = torque_filter_.update(status_.torque);

        MotorCommand command = {};
        if (state_ != last_state_) {
            t_ms_state_ = 0;
            last_state_ = state_;
        }
        switch (state_) {
            case OFF:
                logger.log_once("off");
                // todo sleep to save power
                command.mode_desired = OPEN;
                if (config::gpio1.is_clear()) { // button pushed
                    config::fast_loop.beep_on(0.25);
                    state_ = LOW_VELOCITY;
                    logger.log("button 1");
                }
                break;
            case LOW_VELOCITY:
                logger.log_once("low velocity");
                command.mode_desired = VELOCITY;
                command.velocity_desired = low_velocity;
                if (config::gpio2.is_clear()) { // button pushed
                    config::fast_loop.beep_on(0.25);
                    state_ = START_VELOCITY;
                    logger.log("button 2");
                }
                if (torque_filtered > start_torque || config::gpio1.is_set()) {
                    state_ = ERROR;
                    config::fast_loop.beep_on(2);
                    logger.log("error");
                }
                break;
            case START_VELOCITY:
                logger.log_once("start velocity");
                command.mode_desired = VELOCITY;
                command.velocity_desired = start_velocity;
                if (status_.torque > start_torque) {
                    config::fast_loop.beep_on(.5);
                    state_ = TORQUE;
                    torque_desired_current_ = start_torque;
                    logger.log("torque mode");
                }
                if (config::gpio1.is_set() || config::gpio2.is_set()) {
                    state_ = ERROR;
                    config::fast_loop.beep_on(2);
                    logger.log("error2");
                }
                break;
            case TORQUE:
                logger.log_once("torque");
                command.mode_desired = MotorMode::TORQUE;
                // A slope to try to cancel friction
                torque_desired_current_ += torque_vs_position_ramp*velocity*dt;
                torque_desired_current_ = fsat2(torque_desired_current_, min_torque_desired, torque_desired);
                command.torque_desired = torque_desired_current_;
                torque_dither_trajectory_.set_amplitude(dithering_torque);
                torque_dither_trajectory_.set_frequency(dithering_frequency_hz);
                command.torque_desired += torque_dither_trajectory_.step(.001).value;
                if (config::gpio1.is_set() || config::gpio2.is_set()) {
                    // done
                    logger.log("done");
                    command.mode_desired = CURRENT; // necessary to get beep
                    config::main_loop.set_command(command);
                    asm("NOP");
                    config::fast_loop.beep_on(.3);
                    asm("NOP");
                    ms_delay(500);
                    asm("NOP");
                    config::fast_loop.beep_on(.3);
                    asm("NOP");
                    ms_delay(500);
                    asm("NOP");
                    config::fast_loop.beep_on(.3);
                    asm("NOP");
                    state_ = END;
                }
                break;
            case END:
                logger.log_once("end");
            case ERROR:
                command.mode_desired = CURRENT; // necessary to get beep
                if (t_ms_state_ > 2000) {
                    state_ = OFF;
                    logger.log("exit done or error");
                }
                break;
            default:
                command.mode_desired = OPEN;
                logger.log_once("open");
                break;
        }

        config::main_loop.set_command(command);
    }
 private:
    uint64_t t_ms_ = 0;
    uint64_t t_ms_state_ = 0;
    MainLoopStatus status_;
    bool command_received_ = false;
    float last_motor_position_ = 0;
    float last_timestamp_ = 0;
    FirstOrderLowPassFilter velocity_filter_ = {.001, 100};
    FirstOrderLowPassFilter torque_filter_ = {.001, 100};
    TrajectoryGenerator torque_dither_trajectory_;
    State state_ = OFF;
    State last_state_ = OFF;
    volatile float low_velocity = 20;
    volatile float start_velocity = 30;
    volatile float torque_desired = 5;
    volatile float start_torque = 4;
    volatile float dithering_torque = .2;
    volatile float dithering_frequency_hz = 10;
    volatile float torque_vs_position_ramp = .1;
    volatile float min_torque_desired = 2;
    float torque_desired_current_ = start_torque;
    friend class InitCode2;
};
