#pragma once
#include <charconv>
#include <cstdint>
#include <cstring>

// The obot text api of an obot3 application, as far as it does not depend on
// the board: named variables, and the gdb remote protocol requests that
// motorlib's parameter_api takes ("$m...", see gdb_request below). A request
// is a string and so is its reply.

// A variable is a float or an integer, through functions: get, and set if it
// can be written. text_float() and text_int() make one from lambdas.
struct TextVariable {
    const char* name;
    float (*get_float)();
    void (*set_float)(float);
    int64_t (*get_int)();
    void (*set_int)(int64_t);
};
constexpr TextVariable text_float(const char* name, float (*get)(), void (*set)(float) = nullptr) {
    return {name, get, set, nullptr, nullptr};
}
constexpr TextVariable text_int(const char* name, int64_t (*get)(), void (*set)(int64_t) = nullptr) {
    return {name, nullptr, nullptr, get, set};
}

// A float as text with 7 significant digits, in the shortest of 0.00123,
// 1234.5 and 1.2345e-10. The library's conversions need a heap and
// exceptions. Returns the length; buffer has room for 16 characters.
constexpr uint32_t float_to_text(float value, char* buffer) {
    char* p = buffer;
    const auto put = [&p](const char* text) {
        while (*text != 0) *p++ = *text++;
    };
    if (value != value) {
        put("nan");
        return p - buffer;
    }
    if (value < 0) {
        *p++ = '-';
        value = -value;
    }
    if (value == 0) {
        *p++ = '0';
        return p - buffer;
    }
    if (value - value != 0) {
        put("inf");
        return p - buffer;
    }
    int exponent = 0;       // value = mantissa * 10^exponent, 1 <= mantissa < 10
    while (value >= 10) {
        value /= 10;
        exponent++;
    }
    while (value < 1) {
        value *= 10;
        exponent--;
    }
    uint32_t digits = static_cast<uint32_t>(value * 1e6f + 0.5f);     // 7 of them
    if (digits >= 10000000) {
        digits /= 10;
        exponent++;
    }
    char digit[7];
    for (int i = 6; i >= 0; i--) {
        digit[i] = '0' + digits % 10;
        digits /= 10;
    }
    int count = 7;          // without the zeros at the end
    while (count > 1 && digit[count - 1] == '0') count--;

    if (exponent >= -4 && exponent < 7) {
        if (exponent < 0) {
            put("0.");
            for (int i = -1; i > exponent; i--) *p++ = '0';
            for (int i = 0; i < count; i++) *p++ = digit[i];
        } else {
            for (int i = 0; i < count || i <= exponent; i++) {
                if (i == exponent + 1) *p++ = '.';
                *p++ = i < count ? digit[i] : '0';
            }
        }
    } else {
        *p++ = digit[0];
        if (count > 1) *p++ = '.';
        for (int i = 1; i < count; i++) *p++ = digit[i];
        *p++ = 'e';
        if (exponent < 0) {
            *p++ = '-';
            exponent = -exponent;
        }
        if (exponent >= 10) *p++ = '0' + exponent / 10;
        *p++ = '0' + exponent % 10;
    }
    return p - buffer;
}

// The other way: [-]digits[.digits][e[-]digits], and nothing behind it but
// spaces. False if the text is not that.
constexpr bool text_to_float(const char* p, float& value) {
    const bool negative = *p == '-';
    if (*p == '-' || *p == '+') p++;
    float mantissa = 0;
    int exponent = 0;
    bool any = false, fraction = false;
    for (;; p++) {
        if (*p >= '0' && *p <= '9') {
            mantissa = mantissa * 10 + (*p - '0');
            if (fraction) exponent--;
            any = true;
        } else if (*p == '.' && !fraction) {
            fraction = true;
        } else {
            break;
        }
    }
    if (!any) return false;
    if (*p == 'e' || *p == 'E') {
        p++;
        const bool exponent_negative = *p == '-';
        if (*p == '-' || *p == '+') p++;
        if (*p < '0' || *p > '9') return false;
        int number = 0;
        while (*p >= '0' && *p <= '9' && number < 1000) number = number * 10 + (*p++ - '0');
        exponent += exponent_negative ? -number : number;
    }
    while (*p == ' ') p++;
    if (*p != 0) return false;
    for (; exponent > 0 && mantissa - mantissa == 0; exponent--) mantissa *= 10;
    for (; exponent < 0 && mantissa != 0; exponent++) mantissa /= 10;
    value = negative ? -mantissa : mantissa;
    return true;
}
static_assert([] {
    constexpr auto same = [](float value, const char* expected) {
        char text[16] = {};
        const uint32_t length = float_to_text(value, text);
        for (uint32_t i = 0; i <= length; i++) {
            if (text[i] != expected[i]) return false;
        }
        float back = 0;
        return text_to_float(text, back) && back - value < 1e-6f * (value < 0 ? -value : value) + 1e-30f &&
            value - back < 1e-6f * (value < 0 ? -value : value) + 1e-30f;
    };
    float f = 0;
    return same(0, "0") && same(1, "1") && same(-2.5f, "-2.5") && same(0.1f, "0.1") && same(1234.5f, "1234.5") &&
        same(1e6f, "1000000") && same(1e7f, "1e7") && same(0.00123f, "0.00123") && same(1.5e-10f, "1.5e-10") &&
        same(40000, "40000") && !text_to_float("", f) && !text_to_float("abc", f) && !text_to_float("1.5x", f) &&
        text_to_float("  ", f) == false && text_to_float("3 ", f) && f == 3;
}(), "float text conversions");

// Where a reply is put together. What does not fit is dropped.
class TextOut {
 public:
    TextOut(char* buffer, uint32_t capacity) : begin_(buffer), p_(buffer), end_(buffer + capacity - 1) { *p_ = 0; }
    void put(char c) {
        if (p_ < end_) *p_++ = c;
        *p_ = 0;
    }
    void put(const char* string) {
        while (*string != 0) put(*string++);
    }
    void put(const char* string, uint32_t length) {
        for (uint32_t i = 0; i < length; i++) put(string[i]);
    }
    void put_number(float value) {
        char number[16];
        put(number, float_to_text(value, number));
    }
    void put_number(int64_t value) {
        char number[24];
        const auto result = std::to_chars(number, number + sizeof(number), value);
        put(number, result.ptr - number);
    }
    void put_hex(const uint8_t* data, uint32_t length) {
        constexpr char hex[] = "0123456789abcdef";
        for (uint32_t i = 0; i < length; i++) {
            put(hex[data[i] >> 4]);
            put(hex[data[i] & 0xF]);
        }
    }
    const char* text() const { return begin_; }
    uint32_t length() const { return p_ - begin_; }
    uint32_t room() const { return end_ - p_; }
    void clear() {
        p_ = begin_;
        *p_ = 0;
    }
 private:
    char* begin_;
    char* p_;
    char* end_;
};

// "name" gets and "name=value" sets one of the variables: the reply is the
// value, or "name set value", or "name error" for a value that is no number or
// a variable that can't be written. False if there is no such variable.
template<uint32_t N>
bool text_variable_request(const TextVariable (&variables)[N], const char* request, TextOut& out) {
    const char* equal = std::strchr(request, '=');
    const uint32_t name_length = equal != nullptr ? equal - request : std::strlen(request);
    for (const TextVariable& variable : variables) {
        if (std::strlen(variable.name) != name_length || std::strncmp(variable.name, request, name_length) != 0) continue;
        if (equal == nullptr) {
            if (variable.get_float != nullptr) out.put_number(variable.get_float());
            else out.put_number(variable.get_int());
            return true;
        }
        const char* value = equal + 1;
        const char* value_end = value + std::strlen(value);
        bool ok = false;
        if (variable.set_float != nullptr) {
            float number;
            ok = text_to_float(value, number);
            if (ok) variable.set_float(number);
        } else if (variable.set_int != nullptr) {
            int64_t number;
            const auto result = std::from_chars(value, value_end, number);
            ok = result.ec == std::errc{} && result.ptr == value_end;
            if (ok) variable.set_int(number);
        }
        out.put(variable.name);
        out.put(ok ? " set " : " error");
        if (ok) out.put(value);
        return true;
    }
    return false;
}

constexpr int hex_digit(char c) {
    return c >= '0' && c <= '9' ? c - '0' : c >= 'a' && c <= 'f' ? c - 'a' + 10 : c >= 'A' && c <= 'F' ? c - 'A' + 10 : -1;
}

// A hex number at p, which is moved behind it. False if there is none.
inline bool parse_hex(const char*& p, uint32_t& value) {
    const char* start = p;
    value = 0;
    while (hex_digit(*p) >= 0) value = value << 4 | hex_digit(*p++);
    return p != start;
}

// Hex digit pairs to bytes: the number of bytes, at most capacity
inline uint32_t parse_hex_bytes(const char* p, uint8_t* data, uint32_t capacity) {
    uint32_t length = 0;
    while (length < capacity && hex_digit(p[0]) >= 0 && hex_digit(p[1]) >= 0) {
        data[length++] = hex_digit(p[0]) << 4 | hex_digit(p[1]);
        p += 2;
    }
    return length;
}

// The gdb remote protocol packets that come as text requests with a leading
// "$" and without the checksum, from `motor_util gdbserver` between gdb and
// the node: memory, which with the elf file is every static variable by name,
// and monitor commands. An application with the debug monitor takes the run
// control packets itself (debug_monitor.h) before it calls this; without it
// gdb sees a target that is stopped, while the application runs.
//   m address,length        memory read: hex bytes, fewer than asked for if the reply is full or the
//                           memory ends, E14 if there is none
//   M address,length:hex    memory write: OK, or E14
//   g                       the registers: all 0, there is no stopped context
//   c, b, y                 continue, break and the state: OK, OK, "continued"
//   qRcmd,hex               a monitor command: the text request in hex, its reply in hex
// Anything else gets the empty reply, which is gdb's "not supported".
// Memory::read / write(address, data, length) return the bytes they got to.
// monitor(request, out) is the text api itself.
template<typename Memory, typename Monitor>
void gdb_request(const char* request, TextOut& out, Monitor monitor) {
    const char* p = request + 1;    // behind the $
    uint8_t data[256];
    uint32_t address, length;
    switch (*p++) {
        case 'm': {
            if (!parse_hex(p, address) || *p++ != ',' || !parse_hex(p, length)) break;
            if (length > sizeof(data)) length = sizeof(data);
            if (2 * length > out.room()) length = out.room() / 2;
            if (length > 31) length &= ~1u;     // a reply of several frames is a multiple of 4 characters
            const uint32_t done = Memory::read(address, data, length);
            if (done == 0 && length != 0) out.put("E14");
            else out.put_hex(data, done);
            break;
        }
        case 'M': {
            if (!parse_hex(p, address) || *p++ != ',' || !parse_hex(p, length) || *p++ != ':') break;
            if (length > sizeof(data) || parse_hex_bytes(p, data, sizeof(data)) != length) {
                out.put("E01");
                break;
            }
            out.put(Memory::write(address, data, length) == length ? "OK" : "E14");
            break;
        }
        case 'g':
            for (int i = 0; i < 17 * 8; i++) out.put('0');
            break;
        case 'c':
        case 'b':
            out.put("OK");
            break;
        case 'y':
            out.put("continued");
            break;
        case 'q': {
            if (std::strncmp(p, "Rcmd,", 5) != 0) break;
            char command[64];
            const uint32_t command_length = parse_hex_bytes(p + 5, reinterpret_cast<uint8_t*>(command), sizeof(command) - 1);
            command[command_length] = 0;
            char text[120];
            TextOut text_out(text, sizeof(text));
            monitor(command, text_out);
            text_out.put('\n');
            out.put_hex(reinterpret_cast<const uint8_t*>(text), text_out.length());
            break;
        }
    }
}
