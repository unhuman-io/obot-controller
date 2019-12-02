#include "gpio.h"

GPIO::GPIO(GPIO_TypeDef &regs, uint8_t pin, Direction direction) :
    regs_(regs), mask_(1<<pin) {
        set_direction(direction);
    }

void GPIO::set() {
    regs_.BSRR |= mask_;
}

void GPIO::clear() {
    regs_.BSRR |= mask_ << 16;
}


// TODO
void GPIO::set_direction(Direction direction) {
    switch (direction) {
        default:
        case INPUT:
        break;
        case OUTPUT:
        break;
    }
}