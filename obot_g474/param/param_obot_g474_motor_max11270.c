#include "param_obot_g474_aksim.h"
#include "math.h"


// Can be written by external methods, e.g. bootloader
const Param __attribute__ ((section ("flash_param"))) param_store = {
#include "param_motor.c"
    .main_loop_param.torque_sensor.gain = 1,
    .main_loop_param.torque_sensor.bias = 0,
    .main_loop_param.output_encoder.cpr = 1,

    .name = "max11270",
#ifdef PARAM_OVERRIDES
#include XSTR(PARAM_OVERRIDES)
#endif
};
