#pragma once
#include <cstdint>
#include "../motorlib/bmi270_data.c"    // bmi270_config_file: the 8 K the chip is loaded with

// BMI270 accelerometer and gyro on spi. IO is the bus and its chip select:
//   static void transfer(const uint8_t* out, uint8_t* in, uint32_t length, const uint8_t* more, uint32_t more_length)
//       one frame: chip select low, length bytes out and in, then more_length
//       bytes out of more with the replies dropped, chip select high
//   static void delay_us(uint32_t us)      a few us between frames
//   static void transfer_start(const uint8_t* out, uint32_t length)    the same frame without the wait:
//   static bool transfer_ready()                                       it is through
//   static void transfer_end(uint8_t* in, uint32_t length)             chip select high, what came in
// A read is the register with bit 7 set, a dummy byte and then the data.
struct BMI270Data {
    int16_t acc[3];     // x, y, z: +-8 g full scale
    int16_t gyr[3];     // +-2000 deg/s full scale
};
constexpr float bmi270_g_per_count = 8.f / 32768;
constexpr float bmi270_rad_s_per_count = 2000.f / 32768 * 3.14159265f / 180;
constexpr uint8_t bmi270_chip_id = 0x24, bmi270_init_ok = 0x01;

template<typename IO>
inline uint8_t bmi270_read(uint8_t address) {
    const uint8_t out[3] = {static_cast<uint8_t>(address | 0x80)};
    uint8_t in[3];
    IO::transfer(out, in, 3, nullptr, 0);
    return in[2];
}

template<typename IO>
inline void bmi270_write(uint8_t address, uint8_t value) {
    const uint8_t out[2] = {address, value};
    uint8_t in[2];
    IO::transfer(out, in, 2, nullptr, 0);
    IO::delay_us(3);
}

// Bringing it up is five steps with a wait after the first four, which is
// left to the caller:
//   bmi270_reset         a reset of the cpu alone leaves the chip as it was set   2 ms
//   bmi270_spi           out of reset the chip is on i2c until it sees a frame    1 ms
//   bmi270_init_start    the chip id, and power save off                          450 us
//   bmi270_init_load     the configuration, about 15 ms on the bus                20 ms
//   bmi270_init_finish   returns the chip id << 8 | the internal status,
//                        bmi270_chip_id and bmi270_init_ok when it worked, with
//                        both sensors running at 400 Hz output
constexpr uint32_t bmi270_reset_us = 2000, bmi270_spi_us = 1000, bmi270_power_up_us = 450, bmi270_load_us = 20000;

template<typename IO>
inline void bmi270_reset() {
    bmi270_read<IO>(0x00);                  // onto spi, should it be out of a power up
    IO::delay_us(200);
    bmi270_write<IO>(0x7E, 0xB6);           // cmd: soft reset
}

template<typename IO>
inline void bmi270_spi() {
    bmi270_read<IO>(0x00);                  // the chip select edge is what it takes; what it answers is nothing
}

template<typename IO>
inline uint8_t bmi270_init_start() {
    const uint8_t chip_id = bmi270_read<IO>(0x00);
    bmi270_write<IO>(0x7C, 0x00);           // power conf: advanced power save off
    return chip_id;
}

template<typename IO>
inline void bmi270_init_load() {
    bmi270_write<IO>(0x59, 0x00);           // init ctrl: load
    const uint8_t address = 0x5E;
    uint8_t in;
    IO::transfer(&address, &in, 1, bmi270_config_file, sizeof(bmi270_config_file));
    bmi270_write<IO>(0x59, 0x01);           // done
}

template<typename IO>
inline uint32_t bmi270_init_finish(uint8_t chip_id) {
    const uint8_t status = bmi270_read<IO>(0x21) & 0xF;
    bmi270_write<IO>(0x7D, 0x0E);           // power ctrl: accelerometer, gyro and temperature on
    bmi270_write<IO>(0x40, 0xAA);           // accelerometer: 400 Hz, normal filter, performance mode
    bmi270_write<IO>(0x41, 0x02);           // +-8 g
    bmi270_write<IO>(0x42, 0xE9);           // gyro: 400 Hz, normal filter, performance mode
    bmi270_write<IO>(0x43, 0x00);           // +-2000 deg/s
    bmi270_write<IO>(0x7C, 0x02);           // power conf: fifo self wake up only
    return chip_id << 8 | status;
}

// A reading in two halves, for a bus that moves the frame on its own: start,
// then end once IO::transfer_ready()
template<typename IO>
inline void bmi270_data_start() {
    const uint8_t out[14] = {0x80 | 0x0C};
    IO::transfer_start(out, sizeof(out));
}

template<typename IO>
inline BMI270Data bmi270_data_end() {
    uint8_t in[14];
    IO::transfer_end(in, sizeof(in));
    BMI270Data data;
    for (int i = 0; i < 3; i++) {
        data.acc[i] = static_cast<int16_t>(in[3 + 2 * i] << 8 | in[2 + 2 * i]);
        data.gyr[i] = static_cast<int16_t>(in[9 + 2 * i] << 8 | in[8 + 2 * i]);
    }
    return data;
}
