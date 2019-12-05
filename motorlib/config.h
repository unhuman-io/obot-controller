#ifndef CONFIG_H
#define CONFIG_H

#ifdef __cplusplus
#include "fast_loop.h"
#include "main_loop.h"

template <class PWM, class Encoder>
class Actuator;

template <class PWM, class Encoder>
struct Config {
    Config();
    FastLoop<PWM, Encoder> &fast_loop;
    MainLoop &main_loop;
    Actuator<PWM, Encoder> &actuator;
};

extern const Config config;

extern "C" {
#endif // __cplusplus
    void actuator_run();
    void usb_interrupt();

#ifdef __cplusplus
}
#endif
#endif
