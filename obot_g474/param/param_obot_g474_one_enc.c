#include "param_obot_g474_osa.h"
#include "math.h"

// Can be written by external methods, e.g. bootloader
const Param __attribute__ ((section ("flash_param"))) param_store = {
#include "param_default.h"
    .fast_loop_param.foc_param.pi_d.kp=3,
    .fast_loop_param.foc_param.pi_d.ki=0.3,
    .fast_loop_param.foc_param.pi_d.ki_limit=16,
    .fast_loop_param.foc_param.pi_d.command_max=20,

    .fast_loop_param.foc_param.pi_q.kp=3,
    .fast_loop_param.foc_param.pi_q.ki=0.3,
    .fast_loop_param.foc_param.pi_q.ki_limit=16,
    .fast_loop_param.foc_param.pi_q.command_max=20,

    .fast_loop_param.foc_param.current_filter_frequency_hz=35000,
    .fast_loop_param.foc_param.num_poles = 7,
    .fast_loop_param.cogging.gain = 1,
    .startup_param.startup_mode = OPEN,

    .fast_loop_param.adc1_gain = 3.0/4096*(10000/4000.0),  // A/count
    .fast_loop_param.adc2_gain = 3.0/4096*(10000/4000.0),
    .fast_loop_param.adc3_gain = 3.0/4096*(10000/4000.0), 

    .fast_loop_param.motor_encoder.dir = 0,
    .fast_loop_param.phase_mode = 0,
    .fast_loop_param.motor_encoder.cpr = 7*65536,
    .fast_loop_param.motor_encoder.rollover = 7*65536*1000,//pow(2,24),
    .fast_loop_param.motor_encoder.use_index_electrical_offset_pos = 0,
    .fast_loop_param.vbus_gain = 1.0/4096*(33+2.0)/2.0,

    .main_loop_param.torque_sensor.gain = 1000,
    .main_loop_param.torque_sensor.bias = 0,
    .main_loop_param.torque_sensor.k_temp = 0,
    .main_loop_param.torque_sensor.offset_A = 0,
    .main_loop_param.torque_sensor.offset_B = 0,
    .main_loop_param.torque_sensor.offset_C = 0,
    .main_loop_param.torque_sensor.gain_A = 1,
    .main_loop_param.torque_sensor.gain_B = 1,
    .main_loop_param.torque_sensor.gain_C = 1,
    .main_loop_param.torque_sensor.gain_T = 1,

    .main_loop_param.position_controller_param.position.kp = 3,
    .main_loop_param.position_controller_param.position.kd = .01,
    .main_loop_param.position_controller_param.position.velocity_filter_frequency_hz = 400,
    .main_loop_param.position_controller_param.position.command_max = 3,

    .main_loop_param.torque_controller_param.torque.kp = 10,
    .main_loop_param.torque_controller_param.torque.kd = 0,
    .main_loop_param.torque_controller_param.torque.velocity_filter_frequency_hz = 0,
    .main_loop_param.torque_controller_param.torque.output_filter_frequency_hz = 0,
    .main_loop_param.torque_controller_param.torque.command_max = 3,

    .main_loop_param.impedance_controller_param.impedance.kp = .1,
    .main_loop_param.impedance_controller_param.impedance.kd = .01,
    .main_loop_param.impedance_controller_param.impedance.velocity_filter_frequency_hz = 500,
    .main_loop_param.impedance_controller_param.impedance.command_max = 2,

    .main_loop_param.impedance_controller_param.torque.kp = 10,
    .main_loop_param.impedance_controller_param.torque.kd = 0,
    .main_loop_param.impedance_controller_param.torque.velocity_filter_frequency_hz = 0,
    .main_loop_param.impedance_controller_param.torque.output_filter_frequency_hz = 0,
    .main_loop_param.impedance_controller_param.torque.command_max = 1,
    .main_loop_param.torque_correction = 0,

    .main_loop_param.velocity_controller_param.velocity.ki = 3,
    .main_loop_param.velocity_controller_param.velocity.kp = .3,
    .main_loop_param.velocity_controller_param.velocity.ki_limit = 3,
    .main_loop_param.velocity_controller_param.velocity.velocity_filter_frequency_hz = 400,
    .main_loop_param.velocity_controller_param.velocity.output_filter_frequency_hz = 400,
    .main_loop_param.velocity_controller_param.velocity.command_max = 3,
    .main_loop_param.velocity_controller_param.acceleration_limit = 100,

    .main_loop_param.output_encoder.cpr = 65536,
    .main_loop_param.host_timeout = 0,
    .main_loop_param.safe_mode = DAMPED,
    .fast_loop_param.cogging.table = {
//#include "cog_small.csv"
    },
    .fast_loop_param.motor_encoder.table = {
//#include "tab.csv"
    },
    .startup_param.do_phase_lock = 1,
    .startup_param.phase_lock_current = -2,
    .startup_param.phase_lock_duration = 2,
    .startup_param.motor_encoder_startup = ENCODER_BIAS_FROM_OUTPUT,
    .name = "AP",
#ifdef PARAM_OVERRIDES
    PARAM_OVERRIDES
#endif
};
