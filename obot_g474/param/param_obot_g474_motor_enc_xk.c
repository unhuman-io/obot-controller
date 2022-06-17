#include "param_obot_g474.h"
#include "math.h"


// Can be written by external methods, e.g. bootloader
const volatile Param __attribute__ ((section ("flash_param"))) param_store = {
    .fast_loop_param.foc_param.pi_d.kp=2,
    .fast_loop_param.foc_param.pi_d.ki=.1,
    // .fast_loop_param.foc_param.pi_d.kp=5, // hd
    // .fast_loop_param.foc_param.pi_d.ki=0.4,
    .fast_loop_param.foc_param.pi_d.ki_limit=8,
    .fast_loop_param.foc_param.pi_d.command_max=10,
    .fast_loop_param.foc_param.pi_q.kp=2,
    .fast_loop_param.foc_param.pi_q.ki=.1,
    .fast_loop_param.foc_param.pi_q.ki_limit=16,
    .fast_loop_param.foc_param.pi_q.command_max=20,
    .fast_loop_param.foc_param.current_filter_frequency_hz=20000,//35000,
    .fast_loop_param.foc_param.num_poles = 8,
    .fast_loop_param.cogging.gain = 1,
    .startup_param.startup_mode = OPEN,
    .fast_loop_param.adc1_offset = 1980,
    .fast_loop_param.adc2_offset = 1980,
    .fast_loop_param.adc3_offset = 1980,
    .fast_loop_param.adc1_gain = -3.3/4096/(.002*10),  // A/count
    .fast_loop_param.adc2_gain = -3.3/4096/(.002*10),
    .fast_loop_param.adc3_gain = -3.3/4096/(.002*10), 
    .fast_loop_param.motor_encoder.dir = -1,
    .fast_loop_param.phase_mode = 1,
    .fast_loop_param.motor_encoder.cpr = 4000,
    .fast_loop_param.motor_encoder.rollover = pow(2,24),
    .fast_loop_param.motor_encoder.use_index_electrical_offset_pos = 1,
    .fast_loop_param.motor_encoder.index_electrical_offset_pos = 1037 - (-2566),
    .main_loop_param.torque_sensor.gain = -30.7,
    .main_loop_param.torque_sensor.bias = 0,
    .main_loop_param.torque_sensor.k_temp = 0,
    .fast_loop_param.vbus_gain = 1.0/4096*(215+13.7)/13.7,
    .main_loop_param.position_controller_param.position.kp = 200,
    .main_loop_param.position_controller_param.position.kd = .5,
    .main_loop_param.position_controller_param.position.velocity_filter_frequency_hz = 500,
    .main_loop_param.position_controller_param.position.output_filter_frequency_hz = 500,
    .main_loop_param.position_controller_param.position.command_max = 45,
    .main_loop_param.position_controller_param.velocity_limit = 20,
    .main_loop_param.position_controller_param.desired_filter_hz = 40,
    .main_loop_param.position_controller_param.position.ki_limit = 25,
    .main_loop_param.position_controller_param.position.ki = 10000,
    .main_loop_param.torque_controller_param.torque.kp = 10,
    .main_loop_param.torque_controller_param.torque.kd = 0,
    .main_loop_param.torque_controller_param.torque.velocity_filter_frequency_hz = 0,
    .main_loop_param.torque_controller_param.torque.output_filter_frequency_hz = 0,
    .main_loop_param.torque_controller_param.torque.command_max = 1,
    .main_loop_param.impedance_controller_param.impedance.kp = .1,
    .main_loop_param.impedance_controller_param.impedance.kd = .01,
    .main_loop_param.impedance_controller_param.impedance.velocity_filter_frequency_hz = 500,
    .main_loop_param.impedance_controller_param.impedance.command_max = 2,
    .main_loop_param.impedance_controller_param.torque.kp = 10,
    .main_loop_param.impedance_controller_param.torque.kd = 0,
    .main_loop_param.impedance_controller_param.torque.velocity_filter_frequency_hz = 0,
    .main_loop_param.impedance_controller_param.torque.output_filter_frequency_hz = 0,
    .main_loop_param.impedance_controller_param.torque.command_max = 1,
    .main_loop_param.torque_correction = 1,
    .main_loop_param.velocity_controller_param.velocity.ki = 200,
    .main_loop_param.velocity_controller_param.velocity.kp = 1,
    .main_loop_param.velocity_controller_param.velocity.ki_limit = 8    ,
    .main_loop_param.velocity_controller_param.velocity.velocity_filter_frequency_hz = 0,
    .main_loop_param.velocity_controller_param.velocity.output_filter_frequency_hz = 500,
    .main_loop_param.velocity_controller_param.velocity.command_max = 10,
    .main_loop_param.velocity_controller_param.acceleration_limit = 40,
    .main_loop_param.output_encoder.cpr = -65536,
    .main_loop_param.output_encoder.bias = 1.396,//1.21+.18,
    .main_loop_param.host_timeout = 0,
    .main_loop_param.safe_mode = OPEN,
    .fast_loop_param.cogging.table = {
//#include "cog.csv"
    },
    .fast_loop_param.motor_encoder.table = {
//#include "tab.csv"
    },
    .drv_regs = {
        (2<<11) | 0x00,  // control_reg 0x00, 6 PWM mode
        (3<<11) | 0x3FF, // hs_reg      0x3CC, moderate drive current
        (4<<11) | 0x37F, // ls_reg      0x0CC, no cycle by cycle, 4000 ns tdrive
                                        // moderate drive current (.57,1.14A)
        (5<<11) | 0x002,  // ocp_reg     0x00 -> 50 ns dead time, 
                                    //latched ocp, 2 us ocp deglitch, 0.2 Vds thresh -> .2/(.0017*1.5) = 78 A
        (6<<11) | 0x240, // csa_reg     0x240 -> bidirectional current, 10V/V
        //(6<<11) | 0x200, // csa_reg     0x200 -> bidirectional current, 5V/V
    },
    .startup_param.do_phase_lock = 1,
    .startup_param.phase_lock_current = -20,
    .startup_param.phase_lock_duration = 2,
    .startup_param.motor_encoder_startup = ENCODER_BIAS_FROM_OUTPUT,
    .startup_param.gear_ratio = 20.25,
    .startup_param.output_encoder_rollover = 2,
    .main_loop_param.encoder_limits.motor_controlled_max = 27,
    .main_loop_param.encoder_limits.motor_controlled_min = -1,
    .main_loop_param.encoder_limits.motor_hard_max = 33,
    .main_loop_param.encoder_limits.motor_hard_min = -3,
    .main_loop_param.encoder_limits.output_hard_max = 33/20.25,
    .main_loop_param.encoder_limits.output_hard_min = -3/20.25,
    .name = "left_knee",
#ifdef PARAM_OVERRIDES
    PARAM_OVERRIDES
#endif
};
const volatile char * const name = param_store.name;
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdiscarded-qualifiers"
const Param * const param = &param_store; // todo figure out a way to not inline without warning
#pragma GCC diagnostic pop