#pragma once
#include "g474_gpio_init.h"
#include "g474_rcc_init.h"

constexpr auto trace_gpio_init = [](){
    GPIOInit init = get_default_gpio_init();
    init.a[2].mode = GPIOMode::ALTERNATE;
    return init;
};
