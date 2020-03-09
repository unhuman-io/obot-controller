#include "control_fun.h"
//#include "hal_fun.h"
#include <cmath>

float fsat(float a, float sat) {
    // Slow version
    // if (a > sat) {
    //     return sat;
    // } else if (a < -sat) {
    //     return -sat;
    // } else {
    //     return a;
    // }

    // Doesn't completely optimize to branchless instructions
    // return a>sat ? sat : (a<-sat ? -sat : a);

    // Optimizes to branchless
    float b = a>sat ? sat : a;
    b = b<-sat ? -sat : b;
    return b;

    // Not so great
    // float s = a>0 ? sat : -sat;
    // return std::abs(a)-sat>0 ? s : a;

    // Assembly
    // asm("vcmpe.f32 %[a], %[sat]\n\t"
    //     "vmrs APSR_nzcv, fpscr\n\t"
    //     "it gt\n\t"
    //     "vmovgt.f32 %[a], %[sat]\n\t"
    //     "vneg.f32 %[sat], %[sat]\n\t"
    //     "vcmpe.f32 %[a], %[sat]\n\t"
    //     "vmrs APSR_nzcv, fpscr\n\t"
    //     "it lt\n\t"
    //     "vmovlt.f32 %[a], %[sat]\n\t"
    //     :
    //     : [a] "t" (a), [sat] "t" (sat));
    // return a;
}

float fsignf(float a) {
    return a>=0 ? 1 : -1;
    // possibly this could be better: (sincos.sin > 0) - (sincos.sin < 0)
}

void Hysteresis::set_hysteresis(float value) {
    hysteresis_ = value;
}

float Hysteresis::step(float value) {
    if (value - value_ > hysteresis_) {
        value_ = value - hysteresis_;
    } else if (value - value_ < -hysteresis_) {
        value_ = value + hysteresis_;
    }
    return value_;
}

void PIController::set_param(const PIParam &pi_param) {
    ki_ = pi_param.ki;
    kp_ = pi_param.kp;
    ki_limit_ = pi_param.ki_limit;
    command_max_ = pi_param.command_max;
}

float PIController::step(float desired, float measured) {
    float error = desired - measured;
    ki_sum_ += ki_ * error;
    ki_sum_ = fsat(ki_sum_, ki_limit_);
    return fsat(kp_*error + ki_sum_, command_max_);
}

void PIDController::set_param(const PIDParam &param) {
    ki_ = param.ki;
    kp_ = param.kp;
    ki_limit_ = param.ki_limit;
    kd_ = param.kd;
    command_max_ = param.command_max;
    error_dot_filter_.set_frequency(param.velocity_filter_frequency_hz);
    hysteresis_.set_hysteresis(command_max_/kp_);
}

float PIDController::step(float desired, float velocity_desired, float measured, float velocity_limit) {
    // PID controller with formula
    // out = (ki/s + kp + kd*s) * error
    // with s*error given by velocity_desired - velocity measured from measured with internal 1st order filter
    // with saturation on ki/s * error: ki_limit
    // saturation on out: command_max
    // velocity limit: velocity_limit on command - not if not tracking velocity can exceed this
    rate_limit_.set_limit(fabsf(velocity_limit*dt_));
    float proxy_desired = rate_limit_.step(desired);
    float proxy_dot_desired = fsat(velocity_desired, fabsf(rate_limit_.get_velocity()/dt_));
    float error = proxy_desired - measured;
    float velocity_measured = (measured - measured_last_)/dt_;
    float error_dot = error_dot_filter_.update(velocity_desired - velocity_measured);
    measured_last_ = measured;
    ki_sum_ += ki_ * error;
    ki_sum_ = fsat(ki_sum_, ki_limit_);
    return fsat(kp_*error + ki_sum_ + kd_*error_dot, command_max_);
}

float PIDDeadbandController::step(float desired, float velocity_desired, float deadband, float measured, float velocity_limit) {
    float desired_with_deadband = fsignf(desired-measured)*fmaxf(fabsf(desired-measured) - deadband, 0) + measured;
    return PIDController::step(desired_with_deadband, velocity_desired, measured, velocity_limit);
}