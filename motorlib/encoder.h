#ifndef ENCODER_H
#define ENCODER_H

#include <cstdint>

class Encoder {
 public:
    Encoder() { }
    int32_t get_value() { return 0; }
    void trigger() {}
    int32_t get_index_pos() { return 0; }
    bool index_received() { return false; }
 private:
};

#endif
