#define PARAM_OVERRIDES \
.name = "right_hip_pitch", \
.main_loop_param.output_encoder.bias = 2.126+.155, \
.fast_loop_param.motor_encoder.use_index_electrical_offset_pos = 1, \
.fast_loop_param.motor_encoder.index_electrical_offset_pos = 4278 - 2778, /*-3149 - (-1373), */ \
.startup_param.phase_lock_current = 20, \
.fast_loop_param.motor_encoder.dir = 1, \
.main_loop_param.output_encoder.cpr = 65536,
