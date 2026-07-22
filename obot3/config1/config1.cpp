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
    MySystem::FastLoop::update();
}

void tim2_isr() {
    std::atomic_signal_fence(std::memory_order_acq_rel);
    MySystem::MainLoop::update();
}

constexpr auto get_gpio_init = [](){
    GPIOInit init = get_default_gpio_init();
    init.a[2].mode = GPIOMode::ALTERNATE;
    return init;
};

void pin_config() {
    g474_pin_config<get_gpio_init()>();
}