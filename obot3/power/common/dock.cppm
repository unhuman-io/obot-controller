module;
#include <cstddef>
#include <cstdint>

export module power.dock;
import power.aes;
import power.messages;

export {
// Pairing, on the dock. The remote's dock has a data contact: one wire between
// the remote and the power board's radio module, a slow UART on which only one
// speaks at a time. Putting the remote on the robot is the pairing: there is
// nothing to press, and a remote that was never on this robot's dock, or was
// last on another's, cannot run it.
//
// Frames are those of the link between the board's processors (0xA5, kind,
// length, ..., CRC-16), of four kinds:
//
//   remote -> board   HELLO  its address (6) | proof (4) | 1 if its cell wants charging
//   board -> remote   KNOWN                                   you are the remote I have: nothing to do
//   board -> remote   KEY    the board's address (6) | a new key (16)
//   remote -> board   TAKEN  proof (4), made with the new key
//
// The remote says HELLO twice a second while it is on the dock. Its proof is
// the first bytes of a CMAC, with the key it has, over both addresses: the
// board sees from it whether the two still have the same key, without the key
// crossing the wire again. If not (a new remote, a replaced board, one of the
// two erased), the board makes a new key and sends it; the remote stores it
// and answers TAKEN, and only then does the board store it too. Whatever is
// lost on the way, the next HELLO starts again.
//
// The key crosses a wire between two spring contacts on the back of the
// robot. Whoever can put a probe there can also press the robot's buttons.
inline constexpr uint8_t dock_kind_hello = 3, dock_kind_known = 4, dock_kind_key = 5, dock_kind_taken = 6;
inline constexpr std::size_t dock_hello_size = 11, dock_key_size = 22, dock_taken_size = 4;

struct Pairing {
    bool valid = false;
    uint8_t peer[6]{};          // the other one's radio address
    uint8_t key[16]{};
};
inline constexpr std::size_t pairing_record_size = 24;      // as stored: 1, 0, the address, the key

constexpr void pack(const Pairing& p, uint8_t* record) {
    record[0] = p.valid ? 1 : 0;
    record[1] = 0;
    for (int k = 0; k < 6; k++) record[2 + k] = p.peer[k];
    for (int k = 0; k < 16; k++) record[8 + k] = p.key[k];
}
constexpr Pairing unpack_pairing(const uint8_t* record) {
    Pairing p;
    p.valid = record[0] == 1;
    for (int k = 0; k < 6; k++) p.peer[k] = record[2 + k];
    for (int k = 0; k < 16; k++) p.key[k] = record[8 + k];
    return p;
}

constexpr void pairing_proof(const uint8_t* key, const uint8_t* remote, const uint8_t* board, uint8_t* proof) {
    uint8_t message[13] = {'P'};
    for (int k = 0; k < 6; k++) {
        message[1 + k] = remote[k];
        message[7 + k] = board[k];
    }
    uint8_t mac[16];
    aes_cmac(Aes128(key), message, sizeof message, mac);
    for (int k = 0; k < 4; k++) proof[k] = mac[k];
}

constexpr std::size_t dock_frame(uint8_t kind, const uint8_t* payload, std::size_t length, uint8_t* frame) {
    uint8_t* at = frame;
    put8(at, link_start);
    put8(at, kind);
    put8(at, 0);
    for (std::size_t k = 0; k < length; k++) put8(at, payload[k]);
    return finish_link_frame(frame, at);
}

// ---- the remote's side -----------------------------------------------------------------
struct DockRemote {
    // the HELLO to send; `own` is the remote's address
    constexpr std::size_t hello(const Pairing& have, const uint8_t* own, bool wants_charge, uint8_t* frame) const {
        uint8_t payload[dock_hello_size]{};
        for (int k = 0; k < 6; k++) payload[k] = own[k];
        if (have.valid) pairing_proof(have.key, own, have.peer, payload + 6);
        payload[10] = wants_charge ? 1 : 0;
        return dock_frame(dock_kind_hello, payload, sizeof payload, frame);
    }
    // A frame from the board. Returns the length of the answer in `frame` (0: none). `changed`:
    // `have` is a new pairing and has to be stored before the answer is sent.
    constexpr std::size_t receive(uint8_t kind, const uint8_t* payload, std::size_t length, const uint8_t* own, Pairing& have, bool& changed, uint8_t* frame) const {
        changed = false;
        if (kind != dock_kind_key || length != dock_key_size) return 0;
        have.valid = true;
        for (int k = 0; k < 6; k++) have.peer[k] = payload[k];
        for (int k = 0; k < 16; k++) have.key[k] = payload[6 + k];
        changed = true;
        uint8_t proof[4];
        pairing_proof(have.key, own, have.peer, proof);
        return dock_frame(dock_kind_taken, proof, sizeof proof, frame);
    }
};

// ---- the board's side ------------------------------------------------------------------------
struct DockBoard {
    Pairing offered{};          // sent to the remote, not stored yet

    // A frame from the dock. Returns the length of the answer in `frame` (0: none).
    //   `have`      what is stored; replaced when `changed` comes back true (to be stored then)
    //   `random`    16 fresh random bytes, used if a key has to be made
    //   `docked`    a remote said HELLO      `wants_charge`  ... and that its cell wants charging
    constexpr std::size_t receive(uint8_t kind, const uint8_t* payload, std::size_t length, const uint8_t* own, const uint8_t* random,
                                  Pairing& have, bool& changed, bool& docked, bool& wants_charge, uint8_t* frame) {
        changed = docked = wants_charge = false;
        if (kind == dock_kind_hello && length == dock_hello_size) {
            docked = true;
            wants_charge = payload[10] & 1;
            if (have.valid) {
                uint8_t proof[4];
                pairing_proof(have.key, payload, own, proof);
                if (same_bytes(payload, have.peer, 6) && same_bytes(proof, payload + 6, 4)) return dock_frame(dock_kind_known, nullptr, 0, frame);
            }
            offered.valid = true;
            uint8_t key_payload[dock_key_size];
            for (int k = 0; k < 6; k++) {
                offered.peer[k] = payload[k];
                key_payload[k] = own[k];
            }
            for (int k = 0; k < 16; k++) offered.key[k] = key_payload[6 + k] = random[k];
            return dock_frame(dock_kind_key, key_payload, sizeof key_payload, frame);
        }
        if (kind == dock_kind_taken && length == dock_taken_size && offered.valid) {
            uint8_t proof[4];
            pairing_proof(offered.key, offered.peer, own, proof);
            if (same_bytes(proof, payload, 4)) {
                have = offered;
                offered.valid = false;
                changed = true;
            }
        }
        return 0;
    }
};
}
