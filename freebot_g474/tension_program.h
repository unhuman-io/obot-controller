#pragma once
#include "../motorlib/control_fun.h"

class TensionProgram {
 public:
    enum State {OFF, LOW_VELOCITY, START_VELOCITY, TORQUE};
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
        status_ = config::main_loop.get_status();
        float dt = (status_.fast_loop.timestamp - last_timestamp_)*(1.0/CPU_FREQUENCY_HZ);
        float velocity = (status_.fast_loop.motor_position.position - last_motor_position_)/dt;
        last_timestamp_ = status_.fast_loop.timestamp;
        last_motor_position_ = status_.fast_loop.motor_position.position;
        float velocity_filtered = velocity_filter_.update(velocity);

        MotorCommand command = {};
        if (t_ms_ > 3000) {
            command.mode_desired = VELOCITY;
            command.velocity_desired = 10;
        } 
        if (config::gpio1.is_clear()) { // button pushed
            command.mode_desired = VELOCITY;
            command.velocity_desired = 200;
        }
        // if (t_ms_ > 12000) {
        //     command.mode_desired = MotorMode::TORQUE;
        //     if (velocity_filtered > 0) {
        //         command.torque_desired = 3;
        //     } else {
        //         command.torque_desired = 5;
        //     }

        // }
        // if (t_ms_ > 24000) {
        //     command.mode_desired = OPEN;
        // }

        config::main_loop.set_command(command);
    }
 private:
    uint64_t t_ms_ = 0;
    MainLoopStatus status_;
    bool command_received_ = false;
    float last_motor_position_ = 0;
    float last_timestamp_ = 0;
    FirstOrderLowPassFilter velocity_filter_ = {.001, 100};
};
