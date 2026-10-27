#pragma once
#include "gpio_init.h"
#include "rcc_init.h"

constexpr auto trace_gpio_init = [](){
    GPIOInit init = get_default_gpio_init();
    init.a[2].mode = GPIOMode::ALTERNATE;
    return init;
};
