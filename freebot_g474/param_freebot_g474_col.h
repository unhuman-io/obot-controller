#pragma once
#include "../motorlib/messages.h"

typedef struct {
    FastLoopParam fast_loop_param;
    MainLoopParam main_loop_param;
    StartupParam startup_param;
    char name[64];    // name available through usb string
} Param;

extern const Param param;
