.name = "dimitri",
#include "sn/2054397F4630.h"
#define MOTOR_ENCODER_BITS 19
.main_loop_param.error_mask.all = ERROR_MASK_ALL & ~ERROR_MASK_OUTPUT_ENCODER,
.fast_loop_param.motor_encoder.cpr = pow(2, 19),
.fast_loop_param.motor_encoder.rollover = pow(2, 25),
.fast_loop_param.motor_encoder.use_index_electrical_offset_pos = 1,
.fast_loop_param.motor_encoder.index_electrical_offset_pos = 235,
.main_loop_param.encoder_limits.output_hard_max = 0,
.main_loop_param.encoder_limits.output_hard_min = 0,
.main_loop_param.encoder_limits.motor_controlled_max = 0,
.main_loop_param.encoder_limits.motor_controlled_min = 0,
.main_loop_param.encoder_limits.motor_hard_max = 0,
.main_loop_param.encoder_limits.motor_hard_min = 0,