module;
#include <cstddef>
#include <cstdint>
#include "st_device.h"

export module power.g5_i2c;

export {
// An I2C master of the stm32g5 for chips with registers, polled: a transfer
// of a few bytes at 400 kHz takes a tenth of a millisecond, and the power
// board's main loop has nothing that cannot wait that long. Every wait has a
// limit, so a chip that is missing, or a bus held low, costs time and not the
// loop. (This belongs in motorlib/device once a second board uses it.)
struct I2CInit {
    uint8_t instance = 2;               // I2C2
    uint32_t kernel_clock_hz = 0;       // the APB1 clock, unless the RCC is told otherwise: up to 128 MHz
    uint32_t speed_hz = 400'000;        // 100'000 or 400'000
    uint32_t timeout_cycles = 0;        // of the cpu's cycle counter, for one byte: a few bit times at least
};

namespace i2c_bit {
inline constexpr uint32_t CR1_PE = 1u << 0;
inline constexpr uint32_t CR2_RD_WRN = 1u << 10, CR2_START = 1u << 13, CR2_STOP = 1u << 14, CR2_NBYTES_Pos = 16, CR2_AUTOEND = 1u << 25;
inline constexpr uint32_t ISR_TXIS = 1u << 1, ISR_RXNE = 1u << 2, ISR_NACKF = 1u << 4, ISR_STOPF = 1u << 5, ISR_TC = 1u << 6;
inline constexpr uint32_t ISR_BERR = 1u << 8, ISR_ARLO = 1u << 9, ISR_BUSY = 1u << 15;
inline constexpr uint32_t ICR_ALL = ISR_NACKF | ISR_STOPF | ISR_BERR | ISR_ARLO | 1u << 10 | 1u << 12;
static_assert(CR2_START == I2C1_I2C_CR2_START_Msk && CR2_AUTOEND == 1u << I2C1_I2C_CR2_AUTOEND_Pos && CR2_NBYTES_Pos == I2C1_I2C_CR2_NBYTES_Pos);
static_assert(ISR_TXIS == I2C1_I2C_ISR_TXIS_Msk && ISR_TC == 1u << I2C1_I2C_ISR_TC_Pos && ISR_BUSY == 1u << I2C1_I2C_ISR_BUSY_Pos);
}

// TIMINGR: the clock divided down to 8 MHz or, where it does not divide, to
// the next below that, and then the values of ST's tables for 8 MHz (standard
// mode 100 kHz, fast mode 400 kHz; analog filter on). With a clock that does
// not divide the bus is a few per cent slower than its name, never faster.
void i2c_timing_not_possible();     // not constexpr: calling it fails the constant evaluation
inline consteval uint32_t i2c_timing(uint32_t kernel_clock_hz, uint32_t speed_hz) {
    const uint32_t prescaler = (kernel_clock_hz + 7'999'999) / 8'000'000;
    // the kernel clock: 8 to 128 MHz; the speed: 100 or 400 kHz
    if (kernel_clock_hz < 8'000'000 || prescaler > 16 || (speed_hz != 400'000 && speed_hz != 100'000)) i2c_timing_not_possible();
    if (speed_hz == 400'000) return (prescaler - 1) << 28 | 0x3u << 20 | 0x1u << 16 | 0x03u << 8 | 0x09u;
    return (prescaler - 1) << 28 | 0x4u << 20 | 0x2u << 16 | 0x1Fu << 8 | 0x27u;
}

template <const I2CInit c>
inline I2C1_Type& i2c_regs() {
    static_assert(c.instance == 2, "I2C2");
    return *I2C2;
}

template <const I2CInit c>
inline void device_i2c_config() {
    constexpr uint32_t timing = i2c_timing(c.kernel_clock_hz, c.speed_hz);
    I2C1_Type& regs = i2c_regs<c>();
    regs.I2C_CR1 = 0;
    regs.I2C_TIMINGR = timing;
    regs.I2C_CR1 = i2c_bit::CR1_PE;
}

// waits for one of `flags`; false after the time limit or on a NACK or bus error that was not waited for
template <const I2CInit c>
inline bool i2c_wait(uint32_t flags) {
    using namespace i2c_bit;
    static_assert(c.timeout_cycles != 0);
    I2C1_Type& regs = i2c_regs<c>();
    const uint32_t start = DWT->CYCCNT;
    for (;;) {
        const uint32_t status = regs.I2C_ISR;
        if (status & flags) return true;
        if (status & (ISR_NACKF | ISR_BERR | ISR_ARLO)) return false;
        if (DWT->CYCCNT - start > c.timeout_cycles) return false;
    }
}

// After a transfer that did not go through: the flags cleared, and the
// peripheral through a disable, which puts its state machine back at the
// start and lets go of the lines
template <const I2CInit c>
inline void i2c_recover() {
    I2C1_Type& regs = i2c_regs<c>();
    regs.I2C_ICR = i2c_bit::ICR_ALL;
    regs.I2C_CR1 = 0;
    (void)regs.I2C_CR1;         // PE has to be seen low for three APB clocks
    (void)regs.I2C_CR1;
    (void)regs.I2C_CR1;
    regs.I2C_CR1 = i2c_bit::CR1_PE;
}

// `count` bytes to a chip (the first usually a register number), then stop
template <const I2CInit c>
inline bool i2c_write(uint8_t address, const uint8_t* data, std::size_t count) {
    using namespace i2c_bit;
    I2C1_Type& regs = i2c_regs<c>();
    regs.I2C_CR2 = static_cast<uint32_t>(address) << 1 | static_cast<uint32_t>(count) << CR2_NBYTES_Pos | CR2_AUTOEND | CR2_START;
    for (std::size_t k = 0; k < count; k++) {
        if (!i2c_wait<c>(ISR_TXIS)) {
            i2c_recover<c>();
            return false;
        }
        regs.I2C_TXDR = data[k];
    }
    if (!i2c_wait<c>(ISR_STOPF)) {
        i2c_recover<c>();
        return false;
    }
    regs.I2C_ICR = ISR_STOPF;
    return true;
}

// `count` bytes of a chip's register `reg`: the register number written,
// then a repeated start and the read
template <const I2CInit c>
inline bool i2c_read(uint8_t address, uint8_t reg, uint8_t* data, std::size_t count) {
    using namespace i2c_bit;
    I2C1_Type& regs = i2c_regs<c>();
    regs.I2C_CR2 = static_cast<uint32_t>(address) << 1 | 1u << CR2_NBYTES_Pos | CR2_START;
    bool ok = i2c_wait<c>(ISR_TXIS);
    if (ok) {
        regs.I2C_TXDR = reg;
        ok = i2c_wait<c>(ISR_TC);
    }
    if (ok) {
        regs.I2C_CR2 = static_cast<uint32_t>(address) << 1 | CR2_RD_WRN | static_cast<uint32_t>(count) << CR2_NBYTES_Pos | CR2_AUTOEND | CR2_START;
        for (std::size_t k = 0; ok && k < count; k++) {
            ok = i2c_wait<c>(ISR_RXNE);
            if (ok) data[k] = static_cast<uint8_t>(regs.I2C_RXDR);
        }
    }
    if (ok) ok = i2c_wait<c>(ISR_STOPF);
    if (!ok) {
        i2c_recover<c>();
        return false;
    }
    regs.I2C_ICR = ISR_STOPF;
    return true;
}
}
