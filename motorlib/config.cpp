#include "config.h"
#include "led.h"
#include "communication.h"
#include "control_fun.h"
#include "encoder.h"
#include "pwm.h"
#include "gpio.h"

#include "dev_00_aksim2.cpp"

const Config config;

Config::Config() :
    fast_loop(config_items.fast_loop),
    main_loop(config_items.main_loop)
    {}