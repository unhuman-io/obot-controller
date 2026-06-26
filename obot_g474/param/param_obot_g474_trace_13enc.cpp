#include "param_obot_g474_can.h"
#include "math.h"


// Can be written by external methods, e.g. bootloader
const volatile Param __attribute__ ((section ("flash_param"))) param_store = {
#include "param_default.h"
.name = "13enc",
.can_id = 1,
};
