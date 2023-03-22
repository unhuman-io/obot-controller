.name="dyno8513D",
#include "sn/2054397F4630.h"
#define MOTOR_ENCODER_BITS 19
#include "param_motor_km_8513.c"
.main_loop_param.error_mask.all = ERROR_MASK_ALL & ~ERROR_MASK_OUTPUT_ENCODER & ~ERROR_MASK_MOTOR_TEMPERATURE,
.fast_loop_param.motor_encoder.cpr = pow(2,19),
.fast_loop_param.motor_encoder.rollover = pow(2,25),
.fast_loop_param.motor_encoder.use_index_electrical_offset_pos = 1,

.main_loop_param.encoder_limits.output_hard_max = 0,
.main_loop_param.encoder_limits.output_hard_min = 0,
.main_loop_param.encoder_limits.motor_controlled_max = 0,
.main_loop_param.encoder_limits.motor_controlled_min = 0,
.main_loop_param.encoder_limits.motor_hard_max = 0,
.main_loop_param.encoder_limits.motor_hard_min = 0,

.main_loop_param.velocity_controller_param.velocity.ki = 70,
.main_loop_param.velocity_controller_param.velocity.kp = 7,
.main_loop_param.velocity_controller_param.velocity.ki_limit = 30,
.main_loop_param.velocity_controller_param.velocity.output_filter_frequency_hz = 100,
.main_loop_param.velocity_controller_param.velocity.command_max = 40,
.main_loop_param.velocity_controller_param.acceleration_limit = 10,