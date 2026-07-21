#include <concepts>
#include <atomic>
#include "config1.h"


template<typename Cfg>
struct System {
    using MainLoop = typename Cfg::MainLoopType<System>;
    using FastLoop = typename Cfg::FastLoopType<System>;
};

using MySystem = System<Config>;

int main() {

    while(1) {
        std::atomic_signal_fence(std::memory_order_acq_rel);
        MySystem::FastLoop::update();
        MySystem::MainLoop::update();
    }
    return 0;
}

extern "C" {
void _close_r() {}
void _getpid_r() {}
void _kill_r() {}
void _lseek_r() {}
void _read_r() {}
void _write_r() {}
}
