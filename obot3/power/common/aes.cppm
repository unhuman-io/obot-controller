module;
#include <cstddef>
#include <cstdint>

export module power.aes;

export {
// AES-128 (encryption only) and the message authentication code made with it
// (CMAC, RFC 4493): what tells the controller that a "run" came from the
// remote it was paired with, and not from anything else that can transmit.
// 20 small messages a second: speed does not matter, so this is the plain
// algorithm, with its substitution table worked out by the compiler.
consteval auto aes_substitution() {
    struct Table {
        uint8_t box[256];
    } table{};
    // 3 generates the multiplicative group of the field: p runs through every
    // non-zero element, q is its inverse
    unsigned p = 1, q = 1;
    do {
        p = (p ^ (p << 1) ^ ((p & 0x80) ? 0x1B : 0)) & 0xFF;
        q ^= q << 1;
        q ^= q << 2;
        q ^= q << 4;
        q &= 0xFF;
        if (q & 0x80) q ^= 0x09;
        const auto turned = [q](int n) { return ((q << n) | (q >> (8 - n))) & 0xFF; };
        table.box[p] = static_cast<uint8_t>(q ^ turned(1) ^ turned(2) ^ turned(3) ^ turned(4) ^ 0x63);
    } while (p != 1);
    table.box[0] = 0x63;
    return table;
}
inline constexpr auto aes_table = aes_substitution();

constexpr uint8_t aes_double(uint8_t x) {
    return static_cast<uint8_t>((x << 1) ^ ((x & 0x80) ? 0x1B : 0));
}

struct Aes128 {
    uint8_t round_key[11][16]{};

    constexpr Aes128() = default;
    constexpr explicit Aes128(const uint8_t* key) { set_key(key); }

    constexpr void set_key(const uint8_t* key) {
        for (int k = 0; k < 16; k++) round_key[0][k] = key[k];
        uint8_t constant = 1;
        for (int round = 1; round <= 10; round++) {
            const uint8_t* last = round_key[round - 1];
            uint8_t* next = round_key[round];
            next[0] = static_cast<uint8_t>(last[0] ^ aes_table.box[last[13]] ^ constant);
            next[1] = static_cast<uint8_t>(last[1] ^ aes_table.box[last[14]]);
            next[2] = static_cast<uint8_t>(last[2] ^ aes_table.box[last[15]]);
            next[3] = static_cast<uint8_t>(last[3] ^ aes_table.box[last[12]]);
            for (int k = 4; k < 16; k++) next[k] = static_cast<uint8_t>(last[k] ^ next[k - 4]);
            constant = aes_double(constant);
        }
    }

    constexpr void encrypt(const uint8_t* in, uint8_t* out) const {
        uint8_t s[16];
        for (int k = 0; k < 16; k++) s[k] = static_cast<uint8_t>(in[k] ^ round_key[0][k]);
        for (int round = 1; round <= 10; round++) {
            uint8_t t[16];
            // substitute, and shift row r (bytes r, r + 4, ...) left by r
            for (int column = 0; column < 4; column++) {
                for (int row = 0; row < 4; row++) t[4 * column + row] = aes_table.box[s[4 * ((column + row) % 4) + row]];
            }
            if (round < 10) {       // mix each column
                for (int column = 0; column < 4; column++) {
                    uint8_t* c = t + 4 * column;
                    const uint8_t all = static_cast<uint8_t>(c[0] ^ c[1] ^ c[2] ^ c[3]), first = c[0];
                    c[0] = static_cast<uint8_t>(c[0] ^ all ^ aes_double(static_cast<uint8_t>(c[0] ^ c[1])));
                    c[1] = static_cast<uint8_t>(c[1] ^ all ^ aes_double(static_cast<uint8_t>(c[1] ^ c[2])));
                    c[2] = static_cast<uint8_t>(c[2] ^ all ^ aes_double(static_cast<uint8_t>(c[2] ^ c[3])));
                    c[3] = static_cast<uint8_t>(c[3] ^ all ^ aes_double(static_cast<uint8_t>(c[3] ^ first)));
                }
            }
            for (int k = 0; k < 16; k++) s[k] = static_cast<uint8_t>(t[k] ^ round_key[round][k]);
        }
        for (int k = 0; k < 16; k++) out[k] = s[k];
    }
};

// CMAC of a message: 16 bytes, of which a frame carries the first few
constexpr void aes_cmac(const Aes128& aes, const uint8_t* message, std::size_t length, uint8_t* mac) {
    const auto doubled = [](const uint8_t* in, uint8_t* out) {     // times x in the field of 128 bits
        const uint8_t carry = in[0] & 0x80;
        for (int k = 0; k < 15; k++) out[k] = static_cast<uint8_t>((in[k] << 1) | (in[k + 1] >> 7));
        out[15] = static_cast<uint8_t>((in[15] << 1) ^ (carry ? 0x87 : 0));
    };
    uint8_t zero[16]{}, l[16], k1[16], k2[16];
    aes.encrypt(zero, l);
    doubled(l, k1);
    doubled(k1, k2);

    uint8_t x[16]{};
    const std::size_t blocks = length == 0 ? 1 : (length + 15) / 16;
    const bool whole = length != 0 && length % 16 == 0;
    for (std::size_t block = 0; block < blocks; block++) {
        uint8_t y[16];
        const bool last = block + 1 == blocks;
        for (int k = 0; k < 16; k++) {
            const std::size_t at = 16 * block + static_cast<std::size_t>(k);
            uint8_t byte = at < length ? message[at] : (at == length ? 0x80 : 0);     // padded: 1 then 0s
            if (last) byte ^= whole ? k1[k] : k2[k];
            y[k] = static_cast<uint8_t>(x[k] ^ byte);
        }
        aes.encrypt(y, x);
    }
    for (int k = 0; k < 16; k++) mac[k] = x[k];
}
}
