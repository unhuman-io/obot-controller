#ifndef GPIO_H
#define GPIO_H

#include <cstdint>
#include "../st_device.h"    //TODO remove

class GPIO {
 public:
    enum Direction {INPUT, OUTPUT};
    GPIO(GPIO_TypeDef &regs, uint8_t pin, Direction direction = INPUT);
    void set();
    void clear();
    uint8_t get_value() const;
    void set_direction(Direction);
 private:
    GPIO_TypeDef &regs_;
    uint32_t mask_;
    uint8_t pin_;
};

#endif
