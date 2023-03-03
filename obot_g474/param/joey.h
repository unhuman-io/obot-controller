.name="joey",
#include "sn/2056398E4630.h"
#define MOTOR_ENCODER_BITS 19
.main_loop_param.error_mask.all = ERROR_MASK_ALL & ~ERROR_MASK_MOTOR_TEMPERATURE &  ~ERROR_MASK_MOTOR_ENCODER,
.fast_loop_param.motor_encoder.cpr = pow(2,19),
.fast_loop_param.motor_encoder.rollover = pow(2,25),
.main_loop_param.output_encoder.cpr = -pow(2,17),
.fast_loop_param.motor_encoder.use_index_electrical_offset_pos = 1,
.fast_loop_param.motor_encoder.index_electrical_offset_pos = 8743,
.main_loop_param.output_encoder.bias = 1.273, 
.fast_loop_param.phase_mode = 1,
.main_loop_param.encoder_limits.output_hard_max = .8,
.main_loop_param.encoder_limits.output_hard_min = -.8,
.main_loop_param.encoder_limits.motor_controlled_max = 0,
.main_loop_param.encoder_limits.motor_controlled_min = 0,
.main_loop_param.encoder_limits.motor_hard_max = 0,
.main_loop_param.encoder_limits.motor_hard_min = 0,


