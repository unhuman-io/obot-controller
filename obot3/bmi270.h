#pragma once
#include <cstdint>
#include "../motorlib/task.h"
#include "../motorlib/bmi270_data.c"    // bmi270_config_file: the 8 K the chip is loaded with

// BMI270 accelerometer and gyro on spi, from the tasks of a scheduler: every
// frame is a co_await, so nothing here keeps the cpu. IO is the bus and the
// chip select:
//   static auto lock(CycleScheduler&)      awaited before a frame: the bus is this chip's
//   static void unlock()
//   static void transfer_start(const uint8_t* out, uint8_t* in, uint32_t length)
//       chip select low, length bytes out and in; in may be nullptr
//   static auto done(CycleScheduler&)      awaited: the frame is through
//   static void transfer_end()             chip select high
// A read is the register with bit 7 set, a dummy byte and then the data.
//
// These are GlobalCall coroutines, with one static frame each, so one call at
// a time. bmi270_init, bmi270_data and bmi270_get are for any task: they take
// the bus for each frame, and the lock is also what keeps the frames of the
// calls below them, bmi270_transfer, bmi270_read and bmi270_write, to one
// caller. Those are for a caller that has the lock. The frame sizes are what gcc and clang
// need and a third more; promise_type::frame_size has what a build asked for.
struct BMI270Data {
    int16_t acc[3];     // x, y, z: +-8 g full scale
    int16_t gyr[3];     // +-2000 deg/s full scale
};
constexpr float bmi270_g_per_count = 8.f / 32768;
constexpr float bmi270_rad_s_per_count = 2000.f / 32768 * 3.14159265f / 180;
constexpr uint8_t bmi270_chip_id = 0x24, bmi270_init_ok = 0x01;
inline constexpr int bmi270_task_id = 120;      // GlobalCall instances from here

// The configuration as the frame that loads it: the register, then the 8 K
constexpr auto bmi270_load_frame = [] {
    struct { uint8_t bytes[1 + sizeof(bmi270_config_file)]; } frame{};
    frame.bytes[0] = 0x5E;
    for (uint32_t i = 0; i < sizeof(bmi270_config_file); i++) frame.bytes[1 + i] = bmi270_config_file[i];
    return frame;
}();

template<typename IO>
inline GlobalCall<void, bmi270_task_id, 96> bmi270_transfer(CycleScheduler& sched, const uint8_t* out, uint8_t* in, uint32_t length) {
    IO::transfer_start(out, in, length);
    co_await IO::done(sched);
    IO::transfer_end();
}

template<typename IO>
inline GlobalCall<uint8_t, bmi270_task_id + 1, 96> bmi270_read(CycleScheduler& sched, uint8_t address) {
    uint8_t out[3] = {static_cast<uint8_t>(address | 0x80)};
    uint8_t in[3];
    co_await bmi270_transfer<IO>(sched, out, in, 3);
    co_return in[2];
}

template<typename IO>
inline GlobalCall<void, bmi270_task_id + 2, 96> bmi270_write(CycleScheduler& sched, uint8_t address, uint8_t value) {
    uint8_t out[2] = {address, value};
    co_await bmi270_transfer<IO>(sched, out, nullptr, 2);
    co_await sched.async_delay_us(3);       // between a write and the next frame
}

// A register, from any task
template<typename IO>
inline GlobalCall<uint8_t, bmi270_task_id + 5, 96> bmi270_get(CycleScheduler& sched, uint8_t address) {
    co_await IO::lock(sched);
    const uint8_t value = co_await bmi270_read<IO>(sched, address);
    IO::unlock();
    co_return value;
}

// Brings the chip up, 35 ms: returns the chip id << 8 | the internal status,
// bmi270_chip_id and bmi270_init_ok when it worked, with both sensors running
// at 400 Hz output. The steps are a table, which is a fraction of the code
// that a co_await for each would be.
struct BMI270Step {
    enum Kind : uint8_t { READ, WRITE, LOAD, WAIT } kind;
    uint8_t address, value;
    uint16_t wait_us;       // after it
};
inline constexpr BMI270Step bmi270_init_steps[] = {
    {BMI270Step::READ, 0x00, 0, 200},       // onto spi, should it be out of a power up
    {BMI270Step::WRITE, 0x7E, 0xB6, 2000},  // cmd: soft reset. A reset of the cpu alone leaves the chip as it was set
    {BMI270Step::READ, 0x00, 0, 1000},      // out of reset the chip is on i2c until it sees a frame: what it answers is nothing
    {BMI270Step::READ, 0x00, 0, 0},         // the chip id
    {BMI270Step::WRITE, 0x7C, 0x00, 450},   // power conf: advanced power save off
    {BMI270Step::WRITE, 0x59, 0x00, 0},     // init ctrl: load
    {BMI270Step::LOAD, 0, 0, 0},            // the configuration, 12 ms on the bus
    {BMI270Step::WRITE, 0x59, 0x01, 20000}, // done
    {BMI270Step::READ, 0x21, 0, 0},         // the internal status
    {BMI270Step::WRITE, 0x7D, 0x0E, 0},     // power ctrl: accelerometer, gyro and temperature on
    {BMI270Step::WRITE, 0x40, 0xAA, 0},     // accelerometer: 400 Hz, normal filter, performance mode
    {BMI270Step::WRITE, 0x41, 0x02, 0},     // +-8 g
    {BMI270Step::WRITE, 0x42, 0xE9, 0},     // gyro: 400 Hz, normal filter, performance mode
    {BMI270Step::WRITE, 0x43, 0x00, 0},     // +-2000 deg/s
    {BMI270Step::WRITE, 0x7C, 0x02, 0},     // power conf: fifo self wake up only
};
inline constexpr uint32_t bmi270_chip_id_step = 3, bmi270_status_step = 8;

template<typename IO>
inline GlobalCall<uint32_t, bmi270_task_id + 3, 160> bmi270_init(CycleScheduler& sched) {
    uint32_t chip_id = 0, status = 0;
    for (uint32_t i = 0; i < sizeof(bmi270_init_steps) / sizeof(bmi270_init_steps[0]); i++) {
        const BMI270Step& step = bmi270_init_steps[i];
        co_await IO::lock(sched);
        if (step.kind == BMI270Step::READ) {
            const uint8_t value = co_await bmi270_read<IO>(sched, step.address);
            if (i == bmi270_chip_id_step) chip_id = value;
            if (i == bmi270_status_step) status = value & 0xF;
        } else if (step.kind == BMI270Step::WRITE) {
            co_await bmi270_write<IO>(sched, step.address, step.value);
        } else if (step.kind == BMI270Step::LOAD) {
            co_await bmi270_transfer<IO>(sched, bmi270_load_frame.bytes, nullptr, sizeof(bmi270_load_frame.bytes));
        }
        IO::unlock();
        if (step.wait_us != 0) co_await sched.async_delay_us(step.wait_us);
    }
    co_return chip_id << 8 | status;
}

template<typename IO>
inline GlobalCall<BMI270Data, bmi270_task_id + 4, 144> bmi270_data(CycleScheduler& sched) {
    uint8_t out[14] = {0x80 | 0x0C};
    uint8_t in[14];
    co_await IO::lock(sched);
    co_await bmi270_transfer<IO>(sched, out, in, 14);
    IO::unlock();
    BMI270Data data;
    for (int i = 0; i < 3; i++) {
        data.acc[i] = static_cast<int16_t>(in[3 + 2 * i] << 8 | in[2 + 2 * i]);
        data.gyr[i] = static_cast<int16_t>(in[9 + 2 * i] << 8 | in[8 + 2 * i]);
    }
    co_return data;
}
