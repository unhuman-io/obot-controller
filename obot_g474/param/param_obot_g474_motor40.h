#pragma once
#include "../../motorlib/messages.h"

typedef struct {
    char name[64];    // name available through usb string
    FastLoopParam fast_loop_param;
    MainLoopParam main_loop_param;
    StartupParam startup_param;
    char git_hash[41];
} Param;

extern const Param * const param;
