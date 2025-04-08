#include "param_obot_g474_can.h"
#include "math.h"


// Can be written by external methods, e.g. bootloader
const Param __attribute__ ((section ("flash_param"))) param_store = {
#include "param_default.h"
.fast_loop_param.adc1_gain = -3.3/4096/(.033*5*1.0),  // A/count,
.fast_loop_param.adc2_gain = -3.3/4096/(.033*5*1.0),
.fast_loop_param.adc3_gain = -3.3/4096/(.033*5*1.0),
.fast_loop_param.vbus_gain = 1.0/4096*(215+13.7)/13.7, // note adc1 set up with GCOMP, 1V -> 4096 counts
.drv_regs = {
    (2<<11) | 0x00,  // control_reg 0x00, 6 PWM mode
    //(3<<11) | 0x3AA, // hs_reg      0x3CC, moderate drive current
    (3<<11) | 0x3FF, // hs_reg      0x3CC, moderate drive current
    //(4<<11) | 0x2FF, // ls_reg      0x0CC, no cycle by cycle, 500 ns tdrive
                                    // moderate drive current (.57,1.14A)
    (4<<11) | 0x37F, // ls_reg      0x0CC, no cycle by cycle, 4000 ns tdrive
                                    // moderate drive current (.57,1.14A)
    (5<<11) | 0x004,  // ocp_reg     0x00 -> 50 ns dead time, 
                                //latched ocp, 2 us ocp deglitch, 0.31 Vds thresh -> .31/.003 = 100A
    //(6<<11) | 0x2C0, // csa_reg     0x2C0 -> bidirectional current, 40V/V
    //(6<<11) | 0x280,
    (6<<11) | 0x200, // csa_reg     0x240 -> bidirectional current, 5V/V
},
.fast_loop_param.foc_param.pi_d.kp=2, // no vbus measurement
.fast_loop_param.foc_param.pi_d.ki=.1,
.fast_loop_param.foc_param.pi_d.ki_limit=10,
.fast_loop_param.foc_param.pi_d.command_max=10,
.fast_loop_param.foc_param.pi_q.kp=2,
.fast_loop_param.foc_param.pi_q.ki=.1,
.fast_loop_param.foc_param.pi_q.ki_limit=10,
.fast_loop_param.foc_param.pi_q.command_max=10,
.fast_loop_param.foc_param.current_filter_frequency_hz=40000,
.fast_loop_param.foc_param.num_poles = 14,
.fast_loop_param.phase_mode = 1,
.fast_loop_param.current_direction = 1,
.fast_loop_param.motor_encoder.dir = -1,
.fast_loop_param.motor_encoder.use_index_electrical_offset_pos = 1,
.fast_loop_param.motor_encoder.rollover = pow(2,24),

.main_loop_param.position_controller_param.position.kp = 20,
.main_loop_param.position_controller_param.position.kd = .2,
.main_loop_param.position_controller_param.position.velocity_filter_frequency_hz = 200,
.main_loop_param.position_controller_param.position.command_max = 3,
.main_loop_param.velocity_controller_param.velocity.ki = 20,
.main_loop_param.velocity_controller_param.velocity.kp = .2,
.main_loop_param.velocity_controller_param.velocity.ki_limit = 3,
.main_loop_param.velocity_controller_param.velocity.velocity_filter_frequency_hz = 0,
.main_loop_param.velocity_controller_param.velocity.output_filter_frequency_hz = 200,
.main_loop_param.velocity_controller_param.velocity.command_max = 3,
.main_loop_param.velocity_controller_param.acceleration_limit = 1000,
.main_loop_param.joint_position_controller_param.velocity.velocity.ki = 20,
.main_loop_param.joint_position_controller_param.velocity.velocity.kp = .2,
.main_loop_param.joint_position_controller_param.velocity.velocity.ki_limit = 3,
.main_loop_param.joint_position_controller_param.velocity.velocity.velocity_filter_frequency_hz = 0,
.main_loop_param.joint_position_controller_param.velocity.velocity.output_filter_frequency_hz = 200,
.main_loop_param.joint_position_controller_param.velocity.velocity.command_max = 3,
.main_loop_param.joint_position_controller_param.kpj = 100,
.main_loop_param.output_encoder.cpr = 65536,
.main_loop_param.output_encoder.dir = -1,
.main_loop_param.no_latch_driver_fault = 1,
.main_loop_param.vbus_min = -1,
.startup_param.phase_lock_current = 4,
.startup_param.motor_encoder_startup = ENCODER_VALUE,
.startup_param.output_encoder_rollover = M_PI,
.name = "tmotor_driver",
.can_id = 0x01,
.fast_loop_param.motor_encoder.cpr = 65536,
};
