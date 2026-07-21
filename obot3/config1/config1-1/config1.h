#include "../config1_default.h"

struct SpecialEncoder {
    void read() {}
};

struct Config1 : DefaultConfig1 {
    using MotorEncoderType = SpecialEncoder;
    int special_parameter = 11;
};


