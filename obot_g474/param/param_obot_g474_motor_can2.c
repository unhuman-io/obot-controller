#include "param_obot_g474_can.h"
#include "math.h"


// Can be written by external methods, e.g. bootloader
const Param __attribute__ ((section ("flash_param"))) param_store = {
#include "param_default.h"
.name = "can_test2",
.can_id = 0x02,
};
