#ifndef CONFIG_H
#define CONFIG_H

#ifdef __cplusplus
#include "fast_loop.h"
#include "main_loop.h"
#include "hall.h"
#include "qep_encoder.h"
//#include "actuator.h"
#include "peripheral/stm32g4/hrpwm.h"

template<class MotorEncoder, class PWM> class Actuator;

struct Config {
    Config();
    FastLoop<QEPEncoder, HRPWM> &fast_loop;
    MainLoop &main_loop;
    Actuator<QEPEncoder, HRPWM> &actuator;
};

extern const Config config;

extern "C" {
#endif // __cplusplus
    void actuator_run();
    void usb_interrupt();
    void send_string(const char * str);

#ifdef __cplusplus
}
#endif
#endif
