module;
#include <atomic>
#include <cstdint>
#include <numeric>
#include "chrono_types.h"

export module control.clock;
import control.latest;

export {
// Time for timestamps, from the cpu's cycle counter. Two clocks, both
// std::chrono clocks with nanoseconds:
//
//   CycleClock    steady: the time since start(), which nothing sets
//   SyncedClock   a host's time, set to it over the bus: steps when it is
//                 set, and runs at the host's rate between
//
// now() is the chrono time_point. time() is the same as seconds and
// nanoseconds, a ClockTime, and the cheap one: the cycle count less a
// multiple of the frequency is the cycles into the second, so the second is
// all the state there is, one word that any interrupt priority reads, and
// neither part takes a 64 bit division. time_at(count) is the time of a
// cycle count that was taken a moment ago, in an interrupt or by a
// peripheral.
//
// update() of the steady clock moves the second on. Called more often than
// once a second, and from what never stops, it keeps time() to a few
// instructions. It has to be called before 2^31 cycles less a second have
// gone by (11 s at 170 MHz), and that is also how far back a count for
// time_at() may be.

struct ClockTime {
    uint32_t seconds = 0;
    uint32_t nanoseconds = 0;       // below 1e9
    constexpr std::chrono::nanoseconds since_epoch() const {
        return std::chrono::nanoseconds(static_cast<int64_t>(seconds) * 1'000'000'000 + nanoseconds);
    }
};
// (not defaulted in the struct: gcc 15 does not get that through a module)
constexpr bool operator==(const ClockTime& a, const ClockTime& b) { return a.seconds == b.seconds && a.nanoseconds == b.nanoseconds; }

// a - b in nanoseconds, for times within 2^31 s of each other
constexpr int64_t clock_difference(ClockTime a, ClockTime b) {
    return static_cast<int64_t>(static_cast<int32_t>(a.seconds - b.seconds)) * 1'000'000'000 +
        static_cast<int32_t>(a.nanoseconds - b.nanoseconds);
}

// Any number of nanoseconds later or earlier: a 64 bit division
constexpr ClockTime clock_add(ClockTime time, int64_t nanoseconds) {
    const int64_t total = static_cast<int64_t>(time.nanoseconds) + nanoseconds;
    int64_t seconds = total / 1'000'000'000, rest = total % 1'000'000'000;
    if (rest < 0) {
        rest += 1'000'000'000;
        seconds--;
    }
    return {time.seconds + static_cast<uint32_t>(seconds), static_cast<uint32_t>(rest)};
}

// cycles() is the counter: 32 bits that wrap, at frequency_hz
template<uint32_t frequency_hz, uint32_t cycles()>
class CycleClock {
 public:
    using rep = int64_t;
    using period = std::nano;
    using duration = std::chrono::nanoseconds;
    using time_point = std::chrono::time_point<CycleClock>;
    static constexpr bool is_steady = true;

    static time_point now() noexcept { return time_point(time().since_epoch()); }
    static ClockTime time() noexcept { return time_at(cycles()); }
    static ClockTime time_at(uint32_t count) noexcept {
        uint32_t seconds = seconds_.load(std::memory_order_relaxed);
        uint32_t in_second = count - origin_ - seconds * frequency_hz;
        if (in_second >= frequency_hz) [[unlikely]] {
            if (in_second < 0x80000000) {           // update() is due
                seconds += in_second / frequency_hz;
                in_second %= frequency_hz;
            } else {                                // a count from before the second began
                const uint32_t back = (0u - in_second + frequency_hz - 1) / frequency_hz;
                seconds -= back;
                in_second += back * frequency_hz;
            }
        }
        return {seconds, nanoseconds(in_second)};
    }

    // Time zero is now
    static void start() noexcept {
        origin_ = cycles();
        seconds_.store(0, std::memory_order_relaxed);
    }
    // From any priority, and from several: the second only moves on
    static void update() noexcept {
        uint32_t seconds = seconds_.load(std::memory_order_relaxed);
        const uint32_t in_second = cycles() - origin_ - seconds * frequency_hz;
        if (in_second >= frequency_hz && in_second < 0x80000000) {
            seconds_.compare_exchange_strong(seconds, seconds + in_second / frequency_hz, std::memory_order_relaxed);
        }
    }

    // Nanoseconds of a number of cycles below the frequency: the cycle is a
    // whole number of nanoseconds and a fraction, and the fraction's part
    // fits 32 bits for the usual frequencies
    static constexpr uint32_t nanoseconds(uint32_t in_second) noexcept {
        if (static_cast<uint64_t>(frequency_hz - 1) * ns_rest <= 0xFFFFFFFF) {
            return in_second * ns_whole + in_second * ns_rest / ns_den;
        }
        return static_cast<uint32_t>(static_cast<uint64_t>(in_second) * ns_num / ns_den);
    }

 private:
    static constexpr uint32_t ns_gcd = std::gcd(1'000'000'000u, frequency_hz);
    static constexpr uint32_t ns_num = 1'000'000'000u / ns_gcd, ns_den = frequency_hz / ns_gcd;    // nanoseconds per cycle
    static constexpr uint32_t ns_whole = ns_num / ns_den, ns_rest = ns_num % ns_den;
    static inline constinit std::atomic<uint32_t> seconds_{0};
    static inline constinit uint32_t origin_ = 0;
};

// The time of a host on a steady clock: set(steady, host) says what the
// host's time was at a time of the steady clock, and from then on it is that
// plus what the steady clock has gone on by, at the rate that the host's
// time has been found to run at against it. Until it is set it is the steady
// clock's time. One writer, set(); readers at any priority.
//
// The rate is measured between two settings that are a while apart, the
// longer the more of them there have been, since what is known of the host's
// time at a setting is only good to some microseconds. For the same reason
// the clock, once it has the rate, goes a part of the way to each setting: it
// keeps time over the seconds between them better than one of them tells it,
// and one that is off by more, a frame that had to wait for the bus, moves
// it by that part only.
template<typename Steady>
class SyncedClock {
 public:
    using rep = int64_t;
    using period = std::nano;
    using duration = std::chrono::nanoseconds;
    using time_point = std::chrono::time_point<SyncedClock>;
    static constexpr bool is_steady = false;

    static time_point now() noexcept { return time_point(time().since_epoch()); }
    static ClockTime time() noexcept { return from_steady(Steady::time()); }
    static ClockTime time_at(uint32_t count) noexcept { return from_steady(Steady::time_at(count)); }

    static ClockTime from_steady(ClockTime steady) noexcept {
        const Sync sync = sync_.read();
        const int32_t seconds = static_cast<int32_t>(steady.seconds - sync.steady.seconds);
        int32_t nanoseconds = static_cast<int32_t>(steady.nanoseconds - sync.steady.nanoseconds);
        // what the host's time has gained on the steady clock since: by the
        // whole seconds, as far as the rate is trusted, and by the rest
        const int32_t rate_seconds = seconds > rate_span_s ? rate_span_s : seconds < -rate_span_s ? -rate_span_s : seconds;
        const int32_t gained = rate_seconds * sync.rate_ppb +
            static_cast<int32_t>(static_cast<int64_t>(nanoseconds) * sync.rate_q32 >> 32);
        ClockTime host{sync.host.seconds + static_cast<uint32_t>(seconds), 0};
        nanoseconds += static_cast<int32_t>(sync.host.nanoseconds);         // within -1e9 and 2e9
        if (nanoseconds < 0) {
            nanoseconds += 1'000'000'000;
            host.seconds--;
        } else if (nanoseconds >= 1'000'000'000) {
            nanoseconds -= 1'000'000'000;
            host.seconds++;
        }
        nanoseconds += gained;                                              // at most 1.1e9 either way
        while (nanoseconds < 0) {
            nanoseconds += 1'000'000'000;
            host.seconds--;
        }
        while (nanoseconds >= 1'000'000'000) {
            nanoseconds -= 1'000'000'000;
            host.seconds++;
        }
        host.nanoseconds = static_cast<uint32_t>(nanoseconds);
        return host;
    }

    // The host's time was `host` at `steady`. Returns what the clock was off
    // by, host's time minus its own, in nanoseconds.
    static int64_t set(ClockTime steady, ClockTime host) noexcept {
        const ClockTime own = from_steady(steady);
        const int64_t step = clock_difference(host, own);
        Sync sync = sync_.read();
        ClockTime to = host;
        if (count_ == 0 || step > step_limit_ns || step < -step_limit_ns) {
            // the first time, or the host's has been set: the rate is measured from here
            reference_steady_ = steady;
            reference_host_ = host;
            window_ns_ = min_window_ns;
        } else if (const int64_t span = clock_difference(steady, reference_steady_); span >= window_ns_) {
            const int64_t measured = (clock_difference(host, reference_host_) - span) * 1'000'000'000 / span;
            int64_t rate = rates_ == 0 ? measured : sync.rate_ppb + (measured - sync.rate_ppb) / 2;
            if (rate > max_rate_ppb) rate = max_rate_ppb;
            if (rate < -max_rate_ppb) rate = -max_rate_ppb;
            sync.rate_ppb = static_cast<int32_t>(rate);
            sync.rate_q32 = static_cast<int32_t>(rate * 4'294'967'296 / 1'000'000'000);
            rates_++;
            reference_steady_ = steady;
            reference_host_ = host;
            if (window_ns_ < max_window_ns) window_ns_ *= 2;
        }
        if (rates_ != 0 && step <= step_limit_ns && step >= -step_limit_ns) to = clock_add(own, step / step_part);
        sync.steady = steady;
        sync.host = to;
        sync_.write(sync);
        last_step_ns_ = step > INT32_MAX ? INT32_MAX : step < INT32_MIN ? INT32_MIN : static_cast<int32_t>(step);
        count_ = count_ + 1;
        return step;
    }

    static uint32_t count() noexcept { return count_; }             // times set
    static int32_t last_step_ns() noexcept { return last_step_ns_; }       // what it was off by the last time, at most 2.1 s either way
    static int32_t rate_ppb() noexcept { return sync_.read().rate_ppb; }   // the host's time against the steady clock
    static ClockTime last_set() noexcept { return sync_.read().steady; }   // when, on the steady clock

    static constexpr int32_t max_rate_ppb = 500'000;        // 500 ppm, more than a crystal is off by
    static constexpr int32_t rate_span_s = 2000;            // without a setting for this long, the rate is no longer extrapolated
    static constexpr int64_t step_limit_ns = 1'000'000;     // off by more is not the rate: the clock is set to the host's time outright
    static constexpr int64_t step_part = 4;                 // and by less it goes this part of the way, once it has the rate
    static constexpr int64_t min_window_ns = 2'000'000'000, max_window_ns = 64'000'000'000;

 private:
    struct Sync {
        ClockTime steady{}, host{};
        int32_t rate_ppb = 0;       // the host's time gains this on the steady clock, parts per billion
        int32_t rate_q32 = 0;       // the same as a fraction of 2^32
    };
    static inline constinit Latest<Sync> sync_{};
    static inline constinit ClockTime reference_steady_{}, reference_host_{};
    static inline constinit int64_t window_ns_ = min_window_ns;
    static inline constinit int32_t last_step_ns_ = 0;
    static inline constinit uint32_t count_ = 0, rates_ = 0;
};

// A host's time over can, in the frames of AUTOSAR's time synchronization
// over CAN (CanTSyn, SWS_CanTSyn_00015 and 00016: the SYNC and FUP messages
// that are not CRC secured): the host sends SYNC with the seconds,
// finds out when exactly that frame was on the bus and says so in a follow
// up. A node notes when the SYNC arrived, on its steady clock, and knows the
// host's time for that moment when the follow up is in. 8 bytes each:
//   SYNC        0x10, user byte, domain << 4 | sequence, user byte, seconds (big endian)
//   follow up   0x18, user byte, domain << 4 | sequence, overflow of seconds in bits 1..0, nanoseconds (big endian)
// The time of the SYNC is seconds + overflow + nanoseconds.
template<typename Synced, uint8_t domain = 0>
class TimeSyncReceiver {
 public:
    static constexpr uint8_t sync_type = 0x10, follow_up_type = 0x18;
    static constexpr int64_t follow_up_timeout_ns = 100'000'000;

    // A frame of the time id, which started at `received` on the steady
    // clock. True if it has set the clock.
    static bool frame(const uint8_t* data, int length, ClockTime received) noexcept {
        if (length != 8 || data[2] >> 4 != domain) return false;
        if (data[0] == sync_type) {
            if (pending_) dropped_ = dropped_ + 1;
            pending_ = true;
            sequence_ = data[2] & 0xF;
            seconds_ = big_endian(data + 4);
            received_ = received;
        } else if (data[0] == follow_up_type && pending_) {
            pending_ = false;
            const uint32_t nanoseconds = big_endian(data + 4);
            if ((data[2] & 0xF) != sequence_ || nanoseconds >= 1'000'000'000 ||
                    clock_difference(received, received_) > follow_up_timeout_ns) {
                dropped_ = dropped_ + 1;
                return false;
            }
            Synced::set(received_, {seconds_ + (data[3] & 3u), nanoseconds});
            return true;
        }
        return false;
    }
    static uint32_t dropped() noexcept { return dropped_; }     // a SYNC without its follow up

 private:
    static constexpr uint32_t big_endian(const uint8_t* bytes) {
        return static_cast<uint32_t>(bytes[0]) << 24 | static_cast<uint32_t>(bytes[1]) << 16 |
            static_cast<uint32_t>(bytes[2]) << 8 | bytes[3];
    }
    static inline constinit ClockTime received_{};
    static inline constinit uint32_t seconds_ = 0, dropped_ = 0;
    static inline constinit uint8_t sequence_ = 0;
    static inline constinit bool pending_ = false;
};
}
