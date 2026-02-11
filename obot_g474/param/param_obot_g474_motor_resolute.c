#include "param_obot_g474.h"
#include "math.h"


// Can be written by external methods, e.g. bootloader
const Param __attribute__ ((section ("flash_param"))) param_store = {
#include "param_default.h"
#include "param_motor.c"
    .fast_loop_param.motor_encoder.cpr = 1 << 32,
    .main_loop_param.output_encoder.cpr = 1 << 32,
    .fast_loop_param.foc_param.pi_d.kp=5,
    .fast_loop_param.foc_param.pi_d.ki=.3,
    // .fast_loop_param.foc_param.pi_d.kp=5, // hd
    // .fast_loop_param.foc_param.pi_d.ki=0.4,
    .fast_loop_param.foc_param.pi_d.ki_limit=8,
    .fast_loop_param.foc_param.pi_d.command_max=10,
    .fast_loop_param.foc_param.pi_q.kp=5,
    .fast_loop_param.foc_param.pi_q.ki=.3,
    .fast_loop_param.foc_param.pi_q.ki_limit=8,
    .fast_loop_param.foc_param.pi_q.command_max=10,
    .fast_loop_param.foc_param.current_filter_frequency_hz=5000,//35000,
    .fast_loop_param.foc_param.num_poles = 14,
    .name = "resolute",
#ifdef PARAM_OVERRIDES
    PARAM_OVERRIDES
#endif
};

