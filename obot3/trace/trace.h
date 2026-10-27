#pragma once
import device.gpio_init;
import device.rcc_init;

constexpr auto trace_gpio_init = [](){
    GPIOInit init = get_default_gpio_init();
    init.a[2].mode = GPIOMode::ALTERNATE;
    return init;
};
