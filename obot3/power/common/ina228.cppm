module;
#include <cstdint>

export module power.ina228;

export {
// TI INA228 power monitor (85 V, 20 bit), as far as the power board uses it:
// two registers are read, the voltage across the shunt and the voltage at the
// VBUS pin, and the current is worked out here from the shunt's resistance.
// The chip's own current, power and energy registers need its calibration
// register set to match; a division here needs nothing.
//
// A register is sent most significant byte first. The two results are 24 bit
// registers with the value in the upper 20 bits: 195.3125 uV a step for VBUS;
// for the shunt 312.5 nV a step in the +-163.84 mV range and 78.125 nV in the
// +-40.96 mV range (CONFIG ADCRANGE), two's complement.
namespace ina228 {
inline constexpr uint8_t CONFIG = 0x00, ADC_CONFIG = 0x01, VSHUNT = 0x04, VBUS = 0x05, DIETEMP = 0x06;
inline constexpr uint8_t MANUFACTURER_ID = 0x3E, DEVICE_ID = 0x3F;
inline constexpr uint16_t manufacturer_id = 0x5449;         // "TI"
inline constexpr uint16_t device_id_mask = 0xFFF0, device_id = 0x2280;
inline constexpr uint16_t config_reset = 1u << 15;
inline constexpr uint16_t config_range_40mv = 1u << 4;
// continuous shunt and bus conversions, 1052 us each, 16 averaged: a new pair every 34 ms
inline constexpr uint16_t adc_config = 0xB << 12 | 5 << 9 | 5 << 6 | 5 << 3 | 2;

struct Monitor {
    uint8_t address;            // 7 bits
    uint32_t shunt_uohm;
    bool range_40mv;            // the shunt never has more than 40 mV across it: four times the resolution
};

constexpr uint16_t config(const Monitor& m) { return m.range_40mv ? config_range_40mv : 0; }

// the VBUS register as millivolts
constexpr int32_t bus_mv(const uint8_t* raw) {
    const uint32_t code = (static_cast<uint32_t>(raw[0]) << 16 | static_cast<uint32_t>(raw[1]) << 8 | raw[2]) >> 4;
    return static_cast<int32_t>(static_cast<uint64_t>(code) * 3125 / 16000);        // x 195.3125 uV
}

// the VSHUNT register as milliamps through the monitor's shunt
constexpr int32_t current_ma(const Monitor& m, const uint8_t* raw) {
    int32_t code = static_cast<int32_t>(static_cast<uint32_t>(raw[0]) << 16 | static_cast<uint32_t>(raw[1]) << 8 | raw[2]);
    if (code & 0x800000) code -= 0x1000000;
    code /= 16;                                                             // the upper 20 bits (exact: the lower 4 are 0)
    const int64_t nv = static_cast<int64_t>(code) * (m.range_40mv ? 625 : 2500) / 8;       // x 78.125 or 312.5 nV
    return static_cast<int32_t>(nv / m.shunt_uohm);                         // nV / uOhm = mA
}
}
}
