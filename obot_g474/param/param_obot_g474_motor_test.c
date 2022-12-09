#include "param_obot_g474.h"
#include "math.h"


// Can be written by external methods, e.g. bootloader
const volatile Param __attribute__ ((section ("flash_param"))) param_store = {
#include "param_motor_km_8508.c"
    .fast_loop_param.cogging.gain = 0,
    .startup_param.startup_mode = OPEN,
#include "param_motor.c"
    .fast_loop_param.motor_encoder.dir = -1,
    .fast_loop_param.phase_mode = 1,
    .fast_loop_param.motor_encoder.cpr = pow(2,18),
    .fast_loop_param.motor_encoder.rollover = pow(2,25),
    .fast_loop_param.motor_encoder.use_index_electrical_offset_pos = 0,
    .main_loop_param.position_controller_param.position.kp = 100,
    .main_loop_param.position_controller_param.position.kd = 1,
    .main_loop_param.position_controller_param.position.velocity_filter_frequency_hz = 1000,
    .main_loop_param.position_controller_param.position.output_filter_frequency_hz = 1000,
    .main_loop_param.position_controller_param.position.command_max = 5,
    .main_loop_param.velocity_controller_param.velocity.ki = 100,
    .main_loop_param.velocity_controller_param.velocity.kp = 1,
    .main_loop_param.velocity_controller_param.velocity.ki_limit = 8,
    .main_loop_param.velocity_controller_param.velocity.output_filter_frequency_hz = 1000,
    .main_loop_param.velocity_controller_param.velocity.command_max = 5,
    .main_loop_param.velocity_controller_param.acceleration_limit = 1000,
    .main_loop_param.output_encoder.cpr = pow(2,18),
    .main_loop_param.host_timeout = 0,
    .main_loop_param.safe_mode = OPEN,
    .startup_param.do_phase_lock = 1,
    .startup_param.phase_lock_current = 5,
    .startup_param.phase_lock_duration = 2,
    .name = "J",
#ifdef PARAM_OVERRIDES
    PARAM_OVERRIDES
#endif
};
const volatile char * const name = param_store.name;
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdiscarded-qualifiers"
const Param * const param = &param_store; // todo figure out a way to not inline without warning
#pragma GCC diagnostic pop