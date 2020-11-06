#include "param_fabulab_f446.h"
#include "math.h"

// Can be written by external methods, e.g. bootloader
const volatile Param param_store = {
    .fast_loop_param.foc_param.pi_d.kp=1,
    .fast_loop_param.foc_param.pi_d.ki=.1,
    .fast_loop_param.foc_param.pi_d.ki_limit=19,
    .fast_loop_param.foc_param.pi_d.command_max=20,
    .fast_loop_param.foc_param.pi_q.kp=1,
    .fast_loop_param.foc_param.pi_q.ki=.1,
    .fast_loop_param.foc_param.pi_q.ki_limit=19,
    .fast_loop_param.foc_param.pi_q.command_max=20,
    .fast_loop_param.foc_param.current_filter_frequency_hz=10000,
    .fast_loop_param.foc_param.num_poles = 7,
    .fast_loop_param.cogging.gain = 1,
    .main_loop_param.gear_ratio = 50,
    .main_loop_param.kt = .012*sqrt(.5),
    .startup_param.startup_mode = OPEN,
    .fast_loop_param.adc1_offset = 1990,
    .fast_loop_param.adc2_offset = 1995,
    .fast_loop_param.adc3_offset = 1980,
    .fast_loop_param.adc1_gain = 3.0/4096/(-.3), //3.3/4096*(.2),  hall // V/count * A/Vr / Vo/Vr
    .fast_loop_param.adc2_gain = 3.0/4096/(-.3), //3.3/4096*(.2),
    .fast_loop_param.adc3_gain = 3.0/4096/(-.3), //3.3/4096*(.2), 
    .fast_loop_param.motor_encoder.dir = -1,
    .fast_loop_param.phase_mode = 0,
    .fast_loop_param.motor_encoder.cpr = 28672,
    .fast_loop_param.motor_encoder.use_index_electrical_offset_pos = 0,
    .fast_loop_param.motor_encoder.index_electrical_offset_pos = 402,
    .main_loop_param.torque_bias = 0.025,
    .main_loop_param.torque_gain = -25,
    .fast_loop_param.vbus_gain = (3.0/4096)*((2+18)/2),
    .main_loop_param.position_controller_param.position.kp = .1,
    .main_loop_param.position_controller_param.position.ki = 0,
    .main_loop_param.position_controller_param.position.kd = 0.001,
    .main_loop_param.position_controller_param.position.command_max = 3,
    .main_loop_param.position_controller_param.position.velocity_filter_frequency_hz = 1000,
    .fast_loop_param.cogging.table = {
    },
    .startup_param.do_phase_lock = 1,
    .startup_param.phase_lock_current = 1,
    .startup_param.phase_lock_duration = 2,
    .name = "J1",
};
const volatile char * const name = param_store.name;
const Param * const param = &param_store; // todo figure out a way to not inline without warning