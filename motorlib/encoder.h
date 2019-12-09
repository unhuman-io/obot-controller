#ifndef ENCODER_H
#define ENCODER_H

#include <cstdint>

class Encoder {
 public:
    Encoder() { }
    virtual int32_t read() { return 0; }
    virtual int32_t get_value() const { return 0; }
    virtual void trigger() {}
    virtual int32_t get_index_pos() { return 0; }
    virtual bool index_received() { return false; }
 private:
};

#endif
