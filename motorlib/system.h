#pragma once

#ifdef __cplusplus
#include "parameter_api.h"
#include "param.h"
#include <cstring>

extern uint32_t t_exec_fastloop;
extern uint32_t t_exec_mainloop;
extern uint32_t t_period_fastloop;
extern uint32_t t_period_mainloop;

template<typename Actuator, typename Communication>
class System {
 public:
    static void init() {
        actuator_.set_param(param()->fast_loop_param, param()->main_loop_param);
    }
    static void run() {
        actuator_.start();

        send_string("finished startup");

        ParameterAPI api;
        api.add_api_variable("kp", new APIFloat(&actuator_.main_loop_.controller_.kp_));
        api.add_api_variable("kd", new APIFloat(&actuator_.main_loop_.controller_.kd_));
        api.add_api_variable("t_exec_fastloop", new APIUint32(&t_exec_fastloop));
        api.add_api_variable("t_exec_mainloop", new APIUint32(&t_exec_mainloop));
        api.add_api_variable("t_period_fastloop", new APIUint32(&t_period_fastloop));
        api.add_api_variable("t_period_mainloop", new APIUint32(&t_period_mainloop));
        while(1) {
            char *s = System::get_string();
            if (s != NULL) {
                System::send_string(api.parse_string(s).c_str());
            }
            actuator_.maintenance();
        }
    }
    static void main_loop_interrupt() {
        actuator_.main_loop_.update();
    }
    static void fast_loop_interrupt() {
        actuator_.fast_loop_.update();
    }
    static void usb_interrupt() {
        usb_.interrupt();
    }
    static void send_string(const char * str) {
        if (!usb_.tx_active(1)) {
            usb_.send_data(1, (const uint8_t *) str, std::strlen(str)+1, false);
        }
    }
    static char *get_string() {
        static char buf[64];
        int count = usb_.receive_data(1, (uint8_t *) buf, 64);
        buf[count] = 0;
        if (count) {
            return buf;
        } else {
            return NULL;
        }
    }
// private:
    static Communication usb_;
    static Actuator actuator_;
};

extern "C" {
#endif // __cplusplus

void system_init();
void system_run();
void main_loop_interrupt();
void fast_loop_interrupt();
void usb_interrupt();

#ifdef __cplusplus
}
#endif
