#include "g474_pins.h"

struct {
    int a;
} g474;

void set_g474_pins(const G474GpioRegs &val) {
    g474.a = val.a_moder;
}
