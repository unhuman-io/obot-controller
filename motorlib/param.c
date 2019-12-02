
#include "param.h"

extern const volatile Param initial_param;
static Param working_param;

Param *param() {
    return &working_param;
}

void init_param_from_flash() {
    working_param = initial_param;
}

void save_param_to_flash() {
    // TODO
}

