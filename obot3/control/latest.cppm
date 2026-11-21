module;
#include <cstdint>

export module control.latest;

export {
// The newest value of a T, between two interrupt priorities (or an interrupt
// and the background) in either direction. The writer fills the slot that is
// not current and then makes it current with one store. A reader that
// interrupts the writer gets the previous value; a reader that is interrupted
// reads a slot the writer leaves alone until its second write, and repeats in
// that case. One writer; T is copied whole.
//
// A reader that is single stepped by a debugger while the writer runs on
// (debug_monitor.h) is interrupted by many writes at every instruction, and
// would repeat forever. After a few tries it reads with the interrupts off,
// which is also one step for the debugger.
#if defined(__arm__) && defined(__thumb2__)
inline uint32_t latest_interrupts_off() {
    uint32_t state;
    __asm__ volatile("mrs %0, primask\n cpsid i" : "=r"(state));
    return state;
}
inline void latest_interrupts_restore(uint32_t state) {
    __asm__ volatile("msr primask, %0" : : "r"(state));
}
#else
inline uint32_t latest_interrupts_off() { return 0; }
inline void latest_interrupts_restore(uint32_t) {}
#endif

template <typename T>
class Latest {
 public:
    constexpr Latest() = default;
    void write(const T& value) {
        const uint32_t sequence = sequence_;
        slots_[(sequence + 1) & 1] = value;
        sequence_ = sequence + 1;
    }
    T read() const {
        T value;
        for (int tries = 0; tries < 3; tries++) {
            const uint32_t sequence = sequence_;
            value = slots_[sequence & 1];
            if (sequence_ - sequence < 2) return value;
        }
        const uint32_t state = latest_interrupts_off();
        value = slots_[sequence_ & 1];
        latest_interrupts_restore(state);
        return value;
    }

 private:
    T slots_[2] = {};
    volatile uint32_t sequence_ = 0;
};
}
