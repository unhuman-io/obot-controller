#include "param_obot_g474_aksim.h"
#include "math.h"


// Can be written by external methods, e.g. bootloader
const volatile Param __attribute__ ((section ("flash_param"))) param_store = {
    #include "param_obot_g474_motor_aksim_base.c"
    .fast_loop_param.motor_encoder.cpr = pow(2,19),
    .main_loop_param.output_encoder.cpr = pow(2,20),
};
const volatile char * const name = param_store.name;
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdiscarded-qualifiers"
const Param * const param = &param_store; // todo figure out a way to not inline without warning
#pragma GCC diagnostic pop