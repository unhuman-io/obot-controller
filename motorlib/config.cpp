#include "config.h"
#include "led.h"
#include "communication.h"
#include "control_fun.h"
#include "encoder.h"
#include "pwm.h"
#include "gpio.h"

#include "dev_00_aksim2.cpp"
#include "actuator.h"

const Config config;
static Actuator actuator_ = {config_items.fast_loop, config_items.main_loop};

Config::Config() :
    fast_loop(config_items.fast_loop),
    main_loop(config_items.main_loop),
    actuator(actuator_)
    {}

void actuator_run() {
    actuator_.run();
}