#include "param_obot_g474.h"
#include "math.h"


// Can be written by external methods, e.g. bootloader
const volatile Param __attribute__ ((section ("flash_param"))) param_store = {
#include "param_default.h"
    .main_loop_param.output_encoder.cpr = pow(2,18),
    .fast_loop_param.motor_encoder.cpr = pow(2,18),
    .fast_loop_param.motor_encoder.rollover = pow(2,28),
    .startup_param.motor_encoder_startup = ENCODER_BIAS,
    .name = "aeat9922",
#ifdef PARAM_OVERRIDES
    PARAM_OVERRIDES
#endif
};
const volatile char * const name = param_store.name;
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdiscarded-qualifiers"
const Param * const param = &param_store; // todo figure out a way to not inline without warning
#pragma GCC diagnostic pop