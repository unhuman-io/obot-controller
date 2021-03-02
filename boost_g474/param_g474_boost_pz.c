#include "param_g474_boost.h"
#include "math.h"

// Can be written by external methods, e.g. bootloader
const volatile Param __attribute__ ((section ("flash_param"))) param_store = {
    .fast_loop_param.foc_param.pi_d.kp=5,
    .fast_loop_param.foc_param.pi_d.ki=0.2,
    // .fast_loop_param.foc_param.pi_d.kp=5, // hd
    // .fast_loop_param.foc_param.pi_d.ki=0.4,
    .fast_loop_param.foc_param.pi_d.ki_limit=8,
    .fast_loop_param.foc_param.pi_d.command_max=10,
    .fast_loop_param.foc_param.pi_q.kp=5,
    .fast_loop_param.foc_param.pi_q.ki=0.2,
    .fast_loop_param.foc_param.pi_q.ki_limit=8,
    .fast_loop_param.foc_param.pi_q.command_max=10,
    .fast_loop_param.foc_param.current_filter_frequency_hz=20000,
    .fast_loop_param.foc_param.num_poles = 15,
    .fast_loop_param.cogging.gain = 0,
    .startup_param.startup_mode = CURRENT,
    .fast_loop_param.adc1_offset = 1980,
    .fast_loop_param.adc2_offset = 1980,
    .fast_loop_param.adc3_offset = 1980,
    .fast_loop_param.adc1_gain = -3.26/4096/(.007*10),  // V/count * A/Vr / Vo/Vr (3.3/4096 / R / Gain)
    .fast_loop_param.adc2_gain = -3.26/4096/(.007*10),
    .fast_loop_param.adc3_gain = -3.26/4096/(.007*10), 
    .fast_loop_param.motor_encoder.dir = -1,
    .fast_loop_param.phase_mode = 0,
    .fast_loop_param.motor_encoder.cpr = pow(2,24),//65536*7,//4096*7,
    .fast_loop_param.motor_encoder.rollover = pow(2,24), // one revolution rollover
    .fast_loop_param.motor_encoder.use_index_electrical_offset_pos = 0,
    .fast_loop_param.motor_encoder.index_electrical_offset_pos = 49546,
    .main_loop_param.output_encoder.cpr = 1,
    .main_loop_param.torque_sensor.gain = -1./10000,
    .main_loop_param.torque_sensor.bias = 0,
    .main_loop_param.torque_sensor.k_temp = 0,
    .fast_loop_param.vbus_gain = 3.26/4096*(82.+4.99)/4.99,
    .main_loop_param.position_controller_param.position.kp = 200,
    .main_loop_param.position_controller_param.position.kd = 4,
    .main_loop_param.position_controller_param.position.velocity_filter_frequency_hz = 400,
    .main_loop_param.position_controller_param.position.command_max = 5,
    .main_loop_param.torque_controller_param.torque.kp = 10,
    .main_loop_param.torque_controller_param.torque.kd = 0,
    .main_loop_param.torque_controller_param.torque.velocity_filter_frequency_hz = 120,
    .main_loop_param.torque_controller_param.torque.output_filter_frequency_hz = 1000,
    .main_loop_param.torque_controller_param.torque.command_max = 5,
    .main_loop_param.impedance_controller_param.impedance.kp = .1,
    .main_loop_param.impedance_controller_param.impedance.kd = .01,
    .main_loop_param.impedance_controller_param.impedance.velocity_filter_frequency_hz = 500,
    .main_loop_param.impedance_controller_param.impedance.command_max = 2,
    .main_loop_param.impedance_controller_param.torque.kp = 10,
    .main_loop_param.impedance_controller_param.torque.kd = .1,
    .main_loop_param.impedance_controller_param.torque.velocity_filter_frequency_hz = 0,
    .main_loop_param.impedance_controller_param.torque.output_filter_frequency_hz = 0,
    .main_loop_param.impedance_controller_param.torque.command_max = 3,
    .main_loop_param.velocity_controller_param.position.kp = 200,
    .main_loop_param.velocity_controller_param.position.kd = 4,
    .main_loop_param.velocity_controller_param.position.velocity_filter_frequency_hz = 400,
    .main_loop_param.velocity_controller_param.position.command_max = 5,
    .fast_loop_param.cogging.table = {
#include "cogprocessed.csv"
    },
    .startup_param.do_phase_lock = 1,
    .startup_param.phase_lock_current = 5,
    .startup_param.phase_lock_duration = 2,
    .name = "wl",
#ifdef PARAM_OVERRIDES
    PARAM_OVERRIDES
#endif
};
const volatile char * const name = param_store.name;
const Param * const param = &param_store; // todo figure out a way to not inline without warning