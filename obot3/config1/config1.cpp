#include "system.h"
#include <atomic>
#include "config1.h"

using MySystem = System<Config>;

void tim1_isr() {
    std::atomic_signal_fence(std::memory_order_acq_rel);
    MySystem::FastLoop::update();
}

void tim2_isr() {
    std::atomic_signal_fence(std::memory_order_acq_rel);
    MySystem::MainLoop::update();
}