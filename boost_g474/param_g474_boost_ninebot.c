#include "math.h"
#include "param.h"

// Can be written by external methods, e.g. bootloader
const volatile Param __attribute__((section("flash_param"))) initial_param = {
    .fast_loop_param.pwm_frequency = 50000,
    .fast_loop_param.foc_param.pi_d.kp = 0.25,
    .fast_loop_param.foc_param.pi_d.ki = 0.04,
    .fast_loop_param.foc_param.pi_d.ki_limit = 2.5,
    .fast_loop_param.foc_param.pi_d.command_max = 3,
    .fast_loop_param.foc_param.pi_q.kp = 0.25,
    .fast_loop_param.foc_param.pi_q.ki = 0.04,
    .fast_loop_param.foc_param.pi_q.ki_limit = 8,
    .fast_loop_param.foc_param.pi_q.command_max = 12,
    .fast_loop_param.foc_param.num_poles = 15,
    .main_loop_param.gear_ratio = 50,
    .main_loop_param.kt = .012 * sqrt(.5),
    .startup_param.startup_mode = CURRENT,
    .main_loop_param.update_frequency = 10000,
    .fast_loop_param.adc1_gain =
        -3.26 / 4096 /
        (.007 * 20),  // V/count * A/Vr / Vo/Vr (3.3/4096 / R / Gain)
    .fast_loop_param.adc2_gain = -3.26 / 4096 / (.007 * 20),
    .fast_loop_param.adc3_gain = -3.26 / 4096 / (.007 * 20),
    .fast_loop_param.motor_encoder.dir = -1,
    .fast_loop_param.phase_mode = 0,
    .fast_loop_param.motor_encoder.cpr = 15 * 6,
    .fast_loop_param.motor_encoder.use_index_electrical_offset_pos = 0,
    .fast_loop_param.motor_encoder.index_electrical_offset_pos = 402,
    .main_loop_param.torque_bias = 0.025,
    .main_loop_param.torque_gain = -25,
    .fast_loop_param.vbus_gain = 3.26 / 4096 * (82. + 4.99) / 4.99,
    .main_loop_param.controller_param.kp = 10,
    .main_loop_param.controller_param.kd = .1,
    .main_loop_param.controller_param.velocity_filter_frequency_hz = 20,
    .main_loop_param.controller_param.command_max = 10,
    .fast_loop_param.cogging.table =
        {
#include "cogprocessed.csv"
        },
    .startup_param.do_phase_lock = 1,
    .startup_param.phase_lock_current = 4,
    .startup_param.phase_lock_duration = 4,
    .name = "J1",
};