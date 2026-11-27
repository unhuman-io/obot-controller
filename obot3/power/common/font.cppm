module;
#include <cstdint>
#include <string_view>

export module power.font;
import power.frame;

export {
// Letters for the status screens: capitals, digits and a few signs, 5 x 7
// pixels, drawn here as they look and made into bits by the compiler. Small
// letters are written as capitals. Text is drawn at a whole multiple of that
// size (scale 2: 10 x 14 and so on), one column of space between letters.
struct Glyph {
    char character;
    uint8_t row[7];     // bit 4 = the leftmost of the 5 columns
};

consteval Glyph glyph(char character, std::string_view picture) {
    Glyph g{character, {}};
    for (int y = 0; y < 7; y++) {
        for (int x = 0; x < 5; x++) {
            if (picture[y * 5 + x] == '#') g.row[y] |= static_cast<uint8_t>(0x10 >> x);
        }
    }
    return g;
}

inline constexpr Glyph font[] = {
    glyph('0', ".###." "#...#" "#..##" "#.#.#" "##..#" "#...#" ".###."),
    glyph('1', "..#.." ".##.." "..#.." "..#.." "..#.." "..#.." ".###."),
    glyph('2', ".###." "#...#" "....#" "...#." "..#.." ".#..." "#####"),
    glyph('3', "#####" "...#." "..#.." "...#." "....#" "#...#" ".###."),
    glyph('4', "...#." "..##." ".#.#." "#..#." "#####" "...#." "...#."),
    glyph('5', "#####" "#...." "####." "....#" "....#" "#...#" ".###."),
    glyph('6', "..##." ".#..." "#...." "####." "#...#" "#...#" ".###."),
    glyph('7', "#####" "....#" "...#." "..#.." ".#..." ".#..." ".#..."),
    glyph('8', ".###." "#...#" "#...#" ".###." "#...#" "#...#" ".###."),
    glyph('9', ".###." "#...#" "#...#" ".####" "....#" "...#." ".##.."),
    glyph('A', ".###." "#...#" "#...#" "#####" "#...#" "#...#" "#...#"),
    glyph('B', "####." "#...#" "#...#" "####." "#...#" "#...#" "####."),
    glyph('C', ".###." "#...#" "#...." "#...." "#...." "#...#" ".###."),
    glyph('D', "###.." "#..#." "#...#" "#...#" "#...#" "#..#." "###.."),
    glyph('E', "#####" "#...." "#...." "####." "#...." "#...." "#####"),
    glyph('F', "#####" "#...." "#...." "####." "#...." "#...." "#...."),
    glyph('G', ".###." "#...#" "#...." "#.###" "#...#" "#...#" ".####"),
    glyph('H', "#...#" "#...#" "#...#" "#####" "#...#" "#...#" "#...#"),
    glyph('I', ".###." "..#.." "..#.." "..#.." "..#.." "..#.." ".###."),
    glyph('J', "..###" "...#." "...#." "...#." "...#." "#..#." ".##.."),
    glyph('K', "#...#" "#..#." "#.#.." "##..." "#.#.." "#..#." "#...#"),
    glyph('L', "#...." "#...." "#...." "#...." "#...." "#...." "#####"),
    glyph('M', "#...#" "##.##" "#.#.#" "#.#.#" "#...#" "#...#" "#...#"),
    glyph('N', "#...#" "#...#" "##..#" "#.#.#" "#..##" "#...#" "#...#"),
    glyph('O', ".###." "#...#" "#...#" "#...#" "#...#" "#...#" ".###."),
    glyph('P', "####." "#...#" "#...#" "####." "#...." "#...." "#...."),
    glyph('Q', ".###." "#...#" "#...#" "#...#" "#.#.#" "#..#." ".##.#"),
    glyph('R', "####." "#...#" "#...#" "####." "#.#.." "#..#." "#...#"),
    glyph('S', ".####" "#...." "#...." ".###." "....#" "....#" "####."),
    glyph('T', "#####" "..#.." "..#.." "..#.." "..#.." "..#.." "..#.."),
    glyph('U', "#...#" "#...#" "#...#" "#...#" "#...#" "#...#" ".###."),
    glyph('V', "#...#" "#...#" "#...#" "#...#" "#...#" ".#.#." "..#.."),
    glyph('W', "#...#" "#...#" "#...#" "#.#.#" "#.#.#" "#.#.#" ".#.#."),
    glyph('X', "#...#" "#...#" ".#.#." "..#.." ".#.#." "#...#" "#...#"),
    glyph('Y', "#...#" "#...#" "#...#" ".#.#." "..#.." "..#.." "..#.."),
    glyph('Z', "#####" "....#" "...#." "..#.." ".#..." "#...." "#####"),
    glyph('.', "....." "....." "....." "....." "....." ".##.." ".##.."),
    glyph(':', "....." ".##.." ".##.." "....." ".##.." ".##.." "....."),
    glyph('-', "....." "....." "....." "#####" "....." "....." "....."),
    glyph('+', "....." "..#.." "..#.." "#####" "..#.." "..#.." "....."),
    glyph('/', "....#" "....#" "...#." "..#.." ".#..." "#...." "#...."),
    glyph('%', "##..#" "##..#" "...#." "..#.." ".#..." "#..##" "#..##"),
    glyph('!', "..#.." "..#.." "..#.." "..#.." "..#.." "....." "..#.."),
    glyph('?', ".###." "#...#" "....#" "...#." "..#.." "....." "..#.."),
    glyph('>', "#...." ".#..." "..#.." "...#." "..#.." ".#..." "#...."),
    glyph('<', "....#" "...#." "..#.." ".#..." "..#.." "...#." "....#"),
    glyph('=', "....." "....." "#####" "....." "#####" "....." "....."),
};

// the glyph for a character; small letters as capitals; nullptr for a space or what the font does not have
constexpr const Glyph* find_glyph(char character) {
    if (character >= 'a' && character <= 'z') character = static_cast<char>(character - 'a' + 'A');
    for (const Glyph& g : font) {
        if (g.character == character) return &g;
    }
    return nullptr;
}

// how wide a text is at a scale, in pixels (no space after the last letter)
constexpr int text_width(std::string_view text, int scale = 1) {
    return text.empty() ? 0 : static_cast<int>(text.size()) * 6 * scale - scale;
}

// Text with its top left corner at (x, y). Returns the x after it, for more.
constexpr int draw_text(Frame& frame, int x, int y, std::string_view text, int scale = 1, bool black = true) {
    for (const char character : text) {
        if (const Glyph* g = find_glyph(character)) {
            for (int row = 0; row < 7; row++) {
                for (int column = 0; column < 5; column++) {
                    if (g->row[row] & (0x10 >> column)) frame.fill(x + column * scale, y + row * scale, scale, scale, black);
                }
            }
        }
        x += 6 * scale;
    }
    return x;
}

// the same, centred on x
constexpr void draw_text_centred(Frame& frame, int x, int y, std::string_view text, int scale = 1, bool black = true) {
    draw_text(frame, x - text_width(text, scale) / 2, y, text, scale, black);
}

// A number into text, for the screens: `decimals` digits after the point of a
// value given in thousandths ("54.2" from 54230 with 1). Returns its length;
// `out` holds at least 12 characters.
constexpr int format_milli(int32_t milli, int decimals, char* out) {
    int n = 0;
    uint32_t value = milli < 0 ? static_cast<uint32_t>(-static_cast<int64_t>(milli)) : static_cast<uint32_t>(milli);
    uint32_t unit = 1000;
    for (int k = 0; k < decimals; k++) unit /= 10;
    value = (value + unit / 2) / unit;          // rounded to the last digit shown
    if (milli < 0 && value != 0) out[n++] = '-';
    char digits[11];
    int count = 0;
    do {
        digits[count++] = static_cast<char>('0' + value % 10);
        value /= 10;
    } while (value != 0 || count <= decimals);
    while (count > 0) {
        if (count == decimals) out[n++] = '.';
        out[n++] = digits[--count];
    }
    return n;
}
}
