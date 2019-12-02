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
}

float PIDController::step(float desired, float measured) {
    float error = desired - measured;
    float error_dot = error-error_last_;
    error_last_ = error;
    ki_sum_ += ki_ * error;
    ki_sum_ = fsat(ki_sum_, ki_limit_);
    return fsat(kp_*error + ki_sum_ + kd_*error_dot, command_max_);
}