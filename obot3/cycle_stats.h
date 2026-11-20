#pragma once
#include <atomic>
#include <cstdint>

// How long each interrupt handler and each task of the background takes, in
// cpu cycles, and how often it runs: what motorlib/interrupt_profiler.h does
// for the obot_g474. A CycleTimer on the stack times the scope it is in into
// a CycleStats.
//
// The time is net of what interrupted it: every timed scope adds its own net
// time to cycle_stats_stolen when it ends, and a scope's clock is the cpu's
// minus that, so the scopes that ran inside it don't count. An interrupt
// without a timer counts as part of what it interrupted. The period is from
// start to start.
//
// CYCLE_STATS 0 takes all of it out.
#ifndef CYCLE_STATS
#define CYCLE_STATS 1
#endif

struct CycleStats {
    const char* name;
    uint32_t min = 0xFFFFFFFF, max = 0;     // net cycles of a run
    uint32_t period_min = 0xFFFFFFFF, period_max = 0;
    uint64_t sum = 0;                       // net cycles of all runs
    uint32_t count = 0;
    uint32_t last_start = 0;
    bool started = false;
};

inline std::atomic<uint32_t> cycle_stats_stolen{0};

inline uint32_t cycle_stats_clock(uint32_t clock()) {
    uint32_t stolen, now;
    do {
        stolen = cycle_stats_stolen.load(std::memory_order_relaxed);
        now = clock();
    } while (stolen != cycle_stats_stolen.load(std::memory_order_relaxed));
    return now - stolen;
}

// highest: nothing timed interrupts this scope, which saves it the stolen
// cycles on the way in and out
template<uint32_t clock(), bool highest = false>
struct CycleTimer {
#if CYCLE_STATS
    [[gnu::always_inline]] explicit CycleTimer(CycleStats& stats) : stats_(stats) {
        const uint32_t wall = clock();
        if (stats.started) {
            const uint32_t period = wall - stats.last_start;
            if (period < stats.period_min) stats.period_min = period;
            if (period > stats.period_max) stats.period_max = period;
        }
        stats.started = true;
        stats.last_start = wall;
        start_ = highest ? wall : cycle_stats_clock(clock);
    }
    [[gnu::always_inline]] ~CycleTimer() {
        const uint32_t net = (highest ? clock() : cycle_stats_clock(clock)) - start_;
        cycle_stats_stolen.fetch_add(net, std::memory_order_relaxed);
        if (net < stats_.min) stats_.min = net;
        if (net > stats_.max) stats_.max = net;
        stats_.sum += net;
        stats_.count = stats_.count + 1;
    }
 private:
    CycleStats& stats_;
    uint32_t start_;
#else
    explicit CycleTimer(CycleStats&) {}
#endif
    CycleTimer(const CycleTimer&) = delete;
};

// The numbers since the last call, with the interrupts off for the copy
template<typename Lock>
inline CycleStats cycle_stats_take(CycleStats& stats) {
    const Lock lock;
    const CycleStats taken = stats;
    stats.min = stats.period_min = 0xFFFFFFFF;
    stats.max = stats.period_max = 0;
    stats.sum = 0;
    stats.count = 0;
    return taken;
}
