#include "otp.h"

const volatile OTP __attribute__ ((section ("otp"), used)) otp1 = {
    .version = 1,
    .name = "motor_molex",
    .rev = "MR1"
};

