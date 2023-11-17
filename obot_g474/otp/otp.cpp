#include "otp.h"

#if !defined(NAME) || !defined(REV)
#error Must define NAME and REV to use this file
#endif

static_assert(NAME == "motor_molex" || NAME == "motor");

static_assert(NAME == "motor_molex" ? 
    REV == "MR0" || REV == "MR0P" || REV == "MR1" || REV == "MR2" : 
    REV == "R0" || REV == "R1" || REV == "R3" || REV == "R4");


const volatile OTP __attribute__ ((section ("otp"), used)) otp1 = {
    .version = 1,
    .name = NAME,
    .rev = REV
};

