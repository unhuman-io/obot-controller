#include "param_obot_g474_motor40.h"
#include "math.h"


// Can be written by external methods, e.g. bootloader
const Param __attribute__ ((section ("flash_param"))) param_store = {
#include "param_default.h"
#include "param_motor40.c"
    .fast_loop_param.foc_param.pi_d.kp=1,
    .fast_loop_param.foc_param.pi_d.ki=0,
    // .fast_loop_param.foc_param.pi_d.kp=5, // hd
    // .fast_loop_param.foc_param.pi_d.ki=0.4,
    .fast_loop_param.foc_param.pi_d.ki_limit=8,
    .fast_loop_param.foc_param.pi_d.command_max=1,
    .fast_loop_param.foc_param.pi_q.kp=1,
    .fast_loop_param.foc_param.pi_q.ki=0,
    .fast_loop_param.foc_param.pi_q.ki_limit=8,
    .fast_loop_param.foc_param.pi_q.command_max=1,
    .fast_loop_param.foc_param.current_filter_frequency_hz=20000,//35000,
    .fast_loop_param.foc_param.num_poles = 0,
    .fast_loop_param.cogging.gain = 1,
    .startup_param.startup_mode = OPEN,
    .fast_loop_param.motor_encoder.dir = -1,
    .fast_loop_param.phase_mode = 0,
    .fast_loop_param.motor_encoder.cpr = pow(2,24),
    .fast_loop_param.motor_encoder.rollover = pow(2,23+6),
    .fast_loop_param.motor_encoder.use_index_electrical_offset_pos = 0,
    // .main_loop_param.torque_sensor.gain = 1,
    // .main_loop_param.torque_sensor.bias = 0,
    // .main_loop_param.torque_sensor.k_temp = 0,
    .fast_loop_param.vbus_gain = 1.0/4096*(215+13.7)/13.7,
    .main_loop_param.position_controller_param.position.kp = 1,
    .main_loop_param.position_controller_param.position.kd = .01,
    .main_loop_param.position_controller_param.position.velocity_filter_frequency_hz = 1000,
    .main_loop_param.position_controller_param.position.command_max = .7,
    .main_loop_param.torque_controller_param.torque.kp = 10,
    .main_loop_param.torque_controller_param.torque.kd = 0,
    .main_loop_param.torque_controller_param.torque.velocity_filter_frequency_hz = 0,
    .main_loop_param.torque_controller_param.torque.output_filter_frequency_hz = 0,
    .main_loop_param.torque_controller_param.torque.command_max = 1,
    .main_loop_param.impedance_controller_param.impedance.kp = 1,
    .main_loop_param.impedance_controller_param.impedance.kd = .01,
    .main_loop_param.impedance_controller_param.impedance.velocity_filter_frequency_hz = 500,
    .main_loop_param.impedance_controller_param.impedance.command_max = 2,
    .main_loop_param.impedance_controller_param.torque.kp = 10,
    .main_loop_param.impedance_controller_param.torque.kd = 0,
    .main_loop_param.impedance_controller_param.torque.velocity_filter_frequency_hz = 0,
    .main_loop_param.impedance_controller_param.torque.output_filter_frequency_hz = 0,
    .main_loop_param.impedance_controller_param.torque.command_max = 1,
    .main_loop_param.torque_correction = 0,
    .main_loop_param.velocity_controller_param.velocity.ki = 1,
    .main_loop_param.velocity_controller_param.velocity.kp = .01,
    .main_loop_param.velocity_controller_param.velocity.ki_limit = .4,
    .main_loop_param.velocity_controller_param.velocity.velocity_filter_frequency_hz = 0,
    .main_loop_param.velocity_controller_param.velocity.output_filter_frequency_hz = 100,
    .main_loop_param.velocity_controller_param.velocity.command_max = .5,
    .main_loop_param.velocity_controller_param.acceleration_limit = 1000,
    .main_loop_param.output_encoder.cpr = -65536,
    .main_loop_param.host_timeout = 0,
    .main_loop_param.safe_mode = DAMPED,
    .main_loop_param.vbus_min = 10,
    .main_loop_param.output_encoder.table = {
//#include "../../calibration/obot_g474/motor_enc/jtab.dat"
    },
    .fast_loop_param.cogging.table = {
//#include "cog.csv"
    },
    .fast_loop_param.motor_encoder.table = {
//#include "tab.csv"
    },
    .startup_param.do_phase_lock = 0,
    .startup_param.phase_lock_current = .7,
    .startup_param.phase_lock_duration = 2,
    .startup_param.gear_ratio = 5.4,

    .startup_param.motor_encoder_startup = ENCODER_ZERO,
    .name = "J1",
#ifdef PARAM_OVERRIDES
    PARAM_OVERRIDES
#endif
};
