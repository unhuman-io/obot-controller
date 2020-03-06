#pragma once

#ifdef __cplusplus
class USB1;
class HRPWM;
class QEPEncoder;
template<typename PWM, typename Encoder>
class FastLoop;
template<class FastLoop>
class MainLoop;
template<typename FastLoop, typename MainLoop>
class Actuator;

class System {
 public:
    static void init();
    static void run();
    static void main_loop_interrupt();
    static void fast_loop_interrupt();
    static void usb_interrupt();
    static void send_string(const char * str);
    static char *get_string();
// private:
    static USB1 usb_;
    static Actuator<FastLoop<HRPWM, QEPEncoder>, MainLoop<FastLoop<HRPWM, QEPEncoder>>> actuator_;
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
