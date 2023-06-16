#include "param_obot_g474_motor_hands.h"

#include "math.h"

#define H1 0
#define H2 1
#define CS_GAIN_MOD 0

// Can be written by external methods, e.g. bootloader
const volatile Param __attribute__ ((section ("flash_param"))) param_store = {
    .name = "test",
    .main_loop_param.output_encoder.cpr = 1,
    .fast_loop_param.motor_encoder.cpr = pow(2,16),
    .fast_loop_param.motor_encoder.rollover = pow(2,25),
    /* .fast_loop_param.vbus_gain = 3.3/4096*(100+6.34)/6.34, */
    .fast_loop_param.vbus_gain = 1.0/4096*(100+6.34)/6.34,
#if CS_GAIN_MOD
    .fast_loop_param.adc1_gain = 3.3/4096*(9200/3000.0),  // A/count
    .fast_loop_param.adc2_gain = 3.3/4096*(9200/3000.0),
    .fast_loop_param.adc3_gain = 3.3/4096*(9200/3000.0),
#else
    .fast_loop_param.adc1_gain = 3.3/4096*(9200/1000.0),  // A/count
    .fast_loop_param.adc2_gain = 3.3/4096*(9200/1000.0),
    .fast_loop_param.adc3_gain = 3.3/4096*(9200/1000.0),
#endif

    .main_loop_param.no_latch_driver_fault = 1,



#if H1
    .name = "h1_test",
    /*************************************************************

      H1 Motor
      
      L_1kHz = 1.09mH
      L_10kHz = 1.2mH
      L_100kHz = 1.00mH
      DCR = 6.7ohm

    *************************************************************/

    /*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
    /* *      CURRENT CONTROL */
    /* *++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

    .fast_loop_param.foc_param.pi_d.kp=15,
    .fast_loop_param.foc_param.pi_d.ki=1.5,
    .fast_loop_param.foc_param.pi_d.ki_limit=35,
    .fast_loop_param.foc_param.pi_d.command_max=40,
    .fast_loop_param.foc_param.pi_q.kp=15,
    .fast_loop_param.foc_param.pi_q.ki=1.5,
    .fast_loop_param.foc_param.pi_q.ki_limit=35,
    .fast_loop_param.foc_param.pi_q.command_max=40,
    .fast_loop_param.foc_param.current_filter_frequency_hz=20000,
    .fast_loop_param.foc_param.num_poles = 7,
    .fast_loop_param.phase_mode = 0,
    /* .fast_loop_param.motor_encoder.index_electrical_offset_pos = */ 

    /*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
    /* *      POSITION CONTROL */
    /* *++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/ 
    .main_loop_param.position_controller_param.position.kp = 2.5, 
    .main_loop_param.position_controller_param.position.kd = .005, 
    .main_loop_param.position_controller_param.position.command_max = 1, 
    .main_loop_param.position_controller_param.position.output_filter_frequency_hz = 500.0, 
    .main_loop_param.position_controller_param.position.velocity_filter_frequency_hz = 500.0, 
     
    /*/1*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
    /* *      VELOCITY CONTROL */
    /* *++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

    .main_loop_param.velocity_controller_param.velocity.ki = 2.5,
    .main_loop_param.velocity_controller_param.velocity.kp = 0.005,
    .main_loop_param.velocity_controller_param.velocity.ki_limit = 0.95,
    .main_loop_param.velocity_controller_param.velocity.output_filter_frequency_hz = 300,
    .main_loop_param.velocity_controller_param.velocity.command_max = 1,
    .main_loop_param.velocity_controller_param.acceleration_limit = 10000,

    .startup_param.phase_lock_current = -1,

#elif H2
    .name = "h2_test",
    /*************************************************************
      H2 Motor

      L_1kHz = 4.7mH
      L_10kHz = 4.6mH
      L_100kHz = 4mH
      DCR = 29ohm
      FSW = 50kHz
    *************************************************************/
    
     /* .startup_param.phase_lock_current = 0.1, */



    /*/1*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
    /* *      CURRENT CONTROL */
    /* *++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
    .fast_loop_param.foc_param.pi_d.kp=80, 
    .fast_loop_param.foc_param.pi_d.ki=5,
    .fast_loop_param.foc_param.pi_d.ki_limit=35,
    .fast_loop_param.foc_param.pi_d.command_max=40,
    .fast_loop_param.foc_param.pi_q.kp=80,
    .fast_loop_param.foc_param.pi_q.ki=5,
    .fast_loop_param.foc_param.pi_q.ki_limit=35,
    .fast_loop_param.foc_param.pi_q.command_max=40,
    .fast_loop_param.foc_param.current_filter_frequency_hz=10000,
    .fast_loop_param.foc_param.num_poles = 7,
    .fast_loop_param.phase_mode = 1,
    /* .fast_loop_param.motor_encoder.use_index_electrical_offset_pos=1, */
    /* .fast_loop_param.motor_encoder.index_electrical_offset_pos = 8572.0, */
    /*//  .fast_loop_param.motor_encoder.dir =1, */

    /*/1*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
    /* *      POSITION CONTROL */
    /* *++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/ 
    .main_loop_param.position_controller_param.position.kd = .002, 
    .main_loop_param.position_controller_param.position.kp = .5, 
    .main_loop_param.position_controller_param.position.command_max = 0.3, 
    .main_loop_param.position_controller_param.position.output_filter_frequency_hz = 500.0, 
    .main_loop_param.position_controller_param.position.velocity_filter_frequency_hz = 500.0, 
     
    /*/1*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
    /* *      VELOCITY CONTROL */
    /* *++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

    .main_loop_param.velocity_controller_param.velocity.ki = .5,
    .main_loop_param.velocity_controller_param.velocity.kp = 0.002,
    .main_loop_param.velocity_controller_param.velocity.ki_limit = .25,
    .main_loop_param.velocity_controller_param.velocity.output_filter_frequency_hz = 300,
    .main_loop_param.velocity_controller_param.velocity.command_max = .3,
    .main_loop_param.velocity_controller_param.acceleration_limit = 10000,
    
    .startup_param.phase_lock_current = 0.3,

#endif


#ifdef PARAM_OVERRIDES
    PARAM_OVERRIDES
#endif
};

const volatile char * const name = param_store.name;
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdiscarded-qualifiers"
const Param * const param = &param_store; // todo figure out a way to not inline without warning
#pragma GCC diagnostic pop
