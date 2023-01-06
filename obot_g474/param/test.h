#define PARAM_OVERRIDES \
.name = "test", \
.main_loop_param.output_encoder.bias = 0, \
.main_loop_param.encoder_limits.output_hard_max = 0, \
.main_loop_param.encoder_limits.output_hard_min = 0, \
.main_loop_param.encoder_limits.motor_controlled_max = 0, \
.main_loop_param.encoder_limits.motor_controlled_min = 0, \
.main_loop_param.encoder_limits.motor_hard_max = 0, \
.main_loop_param.encoder_limits.motor_hard_min = 0, \
.startup_param.do_phase_lock = 1, \
.fast_loop_param.motor_encoder.use_index_electrical_offset_pos = 0, \
.fast_loop_param.phase_mode = 1, \
.startup_param.phase_lock_current = -5, \
.startup_param.motor_encoder_startup = ENCODER_ZERO,