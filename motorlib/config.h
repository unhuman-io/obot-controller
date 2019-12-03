#ifndef CONFIG_H
#define CONFIG_H

#ifdef __cplusplus
#include "fast_loop.h"
#include "main_loop.h"

class Actuator;

struct Config {
    Config();
    FastLoop &fast_loop;
    MainLoop &main_loop;
    Actuator &actuator;
};

extern const Config config;

extern "C" {
#endif // __cplusplus
    void actuator_run();

#ifdef __cplusplus
}
#endif
#endif
