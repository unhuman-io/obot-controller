#include "otp.h"

#if !defined(NAME) || !defined(REV)
#error Must define NAME and REV to use this file
#endif

#if NAME == "motor_molex" || NAME == "motor"
#else
#error Use a a supported NAME
#endif

#if NAME == "motor_molex"
#if REV == "MR0" || REV == "MR0P" || REV == "MR1" || REV == "MR2"
#else
#error Use a supported REV 
#endif
#endif



const volatile OTP __attribute__ ((section ("otp"), used)) otp1 = {
    .version = 1,
    .name = NAME,
    .rev = REV
};

