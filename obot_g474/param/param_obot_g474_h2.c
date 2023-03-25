#include "param_obot_g474_h2.h"
#include "math.h"


// Can be written by external methods, e.g. bootloader
const volatile Param __attribute__ ((section ("flash_param"))) param_store = {
    .name = "test",
    .main_loop_param.output_encoder.cpr = 1,
    .fast_loop_param.motor_encoder.cpr = pow(2,16),
    .fast_loop_param.motor_encoder.rollover = pow(2,25),
    .fast_loop_param.vbus_gain = 1.0/4096*(100+6.34)/6.34,
    .fast_loop_param.adc1_gain = 3.3/4096*(9200/1000.0),  // A/count
    .fast_loop_param.adc2_gain = 3.3/4096*(9200/1000.0),
    .fast_loop_param.adc3_gain = 3.3/4096*(9200/1000.0),
    .main_loop_param.no_latch_driver_fault = 1,


    /*************************************************************
      H1 Motor
      
      L_1kHz = 1.09mH
      L_10kHz = 1.2mH
      L_100kHz = 1.00mH
      DCR = 6.7ohm

    *************************************************************/
    
    /* .fast_loop_param.foc_param.pi_d.kp=10, */
    /* .fast_loop_param.foc_param.pi_d.ki=1.5, */
    /* .fast_loop_param.foc_param.pi_d.ki_limit=28, */
    /* .fast_loop_param.foc_param.pi_d.command_max=30, */
    /* .fast_loop_param.foc_param.pi_q.kp=10, */
    /* .fast_loop_param.foc_param.pi_q.ki=1.5, */
    /* .fast_loop_param.foc_param.pi_q.ki_limit=28, */
    /* .fast_loop_param.foc_param.pi_q.command_max=30, */
    /* .fast_loop_param.foc_param.current_filter_frequency_hz=20000, */
    /* .fast_loop_param.foc_param.num_poles = 7, */
    /* /1* .fast_loop_param.motor_encoder.index_electrical_offset_pos = *1/ */ 
    
    /*************************************************************
      H2 Motor

      L_1kHz = 4.7mH
      L_10kHz = 4.6mH
      L_100kHz = 4mH
      DCR = 29ohm
    *************************************************************/
    
    .fast_loop_param.foc_param.pi_d.kp=45,
    .fast_loop_param.foc_param.pi_d.ki=5.0,
    .fast_loop_param.foc_param.pi_d.ki_limit=24,
    .fast_loop_param.foc_param.pi_d.command_max=25,
    .fast_loop_param.foc_param.pi_q.kp=45,
    .fast_loop_param.foc_param.pi_q.ki=5.0,
    .fast_loop_param.foc_param.pi_q.ki_limit=24,
    .fast_loop_param.foc_param.pi_q.command_max=25,
    .fast_loop_param.foc_param.current_filter_frequency_hz=20000,
    .fast_loop_param.foc_param.num_poles = 7,
    .fast_loop_param.phase_mode = 1,
    /* .fast_loop_param.motor_encoder.index_electrical_offset_pos = */ 


#ifdef PARAM_OVERRIDES
    PARAM_OVERRIDES
#endif
};

const volatile char * const name = param_store.name;
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdiscarded-qualifiers"
const Param * const param = &param_store; // todo figure out a way to not inline without warning
#pragma GCC diagnostic pop
