#include "system.h"
#include <atomic>
#include "config1.h"
#include "../motorlib/task.h"

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

CycleScheduler sched;
volatile uint32_t* const cpu_clock = &DWT->CYCCNT;
GlobalTask<void, 1, 36> task1(CycleScheduler &sched){
    volatile int count = 0;
    while(1) {
        count += 1;
        co_await sched.yield();
    }
}

auto task1_task = task1(sched);

GlobalTask<void, 2, 48> task2(CycleScheduler &sched){
    volatile int count2 = 0;
    constexpr uint32_t PERIOD_CYCLES = CPU_FREQUENCY_HZ / 10; // 10Hz
    uint32_t target_wake_time = get_clock();

    while (1) {
        target_wake_time += PERIOD_CYCLES;
        count2 += 1;
        co_await sched.delay_until(target_wake_time);
    }
}

auto task2_task = task2(sched);

void os_loop() {
    sched.poll();
}