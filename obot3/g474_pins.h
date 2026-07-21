#pragma once
#include "stm32g474_gpio_init.h"

struct G474Pins {
    static constexpr int gpio_a_moder = 0x1234;
};

struct G474GpioRegs {
    int a_moder;
};

void set_g474_pins(const G474GpioRegs &);

template<GPIOInit gpio_init>
void g474_pin_config() {
    // set_g474_pins({
    //     .a_moder = Pins::gpio_a_moder,
    // });
    
    init_gpio(get_gpio_regs_init(gpio_init));
}
