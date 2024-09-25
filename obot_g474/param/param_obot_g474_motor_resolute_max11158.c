#include "param_obot_g474.h"
#include "math.h"


// Can be written by external methods, e.g. bootloader
const Param __attribute__ ((section ("flash_param"))) param_store = {
#include "param_default.h"
#include "param_motor.c"
    .main_loop_param.output_encoder.cpr = pow(2,32),
    .fast_loop_param.motor_encoder.cpr = pow(2,32)-1,
    .name = "res_max",
#ifdef PARAM_OVERRIDES
    PARAM_OVERRIDES
#endif
};
