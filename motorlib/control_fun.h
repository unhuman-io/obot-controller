#ifndef MOTOR_CONTROL_FUN_H
#define MOTOR_CONTROL_FUN_H

#include "messages.h"
#undef _DEFAULT_SOURCE
#include <cmath>
#define M_PI 3.1415926f
class Hysteresis {
 public:
    float step(float);
    void set_hysteresis(float);
    void set_value(float value) { value_ = value; }
 private:
    float value_ = 0;
    float hysteresis_ = 0;
};

float fsignf(float a);

class KahanSum {
 public:
    float add(float input) //__attribute__((section (".ccmram")))
    {
        float y = input - c_;
        float t = sum_ + y;
        c_ = (t - sum_) - y;
        sum_ = t;
        return sum_;
    }
    float value() const //__attribute__((section (".ccmram")))
    {
        return sum_;
    }
    void init() {
        sum_ = 0;
        c_ = 0;
    }
 private:
    float sum_ = 0;
    float c_ = 0;

};

class FirstOrderLowPassFilter {
public:
    FirstOrderLowPassFilter(float dt, float frequency_hz=0) {
        dt_ = dt;
        set_frequency(frequency_hz);
    }
    void init(float value) {
        value_ = value;
        last_value_ = value;
    }
    float update(float value) {
        value_ = alpha_*value + (1-alpha_)*last_value_;
        last_value_ = value_;
        return get_value();
    }
    float get_value() const { return value_; }
    void set_frequency(float frequency_hz) {
        if (frequency_hz == 0) {
            alpha_ = 1;
        } else { 
            alpha_ = 2*M_PI*dt_*frequency_hz/(2*M_PI*dt_*frequency_hz + 1);
        }
    }
private:
    float value_ = 0, last_value_ = 0;
    float alpha_, dt_;
};

class PIController {
public:
    ~PIController() {}
    float step(float desired, float measured);
    void set_param(const PIParam &pi_param);
private:
    float kp_ = 0, ki_ = 0, ki_sum_ = 0, ki_limit_ = 0, command_max_ = 0;

};

class RateLimiter {
 public:
    void set_limit(float limit) { limit_ = limit; }
    void init(float value) { last_value_ = value; velocity_ = 0;}
    float step(float value) {
        float out_value = value;
        if (value > (last_value_ + limit_)) {
            out_value = last_value_ + limit_;
            velocity_ = limit_;
        } else if (value < (last_value_ - limit_)) {
            out_value = last_value_ - limit_;
            velocity_ = -limit_;
        } else {
            out_value = value;
            velocity_ = value - last_value_;
        }

        last_value_ = out_value;
        return out_value;
    }
    float get_velocity() const { return velocity_; }
 private:
    float limit_ = INFINITY;
    float velocity_ = 0;
    float last_value_ = 0;
};

class PIDController {
public:
    PIDController(float dt) : dt_(dt), error_dot_filter_(dt) {}
    virtual ~PIDController() {}
    void init(float measured) { rate_limit_.init(measured), measured_last_ = measured; error_dot_filter_.init(0); }
    virtual float step(float desired, float velocity_desired, float measured, float velocity_limit = INFINITY);
    void set_param(const PIDParam &param);
private:
    float kp_ = 0, kd_ = 0, ki_ = 0, ki_sum_ = 0, ki_limit_ = 0, command_max_ = 0;
    float measured_last_ = 0;
    float last_desired_ = 0;
    float dt_;
    Hysteresis hysteresis_;
    RateLimiter rate_limit_;
    FirstOrderLowPassFilter error_dot_filter_;
};

class PIDDeadbandController : public PIDController {
public:
    PIDDeadbandController(float dt) : PIDController(dt) {}
    virtual ~PIDDeadbandController() {}
    virtual float step(float desired, float velocity_desired, float deadband, float measured, float velocity_limit = INFINITY);
};




#endif //MOTOR_CONTROL_FUN_H
