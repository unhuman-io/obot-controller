#include "param_obot_g474.h"
#include "math.h"


// Can be written by external methods, e.g. bootloader
const volatile Param __attribute__ ((section ("flash_param"))) param_store = {
#include "param_default.h"
    .fast_loop_param.foc_param.num_poles = 1,
    .startup_param.startup_mode = OPEN,
    .startup_param.no_zero_current_sensors = 1,
    .fast_loop_param.adc1_gain = -3.3/4096/(.005*10),  // A/count
    .fast_loop_param.adc2_gain = -3.3/4096/(.005*10),
    .fast_loop_param.adc3_gain = -3.3/4096/(.005*10), 

    .fast_loop_param.motor_encoder.cpr = 2048,
    .fast_loop_param.motor_encoder.rollover = pow(2,23),

    .fast_loop_param.vbus_gain = 1.0/4096*(215+13.7)/13.7,

    .main_loop_param.output_encoder.cpr = pow(2,32),
    .main_loop_param.host_timeout = 0,
    .main_loop_param.safe_mode = DAMPED,
    .startup_param.do_phase_lock = 0,
    .name = "resolute",
#ifdef PARAM_OVERRIDES
    PARAM_OVERRIDES
#endif
};
const volatile char * const name = param_store.name;
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdiscarded-qualifiers"
const Param * const param = &param_store; // todo figure out a way to not inline without warning
#pragma GCC diagnostic pop
