#include "actuator.h"
#include "util.h"
#include "param.h"
#include "fast_loop.h"
#include "main_loop.h"
#include "config.h"
#include <cstdio>
#include "parameter_api.h"

extern uint32_t t_exec_fastloop;
extern uint32_t t_exec_mainloop;
extern uint32_t t_period_fastloop;
extern uint32_t t_period_mainloop;

void Actuator::run() {
    // zero current sensors in voltage mode to try to eliminate bias from pwm noise, could also do open mode
    fast_loop_.voltage_mode();
    uint32_t t_start = get_clock();
    while ((get_clock() - t_start)/170e6 < 2) {
        fast_loop_.zero_current_sensors();
    }

    if (param()->startup_param.do_phase_lock) {
        fast_loop_.phase_lock_mode(param()->startup_param.phase_lock_current);
        ms_delay(1000*param()->startup_param.phase_lock_duration);
    }
    fast_loop_.maintenance();  // TODO better way than calling this to update zero pos
    //main_loop_.set_mode(param()->startup_param.startup_mode);
    fast_loop_.current_mode();
    fast_loop_.set_iq_des(0);

    send_string("finished startup");

    FastLoopStatus fast_loop_status;
    ParameterAPI api;
    api.add_api_variable("kp", new APIFloat(&main_loop_.controller_.kp_));
    api.add_api_variable("kd", new APIFloat(&main_loop_.controller_.kd_));
    api.add_api_variable("t_exec_fastloop", new APIUint32(&t_exec_fastloop));
    api.add_api_variable("t_exec_mainloop", new APIUint32(&t_exec_mainloop));
    api.add_api_variable("t_period_fastloop", new APIUint32(&t_period_fastloop));
    api.add_api_variable("t_period_mainloop", new APIUint32(&t_period_mainloop));
    while(1) {
       // fast_loop_.get_status(&fast_loop_status);
        char *s = get_string();
        if (s != NULL) {
            send_string(api.parse_string(s).c_str());
        }
     //   send_string(buf);
        fast_loop_.maintenance();
      //  fast_loop_.set_param(param()->fast_loop_param);   // to help with debugging
      //  main_loop_.set_param(param()->main_loop_param);
    }
}