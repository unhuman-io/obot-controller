#include "param_freebot_g474_col.h"
#include "math.h"

#define V_REF 3.0

// Can be written by external methods, e.g. bootloader
const volatile Param __attribute__ ((section ("flash_param"))) param_store = {
    .fast_loop_param.foc_param.pi_d.kp=4,
    .fast_loop_param.foc_param.pi_d.ki=1,
    // .fast_loop_param.foc_param.pi_d.kp=5, // hd
    // .fast_loop_param.foc_param.pi_d.ki=0.4,
    .fast_loop_param.foc_param.pi_d.ki_limit=8,
    .fast_loop_param.foc_param.pi_d.command_max=10,
    .fast_loop_param.foc_param.pi_q.kp=4,
    .fast_loop_param.foc_param.pi_q.ki=1,
    .fast_loop_param.foc_param.pi_q.ki_limit=8,
    .fast_loop_param.foc_param.pi_q.command_max=10,
    .fast_loop_param.foc_param.current_filter_frequency_hz=0,//35000,
    .fast_loop_param.foc_param.num_poles = 7,
    .fast_loop_param.cogging.gain = 0,
    .main_loop_param.gear_ratio = 50,
    .main_loop_param.kt = .012*sqrt(.5),
    .startup_param.startup_mode = CURRENT,
    .fast_loop_param.adc1_offset = 1980,
    .fast_loop_param.adc2_offset = 1980,
    .fast_loop_param.adc3_offset = 1980,
    .fast_loop_param.adc1_gain = V_REF/4096*(10000/3900.0),  // A/count
    .fast_loop_param.adc2_gain = V_REF/4096*(10000/3900.0),
    .fast_loop_param.adc3_gain = V_REF/4096*(10000/3900.0), 
    .fast_loop_param.motor_encoder.dir = -1,
    .fast_loop_param.phase_mode = 0,
    .fast_loop_param.motor_encoder.cpr = 65536*7,
    .fast_loop_param.motor_encoder.rollover = 65536*7*100,
    .fast_loop_param.motor_encoder.use_index_electrical_offset_pos = 0,
    .fast_loop_param.motor_encoder.index_electrical_offset_pos = -22643,
    .main_loop_param.torque_bias = 0,
    .main_loop_param.torque_gain = 1,
    //.main_loop_param.torque_sensor.gain = -30.7,
    //.main_loop_param.torque_sensor.bias = .029,
    .main_loop_param.torque_sensor.gain = 30.7,
    .main_loop_param.torque_sensor.bias = .203,
    .main_loop_param.torque_sensor.k_temp = 0,
    .main_loop_param.torque_sensor.filter_frequency_hz = 0,
    .fast_loop_param.vbus_gain = V_REF/4096*(18+2)/2.0,
    .main_loop_param.controller_param.kp = 3,
    .main_loop_param.controller_param.kd = .01,
    .main_loop_param.controller_param.velocity_filter_frequency_hz = 0,
    .main_loop_param.controller_param.command_max = .5,
    .main_loop_param.torque_controller_param.kp = 50,
    .main_loop_param.torque_controller_param.kd = .1,
    .main_loop_param.torque_controller_param.velocity_filter_frequency_hz = 0,
    .main_loop_param.torque_controller_param.output_filter_frequency_hz = 0,
    .main_loop_param.torque_controller_param.command_max = 1,
    .main_loop_param.impedance_controller_param.kp = .1,
    .main_loop_param.impedance_controller_param.kd = .01,
    .main_loop_param.impedance_controller_param.velocity_filter_frequency_hz = 500,
    .main_loop_param.impedance_controller_param.command_max = 2,
    .main_loop_param.output_encoder.cpr = -65536,
    .main_loop_param.host_timeout = 10000,
    .main_loop_param.safe_mode = OPEN,
    .fast_loop_param.cogging.table = {
//#include "cogprocessed.csv"
    },
    .startup_param.do_phase_lock = 1,
    .startup_param.phase_lock_current = 1,
    .startup_param.phase_lock_duration = 1,
    .name = "J1",
#ifdef PARAM_OVERRIDES
    PARAM_OVERRIDES
#endif
};
const volatile char * const name = param_store.name;
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdiscarded-qualifiers"
const Param * const param = &param_store; // todo figure out a way to not inline without warning
#pragma GCC diagnostic pop