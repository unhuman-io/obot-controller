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
template<typename T>
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
        uint32_t sequence;
        do {
            sequence = sequence_;
            value = slots_[sequence & 1];
        } while (sequence_ - sequence >= 2);
        return value;
    }
 private:
    T slots_[2] = {};
    volatile uint32_t sequence_ = 0;
};
}
