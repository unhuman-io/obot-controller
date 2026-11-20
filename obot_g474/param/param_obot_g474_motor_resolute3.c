#include "param_obot_g474.h"
#include "math.h"


// Can be written by external methods, e.g. bootloader
const Param __attribute__ ((section ("flash_param"))) param_store = {
#include "param_default.h"
    .fast_loop_param.foc_param.pi_d.kp=5,
    .fast_loop_param.foc_param.pi_d.ki=.5,
    // .fast_loop_param.foc_param.pi_d.kp=5, // hd
    // .fast_loop_param.foc_param.pi_d.ki=0.4,
    .fast_loop_param.foc_param.pi_d.ki_limit=8,
    .fast_loop_param.foc_param.pi_d.command_max=10,
    .fast_loop_param.foc_param.pi_q.kp=5,
    .fast_loop_param.foc_param.pi_q.ki=.5,
    .fast_loop_param.foc_param.pi_q.ki_limit=8,
    .fast_loop_param.foc_param.pi_q.command_max=10,
    .fast_loop_param.foc_param.current_filter_frequency_hz=3000,//35000,
    .fast_loop_param.foc_param.num_poles = 20,
    .main_loop_param.position_controller_param.position.kp = 100,
    .main_loop_param.position_controller_param.position.kd = 0.5,
    .main_loop_param.position_controller_param.position.command_max = 4,
    .main_loop_param.output_encoder.cpr = 1ull<<32,
    .fast_loop_param.motor_encoder.cpr = (1ull<<32)-1,
    .fast_loop_param.phase_mode = 1,
    .fast_loop_param.motor_encoder.use_index_electrical_offset_pos = 1,
    .startup_param.motor_encoder_startup = ENCODER_BIAS_FROM_OUTPUT_WITH_MOTOR_CORRECTION,

    .fast_loop_param.adc1_gain = -3.3/4096/(.0005*40*1.0),  // A/count, 1.04 is a measured factor
    .fast_loop_param.adc2_gain = -3.3/4096/(.0005*40*1.0),
    .fast_loop_param.adc3_gain = -3.3/4096/(.0005*40*1.0),
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
        (6<<11) | 0x2C0, // csa_reg     0x2C0 -> bidirectional current, 40V/V
        //(6<<11) | 0x280,
        //(6<<11) | 0x240, // csa_reg     0x240 -> bidirectional current, 10V/V
    },
    .name = "res_max",
#ifdef PARAM_OVERRIDES
    PARAM_OVERRIDES
#endif
};
