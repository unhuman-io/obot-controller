#define PARAM_OVERRIDES \
.name = "right_knee", \
.main_loop_param.output_encoder.bias = -0.733, \
.fast_loop_param.motor_encoder.use_index_electrical_offset_pos = 1, \
.fast_loop_param.motor_encoder.index_electrical_offset_pos = -1999 + 250 - (-1373), \
.startup_param.phase_lock_current = 20, \
.fast_loop_param.motor_encoder.dir = 1, \
.main_loop_param.output_encoder.cpr = 65536,
