#include "param_obot_g474.h"
#include "math.h"


// Can be written by external methods, e.g. bootloader
const volatile Param __attribute__ ((section ("flash_param"))) param_store = {
#include "param_motor_km_8508.c"
    .fast_loop_param.cogging.gain = 0,
    .startup_param.startup_mode = OPEN,
#include "param_motor.c"
    .fast_loop_param.motor_encoder.dir = -1,
    .fast_loop_param.phase_mode = 0,
    .fast_loop_param.motor_encoder.cpr = pow(2,18),
    .fast_loop_param.motor_encoder.rollover = pow(2,25),
    .fast_loop_param.motor_encoder.use_index_electrical_offset_pos = 1,
    .fast_loop_param.motor_encoder.index_electrical_offset_pos = 17000,
    .main_loop_param.torque_sensor.gain = 242,
    .main_loop_param.torque_sensor.bias = 0,
    .main_loop_param.position_controller_param.position.kp = 300,
    .main_loop_param.position_controller_param.position.kd = 3,
    .main_loop_param.position_controller_param.position.velocity_filter_frequency_hz = 1000,
    .main_loop_param.position_controller_param.position.output_filter_frequency_hz = 1000,
    .main_loop_param.position_controller_param.position.command_max = 10,
    .main_loop_param.velocity_controller_param.velocity.ki = 300,
    .main_loop_param.velocity_controller_param.velocity.kp = 3,
    .main_loop_param.velocity_controller_param.velocity.ki_limit = 8,
    .main_loop_param.velocity_controller_param.velocity.output_filter_frequency_hz = 1000,
    .main_loop_param.velocity_controller_param.velocity.command_max = 10,
    .main_loop_param.velocity_controller_param.acceleration_limit = 1000,
    .main_loop_param.output_encoder.cpr = pow(2,18),
    .main_loop_param.host_timeout = 0,
    .main_loop_param.safe_mode = OPEN,
    .startup_param.do_phase_lock = 0,
    .startup_param.phase_lock_current = 5,
    .startup_param.phase_lock_duration = 2,
    .startup_param.gear_ratio = 81,
    .startup_param.no_driver_enable = 1,
    .startup_param.no_zero_current_sensors = 1,
    .main_loop_param.output_encoder.bias = -1.5767,
    .main_loop_param.encoder_limits.output_hard_max = 1.9,
    .main_loop_param.encoder_limits.output_hard_min = -1.9,
    .main_loop_param.encoder_limits.motor_controlled_max = 1.8*81,
    .main_loop_param.encoder_limits.motor_controlled_min = -1.8*81,
    .main_loop_param.encoder_limits.motor_hard_max = 1.9*81,
    .main_loop_param.encoder_limits.motor_hard_min = -1.9*80,

//    .main_loop_param.error_mask.all = ERROR_MASK_ALL,
    .main_loop_param.error_mask.all = ERROR_MASK_ALL & ~ERROR_MASK_MOTOR_TEMPERATURE & ~ERROR_MASK_OUTPUT_ENCODER & ~ERROR_MASK_MOTOR_ENCODER,
    .main_loop_param.safe_mode_driver_disable = 1,

    .startup_param.motor_encoder_startup = ENCODER_BIAS_FROM_OUTPUT_WITH_MOTOR_CORRECTION,
    .startup_param.motor_encoder_bias = 0,
    .startup_param.output_encoder_rollover = M_PI,

    .main_loop_param.no_latch_driver_fault = 1,

    .name = "A",
#ifdef PARAM_OVERRIDES
    PARAM_OVERRIDES
#endif
};
const volatile char * const name = param_store.name;
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdiscarded-qualifiers"
const Param * const param = &param_store; // todo figure out a way to not inline without warning
#pragma GCC diagnostic pop