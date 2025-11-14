export module stm32g474_gpio_trace_blinker;

import stm32g474_gpio_init;

using enum GPIOMode;
using enum GPIOOType;
using enum GPIOSpeed;
using enum GPIOPullUpD;
using enum GPIOSet;
using enum GPIOLock;
using enum GPIOAF;

export constexpr const GPIOInit gpio_settings {
  .a = {
    {.name = "A0", .function = "gpio", .mode = ANALOG, .otype = PUSH_PULL, .speed = LOW, .pullupd = NO_PULL, .bsrr = NO_SET, .lck = NO_LOCK, .af = AF0},
    {.name = "A1", .function = "gpio", .mode = ANALOG, .otype = PUSH_PULL, .speed = LOW, .pullupd = NO_PULL, .bsrr = NO_SET, .lck = NO_LOCK, .af = AF0},
    {.name = "A2", .function = "gpio", .mode = ANALOG, .otype = PUSH_PULL, .speed = LOW, .pullupd = NO_PULL, .bsrr = NO_SET, .lck = NO_LOCK, .af = AF0},
    {.name = "A3", .function = "gpio", .mode = ANALOG, .otype = PUSH_PULL, .speed = LOW, .pullupd = NO_PULL, .bsrr = NO_SET, .lck = NO_LOCK, .af = AF0},
    {.name = "A4", .function = "gpio", .mode = ANALOG, .otype = PUSH_PULL, .speed = LOW, .pullupd = NO_PULL, .bsrr = NO_SET, .lck = NO_LOCK, .af = AF0},
    {.name = "A5", .function = "gpio", .mode = ANALOG, .otype = PUSH_PULL, .speed = LOW, .pullupd = NO_PULL, .bsrr = NO_SET, .lck = NO_LOCK, .af = AF0},
    {.name = "A6", .function = "gpio", .mode = ANALOG, .otype = PUSH_PULL, .speed = LOW, .pullupd = NO_PULL, .bsrr = NO_SET, .lck = NO_LOCK, .af = AF0},
    {.name = "A7", .function = "gpio", .mode = ANALOG, .otype = PUSH_PULL, .speed = LOW, .pullupd = NO_PULL, .bsrr = NO_SET, .lck = NO_LOCK, .af = AF0},
    {.name = "A8", .function = "gpio", .mode = ANALOG, .otype = PUSH_PULL, .speed = LOW, .pullupd = NO_PULL, .bsrr = NO_SET, .lck = NO_LOCK, .af = AF0},
    {.name = "A9", .function = "gpio", .mode = ANALOG, .otype = PUSH_PULL, .speed = LOW, .pullupd = NO_PULL, .bsrr = NO_SET, .lck = NO_LOCK, .af = AF0},
    {.name = "A10", .function = "usb_exti", .mode = INPUT, .otype = PUSH_PULL, .speed = LOW, .pullupd = NO_PULL, .bsrr = NO_SET, .lck = NO_LOCK, .af = AF0},
    {.name = "A11", .function = "usb_dm", .mode = ANALOG, .otype = PUSH_PULL, .speed = LOW, .pullupd = NO_PULL, .bsrr = NO_SET, .lck = NO_LOCK, .af = AF0},
    {.name = "A12", .function = "usb_dp", .mode = ANALOG, .otype = PUSH_PULL, .speed = LOW, .pullupd = NO_PULL, .bsrr = NO_SET, .lck = NO_LOCK, .af = AF0},
    {.name = "A13", .function = "swdio", .mode = ALTERNATE, .otype = PUSH_PULL, .speed = VERY_HIGH, .pullupd = PULL_UP, .bsrr = NO_SET, .lck = NO_LOCK, .af = AF0},
    {.name = "A14", .function = "swclk", .mode = ALTERNATE, .otype = PUSH_PULL, .speed = LOW, .pullupd = PULL_UP, .bsrr = NO_SET, .lck = NO_LOCK, .af = AF0},
    {.name = "A15", .function = "jtdi", .mode = ALTERNATE, .otype = PUSH_PULL, .speed = LOW, .pullupd = PULL_DOWN, .bsrr = NO_SET, .lck = NO_LOCK, .af = AF0}
  },
  .b = {
    {.name = "B0", .function = "gpio", .mode = ANALOG, .otype = PUSH_PULL, .speed = LOW, .pullupd = NO_PULL, .bsrr = NO_SET, .lck = NO_LOCK, .af = AF0},
    {.name = "B1", .function = "gpio", .mode = ANALOG, .otype = PUSH_PULL, .speed = LOW, .pullupd = NO_PULL, .bsrr = NO_SET, .lck = NO_LOCK, .af = AF0},
    {.name = "B2", .function = "gpio", .mode = ANALOG, .otype = PUSH_PULL, .speed = LOW, .pullupd = NO_PULL, .bsrr = NO_SET, .lck = NO_LOCK, .af = AF0},
    {.name = "B3", .function = "swo", .mode = ALTERNATE, .otype = PUSH_PULL, .speed = LOW, .pullupd = NO_PULL, .bsrr = NO_SET, .lck = NO_LOCK, .af = AF0},
    {.name = "B4", .function = "jtrst", .mode = ALTERNATE, .otype = PUSH_PULL, .speed = LOW, .pullupd = PULL_UP, .bsrr = NO_SET, .lck = NO_LOCK, .af = AF0},
    {.name = "B5", .function = "gpio", .mode = ANALOG, .otype = PUSH_PULL, .speed = LOW, .pullupd = NO_PULL, .bsrr = NO_SET, .lck = NO_LOCK, .af = AF0},
    {.name = "B6", .function = "gpio", .mode = ANALOG, .otype = PUSH_PULL, .speed = LOW, .pullupd = NO_PULL, .bsrr = NO_SET, .lck = NO_LOCK, .af = AF0},
    {.name = "B7", .function = "gpio", .mode = ANALOG, .otype = PUSH_PULL, .speed = LOW, .pullupd = NO_PULL, .bsrr = NO_SET, .lck = NO_LOCK, .af = AF0},
    {.name = "B8", .function = "gpio", .mode = ANALOG, .otype = PUSH_PULL, .speed = LOW, .pullupd = NO_PULL, .bsrr = NO_SET, .lck = NO_LOCK, .af = AF0},
    {.name = "B9", .function = "gpio", .mode = ANALOG, .otype = PUSH_PULL, .speed = LOW, .pullupd = NO_PULL, .bsrr = NO_SET, .lck = NO_LOCK, .af = AF0},
    {.name = "B10", .function = "gpio", .mode = ANALOG, .otype = PUSH_PULL, .speed = LOW, .pullupd = NO_PULL, .bsrr = NO_SET, .lck = NO_LOCK, .af = AF0},
    {.name = "B11", .function = "gpio", .mode = ANALOG, .otype = PUSH_PULL, .speed = LOW, .pullupd = NO_PULL, .bsrr = NO_SET, .lck = NO_LOCK, .af = AF0},
    {.name = "B12", .function = "gpio", .mode = ANALOG, .otype = PUSH_PULL, .speed = LOW, .pullupd = NO_PULL, .bsrr = NO_SET, .lck = NO_LOCK, .af = AF0},
    {.name = "B13", .function = "gpio", .mode = ANALOG, .otype = PUSH_PULL, .speed = LOW, .pullupd = NO_PULL, .bsrr = NO_SET, .lck = NO_LOCK, .af = AF0},
    {.name = "B14", .function = "gpio", .mode = ANALOG, .otype = PUSH_PULL, .speed = LOW, .pullupd = NO_PULL, .bsrr = NO_SET, .lck = NO_LOCK, .af = AF0},
    {.name = "B15", .function = "gpio", .mode = ANALOG, .otype = PUSH_PULL, .speed = LOW, .pullupd = NO_PULL, .bsrr = NO_SET, .lck = NO_LOCK, .af = AF0}
  },
  .c = {
    {.name = "C0", .function = "gpio", .mode = ANALOG, .otype = PUSH_PULL, .speed = LOW, .pullupd = NO_PULL, .bsrr = NO_SET, .lck = NO_LOCK, .af = AF0},
    {.name = "C1", .function = "gpio", .mode = ANALOG, .otype = PUSH_PULL, .speed = LOW, .pullupd = NO_PULL, .bsrr = NO_SET, .lck = NO_LOCK, .af = AF0},
    {.name = "C2", .function = "gpio", .mode = ANALOG, .otype = PUSH_PULL, .speed = LOW, .pullupd = NO_PULL, .bsrr = NO_SET, .lck = NO_LOCK, .af = AF0},
    {.name = "C3", .function = "gpio", .mode = ANALOG, .otype = PUSH_PULL, .speed = LOW, .pullupd = NO_PULL, .bsrr = NO_SET, .lck = NO_LOCK, .af = AF0},
    {.name = "C4", .function = "gpio", .mode = ANALOG, .otype = PUSH_PULL, .speed = LOW, .pullupd = NO_PULL, .bsrr = NO_SET, .lck = NO_LOCK, .af = AF0},
    {.name = "C5", .function = "gpio", .mode = ANALOG, .otype = PUSH_PULL, .speed = LOW, .pullupd = NO_PULL, .bsrr = NO_SET, .lck = NO_LOCK, .af = AF0},
    {.name = "C6", .function = "gpio", .mode = ANALOG, .otype = PUSH_PULL, .speed = LOW, .pullupd = NO_PULL, .bsrr = NO_SET, .lck = NO_LOCK, .af = AF0},
    {.name = "C7", .function = "gpio", .mode = ANALOG, .otype = PUSH_PULL, .speed = LOW, .pullupd = NO_PULL, .bsrr = NO_SET, .lck = NO_LOCK, .af = AF0},
    {.name = "C8", .function = "gpio", .mode = ANALOG, .otype = PUSH_PULL, .speed = LOW, .pullupd = NO_PULL, .bsrr = NO_SET, .lck = NO_LOCK, .af = AF0},
    {.name = "C9", .function = "gpio", .mode = ANALOG, .otype = PUSH_PULL, .speed = LOW, .pullupd = NO_PULL, .bsrr = NO_SET, .lck = NO_LOCK, .af = AF0},
    {.name = "C10", .function = "gpio", .mode = ANALOG, .otype = PUSH_PULL, .speed = LOW, .pullupd = NO_PULL, .bsrr = NO_SET, .lck = NO_LOCK, .af = AF0},
    {.name = "C11", .function = "gpio", .mode = ANALOG, .otype = PUSH_PULL, .speed = LOW, .pullupd = NO_PULL, .bsrr = NO_SET, .lck = NO_LOCK, .af = AF0},
    {.name = "C12", .function = "gpio", .mode = ANALOG, .otype = PUSH_PULL, .speed = LOW, .pullupd = NO_PULL, .bsrr = NO_SET, .lck = NO_LOCK, .af = AF0},
    {.name = "C13", .function = "gpio", .mode = ANALOG, .otype = PUSH_PULL, .speed = LOW, .pullupd = NO_PULL, .bsrr = NO_SET, .lck = NO_LOCK, .af = AF0},
    {.name = "C14", .function = "gpio", .mode = ANALOG, .otype = PUSH_PULL, .speed = LOW, .pullupd = NO_PULL, .bsrr = NO_SET, .lck = NO_LOCK, .af = AF0},
    {.name = "C15", .function = "gpio", .mode = ANALOG, .otype = PUSH_PULL, .speed = LOW, .pullupd = NO_PULL, .bsrr = NO_SET, .lck = NO_LOCK, .af = AF0}
  },
  .d = {
    {.name = "D0", .function = "gpio", .mode = ANALOG, .otype = PUSH_PULL, .speed = LOW, .pullupd = NO_PULL, .bsrr = NO_SET, .lck = NO_LOCK, .af = AF0},
    {.name = "D1", .function = "gpio", .mode = ANALOG, .otype = PUSH_PULL, .speed = LOW, .pullupd = NO_PULL, .bsrr = NO_SET, .lck = NO_LOCK, .af = AF0},
    {.name = "D2", .function = "gpio", .mode = ANALOG, .otype = PUSH_PULL, .speed = LOW, .pullupd = NO_PULL, .bsrr = NO_SET, .lck = NO_LOCK, .af = AF0},
    {.name = "D3", .function = "gpio", .mode = ANALOG, .otype = PUSH_PULL, .speed = LOW, .pullupd = NO_PULL, .bsrr = NO_SET, .lck = NO_LOCK, .af = AF0},
    {.name = "D4", .function = "gpio", .mode = ANALOG, .otype = PUSH_PULL, .speed = LOW, .pullupd = NO_PULL, .bsrr = NO_SET, .lck = NO_LOCK, .af = AF0},
    {.name = "D5", .function = "gpio", .mode = ANALOG, .otype = PUSH_PULL, .speed = LOW, .pullupd = NO_PULL, .bsrr = NO_SET, .lck = NO_LOCK, .af = AF0},
    {.name = "D6", .function = "gpio", .mode = ANALOG, .otype = PUSH_PULL, .speed = LOW, .pullupd = NO_PULL, .bsrr = NO_SET, .lck = NO_LOCK, .af = AF0},
    {.name = "D7", .function = "gpio", .mode = ANALOG, .otype = PUSH_PULL, .speed = LOW, .pullupd = NO_PULL, .bsrr = NO_SET, .lck = NO_LOCK, .af = AF0},
    {.name = "D8", .function = "gpio", .mode = ANALOG, .otype = PUSH_PULL, .speed = LOW, .pullupd = NO_PULL, .bsrr = NO_SET, .lck = NO_LOCK, .af = AF0},
    {.name = "D9", .function = "gpio", .mode = ANALOG, .otype = PUSH_PULL, .speed = LOW, .pullupd = NO_PULL, .bsrr = NO_SET, .lck = NO_LOCK, .af = AF0},
    {.name = "D10", .function = "gpio", .mode = ANALOG, .otype = PUSH_PULL, .speed = LOW, .pullupd = NO_PULL, .bsrr = NO_SET, .lck = NO_LOCK, .af = AF0},
    {.name = "D11", .function = "gpio", .mode = ANALOG, .otype = PUSH_PULL, .speed = LOW, .pullupd = NO_PULL, .bsrr = NO_SET, .lck = NO_LOCK, .af = AF0},
    {.name = "D12", .function = "gpio", .mode = ANALOG, .otype = PUSH_PULL, .speed = LOW, .pullupd = NO_PULL, .bsrr = NO_SET, .lck = NO_LOCK, .af = AF0},
    {.name = "D13", .function = "gpio", .mode = ANALOG, .otype = PUSH_PULL, .speed = LOW, .pullupd = NO_PULL, .bsrr = NO_SET, .lck = NO_LOCK, .af = AF0},
    {.name = "D14", .function = "gpio", .mode = ANALOG, .otype = PUSH_PULL, .speed = LOW, .pullupd = NO_PULL, .bsrr = NO_SET, .lck = NO_LOCK, .af = AF0},
    {.name = "D15", .function = "gpio", .mode = ANALOG, .otype = PUSH_PULL, .speed = LOW, .pullupd = NO_PULL, .bsrr = NO_SET, .lck = NO_LOCK, .af = AF0}
  },
  .e = {
    {.name = "E0", .function = "gpio", .mode = ANALOG, .otype = PUSH_PULL, .speed = LOW, .pullupd = NO_PULL, .bsrr = NO_SET, .lck = NO_LOCK, .af = AF0},
    {.name = "E1", .function = "gpio", .mode = ANALOG, .otype = PUSH_PULL, .speed = LOW, .pullupd = NO_PULL, .bsrr = NO_SET, .lck = NO_LOCK, .af = AF0},
    {.name = "E2", .function = "gpio", .mode = ANALOG, .otype = PUSH_PULL, .speed = LOW, .pullupd = NO_PULL, .bsrr = NO_SET, .lck = NO_LOCK, .af = AF0},
    {.name = "E3", .function = "gpio", .mode = ANALOG, .otype = PUSH_PULL, .speed = LOW, .pullupd = NO_PULL, .bsrr = NO_SET, .lck = NO_LOCK, .af = AF0},
    {.name = "E4", .function = "gpio", .mode = ANALOG, .otype = PUSH_PULL, .speed = LOW, .pullupd = NO_PULL, .bsrr = NO_SET, .lck = NO_LOCK, .af = AF0},
    {.name = "E5", .function = "gpio", .mode = ANALOG, .otype = PUSH_PULL, .speed = LOW, .pullupd = NO_PULL, .bsrr = NO_SET, .lck = NO_LOCK, .af = AF0},
    {.name = "E6", .function = "gpio", .mode = ANALOG, .otype = PUSH_PULL, .speed = LOW, .pullupd = NO_PULL, .bsrr = NO_SET, .lck = NO_LOCK, .af = AF0},
    {.name = "E7", .function = "gpio", .mode = ANALOG, .otype = PUSH_PULL, .speed = LOW, .pullupd = NO_PULL, .bsrr = NO_SET, .lck = NO_LOCK, .af = AF0},
    {.name = "E8", .function = "gpio", .mode = ANALOG, .otype = PUSH_PULL, .speed = LOW, .pullupd = NO_PULL, .bsrr = NO_SET, .lck = NO_LOCK, .af = AF0},
    {.name = "E9", .function = "gpio", .mode = ANALOG, .otype = PUSH_PULL, .speed = LOW, .pullupd = NO_PULL, .bsrr = NO_SET, .lck = NO_LOCK, .af = AF0},
    {.name = "E10", .function = "gpio", .mode = ANALOG, .otype = PUSH_PULL, .speed = LOW, .pullupd = NO_PULL, .bsrr = NO_SET, .lck = NO_LOCK, .af = AF0},
    {.name = "E11", .function = "gpio", .mode = ANALOG, .otype = PUSH_PULL, .speed = LOW, .pullupd = NO_PULL, .bsrr = NO_SET, .lck = NO_LOCK, .af = AF0},
    {.name = "E12", .function = "gpio", .mode = ANALOG, .otype = PUSH_PULL, .speed = LOW, .pullupd = NO_PULL, .bsrr = NO_SET, .lck = NO_LOCK, .af = AF0},
    {.name = "E13", .function = "gpio", .mode = ANALOG, .otype = PUSH_PULL, .speed = LOW, .pullupd = NO_PULL, .bsrr = NO_SET, .lck = NO_LOCK, .af = AF0},
    {.name = "E14", .function = "gpio", .mode = ANALOG, .otype = PUSH_PULL, .speed = LOW, .pullupd = NO_PULL, .bsrr = NO_SET, .lck = NO_LOCK, .af = AF0},
    {.name = "E15", .function = "gpio", .mode = ANALOG, .otype = PUSH_PULL, .speed = LOW, .pullupd = NO_PULL, .bsrr = NO_SET, .lck = NO_LOCK, .af = AF0}
  },
  .f = {
    {.name = "F0", .function = "gpio", .mode = ANALOG, .otype = PUSH_PULL, .speed = LOW, .pullupd = NO_PULL, .bsrr = NO_SET, .lck = NO_LOCK, .af = AF0},
    {.name = "F1", .function = "gpio", .mode = ANALOG, .otype = PUSH_PULL, .speed = LOW, .pullupd = NO_PULL, .bsrr = NO_SET, .lck = NO_LOCK, .af = AF0},
    {.name = "F2", .function = "gpio", .mode = ANALOG, .otype = PUSH_PULL, .speed = LOW, .pullupd = NO_PULL, .bsrr = NO_SET, .lck = NO_LOCK, .af = AF0},
    {.name = "F3", .function = "gpio", .mode = ANALOG, .otype = PUSH_PULL, .speed = LOW, .pullupd = NO_PULL, .bsrr = NO_SET, .lck = NO_LOCK, .af = AF0},
    {.name = "F4", .function = "gpio", .mode = ANALOG, .otype = PUSH_PULL, .speed = LOW, .pullupd = NO_PULL, .bsrr = NO_SET, .lck = NO_LOCK, .af = AF0},
    {.name = "F5", .function = "gpio", .mode = ANALOG, .otype = PUSH_PULL, .speed = LOW, .pullupd = NO_PULL, .bsrr = NO_SET, .lck = NO_LOCK, .af = AF0},
    {.name = "F6", .function = "gpio", .mode = ANALOG, .otype = PUSH_PULL, .speed = LOW, .pullupd = NO_PULL, .bsrr = NO_SET, .lck = NO_LOCK, .af = AF0},
    {.name = "F7", .function = "gpio", .mode = ANALOG, .otype = PUSH_PULL, .speed = LOW, .pullupd = NO_PULL, .bsrr = NO_SET, .lck = NO_LOCK, .af = AF0},
    {.name = "F8", .function = "gpio", .mode = ANALOG, .otype = PUSH_PULL, .speed = LOW, .pullupd = NO_PULL, .bsrr = NO_SET, .lck = NO_LOCK, .af = AF0},
    {.name = "F9", .function = "gpio", .mode = ANALOG, .otype = PUSH_PULL, .speed = LOW, .pullupd = NO_PULL, .bsrr = NO_SET, .lck = NO_LOCK, .af = AF0},
    {.name = "F10", .function = "gpio", .mode = ANALOG, .otype = PUSH_PULL, .speed = LOW, .pullupd = NO_PULL, .bsrr = NO_SET, .lck = NO_LOCK, .af = AF0},
    {.name = "F11", .function = "gpio", .mode = ANALOG, .otype = PUSH_PULL, .speed = LOW, .pullupd = NO_PULL, .bsrr = NO_SET, .lck = NO_LOCK, .af = AF0},
    {.name = "F12", .function = "gpio", .mode = ANALOG, .otype = PUSH_PULL, .speed = LOW, .pullupd = NO_PULL, .bsrr = NO_SET, .lck = NO_LOCK, .af = AF0},
    {.name = "F13", .function = "gpio", .mode = ANALOG, .otype = PUSH_PULL, .speed = LOW, .pullupd = NO_PULL, .bsrr = NO_SET, .lck = NO_LOCK, .af = AF0},
    {.name = "F14", .function = "gpio", .mode = ANALOG, .otype = PUSH_PULL, .speed = LOW, .pullupd = NO_PULL, .bsrr = NO_SET, .lck = NO_LOCK, .af = AF0},
    {.name = "F15", .function = "gpio", .mode = ANALOG, .otype = PUSH_PULL, .speed = LOW, .pullupd = NO_PULL, .bsrr = NO_SET, .lck = NO_LOCK, .af = AF0}
  },
  .g = {
    {.name = "G0", .function = "gpio", .mode = ANALOG, .otype = PUSH_PULL, .speed = LOW, .pullupd = NO_PULL, .bsrr = NO_SET, .lck = NO_LOCK, .af = AF0},
    {.name = "G1", .function = "gpio", .mode = ANALOG, .otype = PUSH_PULL, .speed = LOW, .pullupd = NO_PULL, .bsrr = NO_SET, .lck = NO_LOCK, .af = AF0},
    {.name = "G2", .function = "gpio", .mode = ANALOG, .otype = PUSH_PULL, .speed = LOW, .pullupd = NO_PULL, .bsrr = NO_SET, .lck = NO_LOCK, .af = AF0},
    {.name = "G3", .function = "gpio", .mode = ANALOG, .otype = PUSH_PULL, .speed = LOW, .pullupd = NO_PULL, .bsrr = NO_SET, .lck = NO_LOCK, .af = AF0},
    {.name = "G4", .function = "gpio", .mode = ANALOG, .otype = PUSH_PULL, .speed = LOW, .pullupd = NO_PULL, .bsrr = NO_SET, .lck = NO_LOCK, .af = AF0},
    {.name = "G5", .function = "gpio", .mode = ANALOG, .otype = PUSH_PULL, .speed = LOW, .pullupd = NO_PULL, .bsrr = NO_SET, .lck = NO_LOCK, .af = AF0},
    {.name = "G6", .function = "gpio", .mode = ANALOG, .otype = PUSH_PULL, .speed = LOW, .pullupd = NO_PULL, .bsrr = NO_SET, .lck = NO_LOCK, .af = AF0},
    {.name = "G7", .function = "gpio", .mode = ANALOG, .otype = PUSH_PULL, .speed = LOW, .pullupd = NO_PULL, .bsrr = NO_SET, .lck = NO_LOCK, .af = AF0},
    {.name = "G8", .function = "gpio", .mode = ANALOG, .otype = PUSH_PULL, .speed = LOW, .pullupd = NO_PULL, .bsrr = NO_SET, .lck = NO_LOCK, .af = AF0},
    {.name = "G9", .function = "gpio", .mode = ANALOG, .otype = PUSH_PULL, .speed = LOW, .pullupd = NO_PULL, .bsrr = NO_SET, .lck = NO_LOCK, .af = AF0},
    {.name = "G10", .function = "reset", .mode = ANALOG, .otype = PUSH_PULL, .speed = LOW, .pullupd = NO_PULL, .bsrr = NO_SET, .lck = NO_LOCK, .af = AF0},
    {.name = "G11", .function = "gpio", .mode = ANALOG, .otype = PUSH_PULL, .speed = LOW, .pullupd = NO_PULL, .bsrr = NO_SET, .lck = NO_LOCK, .af = AF0},
    {.name = "G12", .function = "gpio", .mode = ANALOG, .otype = PUSH_PULL, .speed = LOW, .pullupd = NO_PULL, .bsrr = NO_SET, .lck = NO_LOCK, .af = AF0},
    {.name = "G13", .function = "gpio", .mode = ANALOG, .otype = PUSH_PULL, .speed = LOW, .pullupd = NO_PULL, .bsrr = NO_SET, .lck = NO_LOCK, .af = AF0},
    {.name = "G14", .function = "gpio", .mode = ANALOG, .otype = PUSH_PULL, .speed = LOW, .pullupd = NO_PULL, .bsrr = NO_SET, .lck = NO_LOCK, .af = AF0},
    {.name = "G15", .function = "gpio", .mode = ANALOG, .otype = PUSH_PULL, .speed = LOW, .pullupd = NO_PULL, .bsrr = NO_SET, .lck = NO_LOCK, .af = AF0}
  },
};
