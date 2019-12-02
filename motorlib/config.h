#ifndef CONFIG_H
#define CONFIG_H

#include "fast_loop.h"
#include "main_loop.h"

struct Config {
    Config();
    FastLoop &fast_loop;
    MainLoop &main_loop;
};

extern const Config config;
#endif
