#include "system.h"
#include <atomic>
#include "config1.h"

template<auto config = Config1{}>
struct Config : BasicConfig<config> {
    int another_special_parameter = 12;
    volatile float voltage_sensor;
};

using MySystem = System<Config<>>;

void tim1_isr() {
    std::atomic_signal_fence(std::memory_order_acq_rel);
    MySystem::fast_loop.update();
}

void tim2_isr() {
    std::atomic_signal_fence(std::memory_order_acq_rel);
    MySystem::main_loop.update();
}

void clock_config() {
    g474_rcc_clock_config<RCCClockInit{}>();
}

void pin_config() {
    g474_rcc_config<config1_rcc_init>();
    g474_pin_config<config1_gpio_init>();
}
