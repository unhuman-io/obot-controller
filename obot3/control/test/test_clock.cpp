// Host test of the clocks, no hardware: control/test/run.sh
// The cycle counter is a variable here, which the test moves on.
#include <chrono>
#include <cstdint>
#include <cstdio>
#include <random>
import control.clock;

static int failures = 0;
#define CHECK(condition, ...) \
    do { \
        const bool ok = (condition); \
        if (!ok) failures++; \
        std::printf("%s  ", ok ? "pass" : "FAIL"); \
        std::printf(__VA_ARGS__); \
        std::printf("\n"); \
    } while (0)

static uint64_t counter;        // the cycles since the counter started, of which the clock sees 32 bits
static uint32_t cycles() { return static_cast<uint32_t>(counter); }

// A clock at a frequency against the time worked out in 64 bits, over many
// turns of the counter
template <uint32_t frequency_hz>
static void test_steady(uint64_t first_count) {
    using Clock = CycleClock<frequency_hz, cycles>;
    static_assert(std::chrono::is_clock_v<Clock> && Clock::is_steady);
    std::mt19937_64 random(frequency_hz);
    counter = first_count;
    Clock::start();
    const auto expected = [&](uint64_t count) {
        const uint64_t elapsed = count - first_count;
        return ClockTime{static_cast<uint32_t>(elapsed / frequency_hz),
                         static_cast<uint32_t>(static_cast<unsigned __int128>(elapsed % frequency_hz) * 1'000'000'000 / frequency_hz)};
    };
    int wrong = 0, wrong_at = 0, wrong_now = 0, backwards = 0;
    ClockTime last{};
    const uint64_t far = 0x80000000 - frequency_hz - 1000;      // cycles: 11 s at 170 MHz
    for (int i = 0; i < 2'000'000; i++) {
        // mostly short steps, as a loop's, and now and then most of the 2^31 cycles without an update
        const uint64_t step = random() % 1000 == 0 ? random() % far : random() % (frequency_hz / 5);
        counter += step;
        const ClockTime time = Clock::time();
        if (time != expected(counter)) wrong++;
        if (clock_difference(time, last) < 0) backwards++;
        last = time;
        if (Clock::now().time_since_epoch() != time.since_epoch()) wrong_now++;
        // a count from as far back, and one a little ahead
        const uint64_t back = random() % far;
        if (counter - first_count > back && Clock::time_at(static_cast<uint32_t>(counter - back)) != expected(counter - back)) wrong_at++;
        if (Clock::time_at(static_cast<uint32_t>(counter + 1000)) != expected(counter + 1000)) wrong_at++;
        Clock::update();
        if (Clock::time() != expected(counter)) wrong++;
    }
    CHECK(wrong == 0 && wrong_at == 0 && wrong_now == 0 && backwards == 0,
          "steady clock at %u Hz from count %llu: %u s, %d wrong, %d wrong for an earlier count, %d wrong as chrono, %d backwards",
          frequency_hz, static_cast<unsigned long long>(first_count), last.seconds, wrong, wrong_at, wrong_now, backwards);
}

static void test_synced() {
    using Steady = CycleClock<170'000'000, cycles>;
    using Synced = SyncedClock<Steady>;
    static_assert(std::chrono::is_clock_v<Synced> && !Synced::is_steady);
    std::mt19937_64 random(1);
    counter = 0xFFF00000;
    Steady::start();
    counter += 3 * 170'000'000ull + 12345;
    CHECK(Synced::time() == Steady::time() && Synced::count() == 0, "the host's time is the steady clock's until it is set");

    // A host whose time gains 30 ppm on the node's, with settings a second
    // apart that are up to 20 us late
    const double rate = 30e-6;
    const ClockTime host_start{1'794'949'634, 877'750'705};
    const uint64_t count_start = counter;
    const auto host_at = [&](uint64_t count) {
        const double elapsed_ns = static_cast<double>(count - count_start) * (1e9 / 170e6);
        return clock_add(host_start, static_cast<int64_t>(elapsed_ns * (1 + rate)));
    };
    int64_t worst_early = 0, worst_late = 0;
    for (int i = 0; i < 300; i++) {
        const uint64_t late = random() % 3400;     // cycles
        const ClockTime steady = Steady::time_at(static_cast<uint32_t>(counter + late));
        const int64_t step = Synced::set(steady, host_at(counter));
        if (i == 0) CHECK(Synced::time_at(static_cast<uint32_t>(counter + late)) == host_at(counter), "set to the host's time");
        if (i >= 150) {     // the rate has settled: what it is off by after a second is the lateness
            if (step < worst_early) worst_early = step;
            if (step > worst_late) worst_late = step;
        }
        for (int k = 0; k < 10; k++) {
            counter += 17'000'000;
            Steady::update();
        }
    }
    CHECK(Synced::rate_ppb() > 29'000 && Synced::rate_ppb() < 31'000, "rate %d ppb for a host 30000 ppb fast", Synced::rate_ppb());
    CHECK(worst_early > -25'000 && worst_late < 25'000, "off by %lld to %lld ns after a second, with settings up to 20000 ns late",
          static_cast<long long>(worst_early), static_cast<long long>(worst_late));
    const auto now = Synced::now();
    CHECK(std::chrono::duration_cast<std::chrono::seconds>(now.time_since_epoch()).count() == Synced::time().seconds,
          "chrono seconds %lld", static_cast<long long>(std::chrono::duration_cast<std::chrono::seconds>(now.time_since_epoch()).count()));

    // Without settings the rate carries on, and stops being extrapolated after rate_span_s
    const uint64_t count_last = counter;
    for (int i = 0; i < 10; i++) {
        counter += 10 * 170'000'000ull;
        Steady::update();
    }
    int64_t error = clock_difference(Synced::time(), host_at(counter));
    CHECK(error > -200'000 && error < 200'000, "off by %lld ns 100 s after the last setting", static_cast<long long>(error));
    for (int i = 0; i < 400; i++) {
        counter += 10 * 170'000'000ull;
        Steady::update();
    }
    error = clock_difference(Synced::time(), host_at(counter));
    const int64_t unextrapolated = static_cast<int64_t>((static_cast<double>(counter - count_last) / 170e6 - Synced::rate_span_s) * rate * 1e9);
    CHECK(error + unextrapolated > -2'000'000 && error + unextrapolated < 2'000'000,
          "off by %lld ns after 4100 s without one, %lld of it past the rate's span", static_cast<long long>(error), static_cast<long long>(-unextrapolated));

    // The host's time is set back an hour: the clock follows at once
    const ClockTime earlier = clock_add(host_at(counter), -3600ll * 1'000'000'000);
    Synced::set(Steady::time(), earlier);
    CHECK(Synced::time() == earlier, "follows the host's time back an hour");
}

static void test_frames() {
    using Steady = CycleClock<170'000'000, cycles>;
    using Synced = SyncedClock<Steady>;
    using Receiver = TimeSyncReceiver<Synced>;
    counter += 170'000'000;
    Steady::update();
    const uint32_t count_before = Synced::count();
    const ClockTime received = Steady::time();
    // sequence 5, sent at 1'800'000'002.123456789 s
    const uint8_t sync[1] = {5};
    const TimeSyncFollowUp follow_up_frame = {.sequence = 5, .seconds = 1'800'000'002, .nanoseconds = 123'456'789};
    const uint8_t* follow_up = reinterpret_cast<const uint8_t*>(&follow_up_frame);
    // little endian on the bus: the seconds are 0x6B49D202
    CHECK(follow_up[0] == 5 && follow_up[4] == 0x02 && follow_up[7] == 0x6B && follow_up[8] == 0x15, "the bytes of a follow up");
    CHECK(!Receiver::frame(sync, 1, received), "a SYNC alone sets nothing");
    counter += 170'000;     // 1 ms
    CHECK(Receiver::frame(follow_up, 12, Steady::time()) && Synced::count() == count_before + 1, "its follow up sets the clock");
    const ClockTime time = Synced::time_at(static_cast<uint32_t>(counter - 170'000));
    CHECK(time.seconds == 1'800'000'002 && time.nanoseconds == 123'456'789, "to %u.%09u for the start of the SYNC", time.seconds, time.nanoseconds);

    const TimeSyncFollowUp other_sequence_frame = {.sequence = 6, .seconds = 1'800'000'003};
    const uint8_t* other_sequence = reinterpret_cast<const uint8_t*>(&other_sequence_frame);
    Receiver::frame(sync, 1, Steady::time());
    CHECK(!Receiver::frame(other_sequence, 12, Steady::time()) && Receiver::dropped() == 1, "a follow up of another sequence is dropped");
    CHECK(!Receiver::frame(follow_up, 12, Steady::time()), "a follow up without a SYNC is dropped");
    Receiver::frame(sync, 1, Steady::time());
    counter += 170'000'000 / 5;
    CHECK(!Receiver::frame(follow_up, 12, Steady::time()), "a follow up 200 ms after its SYNC is dropped");
    Receiver::frame(sync, 1, Steady::time());
    CHECK(!Receiver::frame(follow_up, 8, Steady::time()) && !Receiver::frame(follow_up, 16, Steady::time()), "a frame of another length is not taken");
}

int main() {
    test_steady<170'000'000>(0);
    test_steady<170'000'000>(0xFFFFFF00);       // the counter turns over right away
    test_steady<160'000'000>(12345);
    test_steady<168'000'000>(1ull << 31);
    test_steady<480'000'001>(77);               // nanoseconds by the 64 bit path
    test_synced();
    test_frames();
    std::printf("%d failure%s\n", failures, failures == 1 ? "" : "s");
    return failures != 0;
}
