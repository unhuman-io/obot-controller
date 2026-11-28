module;
#include <cstddef>
#include <cstdint>

export module power.frame;

export {
// The picture on a Sharp memory LCD LS013B7DH03 (128 x 128, black and white,
// reflective), and the bytes that put it there.
//
// The display keeps its picture by itself; it is sent lines, in any number,
// and only the lines that changed need sending. The serial format, with the
// SPI sending the least significant bit first and SCS (active HIGH) held high
// for the whole transfer:
//
//     mode | line number, 16 bytes of pixels, 0 | ... more lines ... | 0
//
// mode: bit 0 = this writes lines, bit 1 = VCOM, bit 2 = clear everything.
// Line numbers count from 1 at the top. Of a line's pixels the leftmost goes
// first, so it is bit 0 of the first byte here; a 1 is white (the mirror), a 0
// black.
//
// VCOM is the polarity the liquid crystal is driven with. With the display's
// EXTMODE pin low, as on our boards, it follows this bit, and the bit has to
// change at least once a second or the display is damaged over time (a DC
// voltage across the crystal). lcd_hold() is the transfer for that when there
// is nothing to draw: it changes nothing on the glass.
inline constexpr int lcd_width = 128, lcd_height = 128;
inline constexpr int lcd_line_bytes = lcd_width / 8;
inline constexpr uint8_t lcd_mode_write = 0x01, lcd_mode_vcom = 0x02, lcd_mode_clear = 0x04;
// most bytes of a transfer: every line at once
inline constexpr std::size_t lcd_transfer_max = 1 + lcd_height * (1 + lcd_line_bytes + 1) + 1;

struct Frame {
    uint8_t line[lcd_height][lcd_line_bytes];

    constexpr void clear(bool black = false) {
        for (auto& row : line) {
            for (uint8_t& byte : row) byte = black ? 0x00 : 0xFF;
        }
    }
    constexpr bool inside(int x, int y) const { return x >= 0 && x < lcd_width && y >= 0 && y < lcd_height; }
    constexpr void set(int x, int y, bool black = true) {
        if (!inside(x, y)) return;
        const uint8_t bit = static_cast<uint8_t>(1u << (x & 7));
        if (black) {
            line[y][x >> 3] &= static_cast<uint8_t>(~bit);
        } else {
            line[y][x >> 3] |= bit;
        }
    }
    constexpr bool black(int x, int y) const { return inside(x, y) && !(line[y][x >> 3] & (1u << (x & 7))); }
    // a filled rectangle, x0 <= x < x0 + width
    constexpr void fill(int x0, int y0, int width, int height, bool black = true) {
        for (int y = y0; y < y0 + height; y++) {
            for (int x = x0; x < x0 + width; x++) set(x, y, black);
        }
    }
    // black to white and white to black in a rectangle
    constexpr void invert(int x0, int y0, int width, int height) {
        for (int y = y0; y < y0 + height; y++) {
            for (int x = x0; x < x0 + width; x++) set(x, y, !black(x, y));
        }
    }
    // the outline of a rectangle, one pixel wide
    constexpr void box(int x0, int y0, int width, int height) {
        fill(x0, y0, width, 1);
        fill(x0, y0 + height - 1, width, 1);
        fill(x0, y0, 1, height);
        fill(x0 + width - 1, y0, 1, height);
    }
};

// The transfer that writes the lines first..last (0 is the top one) of a
// frame. Returns its length; `out` must hold lcd_transfer_max bytes.
constexpr std::size_t lcd_lines(const Frame& frame, int first, int last, bool vcom, uint8_t* out) {
    std::size_t n = 0;
    out[n++] = static_cast<uint8_t>(lcd_mode_write | (vcom ? lcd_mode_vcom : 0));
    for (int y = first; y <= last; y++) {
        out[n++] = static_cast<uint8_t>(y + 1);
        for (int k = 0; k < lcd_line_bytes; k++) out[n++] = frame.line[y][k];
        out[n++] = 0;
    }
    out[n++] = 0;
    return n;
}

// The transfer that only sets VCOM: two bytes
constexpr std::size_t lcd_hold(bool vcom, uint8_t* out) {
    out[0] = vcom ? lcd_mode_vcom : 0;
    out[1] = 0;
    return 2;
}

// The transfer that makes the whole display white: two bytes
constexpr std::size_t lcd_clear(bool vcom, uint8_t* out) {
    out[0] = static_cast<uint8_t>(lcd_mode_clear | (vcom ? lcd_mode_vcom : 0));
    out[1] = 0;
    return 2;
}

// What is on the glass, and so which lines of a new picture have to be sent:
// next() gives one run of changed lines at a time, each a transfer, until
// there are none. A display takes a millisecond for 6 lines at 1 MHz, and
// the processor is awake for that long: a status screen that changes one
// number sends a dozen lines, not 128.
struct LcdShadow {
    Frame shown;
    bool known = false;     // false: the glass shows something else (power on): everything is sent

    struct Run {
        int first = 0, last = -1;
        constexpr bool empty() const { return last < first; }
    };
    // the next run of lines in which `frame` differs from what is shown, from line `from` on
    constexpr Run next(const Frame& frame, int from = 0) const {
        Run run;
        int y = from;
        while (y < lcd_height && same(frame, y)) y++;
        if (y == lcd_height) return run;
        run.first = y;
        // lines that are the same but lie between changed ones cost 18 bytes sent, a new transfer costs
        // about as much in the time around it: a gap of up to two lines is sent along
        int gap = 0;
        for (run.last = y; y < lcd_height; y++) {
            if (!same(frame, y)) {
                run.last = y;
                gap = 0;
            } else if (++gap > 2) {
                break;
            }
        }
        return run;
    }
    constexpr void sent(const Frame& frame, Run run) {
        for (int y = run.first; y <= run.last; y++) {
            for (int k = 0; k < lcd_line_bytes; k++) shown.line[y][k] = frame.line[y][k];
        }
    }
    constexpr bool same(const Frame& frame, int y) const {
        if (!known) return false;
        for (int k = 0; k < lcd_line_bytes; k++) {
            if (shown.line[y][k] != frame.line[y][k]) return false;
        }
        return true;
    }
};
}
