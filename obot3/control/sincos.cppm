module;
#include <cstdint>
#include <numbers>

export module control.sincos;

// sin and cos from a 512 entry table with a third order correction, as
// motorlib/sincos.cpp. The table is made at compile time.
export struct Sincos {
    float sin, cos;
};

namespace sincos_detail {
inline constexpr int table_length = 512;

// Taylor series, x in -pi..pi: only for building the table
constexpr double sin_series(double x) {
    double term = x, sum = x;
    for (int n = 1; n < 16; n++) {
        term *= -x * x / ((2 * n) * (2 * n + 1));
        sum += term;
    }
    return sum;
}

struct Table {
    float sin[table_length], cos[table_length];
};

constexpr Table make_table() {
    Table table{};
    for (int i = 0; i < table_length; i++) {
        // angle in -pi..pi for the series
        const double x = 2 * std::numbers::pi * (i < table_length / 2 ? i : i - table_length) / table_length;
        const double xc = 2 * std::numbers::pi * ((i + table_length / 4) % table_length < table_length / 2 ?
            (i + table_length / 4) % table_length : (i + table_length / 4) % table_length - table_length) / table_length;
        table.sin[i] = sin_series(x);
        table.cos[i] = sin_series(xc);    // cos(x) = sin(x + pi/2)
    }
    return table;
}

inline constexpr Table table = make_table();
}

export // x in radians, any range that fits an int32 of table steps
inline Sincos sincos1(float x) {
    using namespace sincos_detail;
    const float table_x = x * static_cast<float>(table_length / 2 / std::numbers::pi);
    int32_t i = table_x;
    const float remainder_x = table_x - i;
    const float dx = remainder_x * static_cast<float>(2 * std::numbers::pi / table_length);
    i &= table_length - 1;
    const float stab = table.sin[i];
    const float ctab = table.cos[i];
    return {stab + dx * ctab - .5f * dx * dx * stab - (1.f / 6) * dx * dx * dx * ctab,
            ctab - dx * stab - .5f * dx * dx * ctab + (1.f / 6) * dx * dx * dx * stab};
}
