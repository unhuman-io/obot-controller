#include "param_obot_g474.h"
#include "math.h"


// Can be written by external methods, e.g. bootloader
const Param __attribute__ ((section ("flash_param"))) param_store = {
#include "param_default.h"
    .name = "motor_icmu",
    .fast_loop_param.motor_encoder.cpr = 1 << 24,          // the position word is 24 bits per turn (19 significant on the iC-MU)
    .fast_loop_param.motor_encoder.rollover = 1 << (23+6),
#ifdef PARAM_OVERRIDES
    PARAM_OVERRIDES
#endif
};
