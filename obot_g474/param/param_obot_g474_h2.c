#include "param_obot_g474_h2.h"
#include "math.h"


// Can be written by external methods, e.g. bootloader
const volatile Param __attribute__ ((section ("flash_param"))) param_store = {
    .name = "test",
    .main_loop_param.output_encoder.cpr = 1,
    .fast_loop_param.motor_encoder.cpr = pow(2,16),
    .fast_loop_param.motor_encoder.rollover = pow(2,25),
#ifdef PARAM_OVERRIDES
    PARAM_OVERRIDES
#endif
};

const volatile char * const name = param_store.name;
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdiscarded-qualifiers"
const Param * const param = &param_store; // todo figure out a way to not inline without warning
#pragma GCC diagnostic pop