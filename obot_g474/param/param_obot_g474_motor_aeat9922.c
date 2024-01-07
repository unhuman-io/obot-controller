#include "param_obot_g474.h"
#include "math.h"


// Can be written by external methods, e.g. bootloader
const Param __attribute__ ((section ("flash_param"))) param_store = {
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
