#ifndef MOTOR_CONTROL_FUN_H
#define MOTOR_CONTROL_FUN_H

#include "messages.h"

class PIController {
public:
    ~PIController() {}
    float step(float desired, float measured)  __attribute__((section (".ccmram")));;
    void set_param(const PIParam &pi_param);
private:
    float kp_ = 0, ki_ = 0, ki_sum_ = 0, ki_limit_ = 0, command_max_ = 0;

};


class PIDController {
public:
    virtual ~PIDController() {}
    virtual float step(float desired, float measured)  __attribute__((section (".ccmram")));;
    void set_param(const PIDParam &param);
private:
    float kp_ = 0, kd_ = 0, ki_ = 0, ki_sum_ = 0, ki_limit_ = 0, command_max_ = 0;
    float error_last_ = 0;

};


#endif //MOTOR_CONTROL_FUN_H
