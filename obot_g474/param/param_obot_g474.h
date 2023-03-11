#pragma once
#include "../../motorlib/messages.h"
#define XSTR(x) STR(x)
#define STR(x) #x

typedef struct {
    char name[64];    // name available through usb string
    FastLoopParam fast_loop_param;
    MainLoopParam main_loop_param;
    StartupParam startup_param;
    uint16_t drv_regs[5];
} Param;

extern const Param * const param;
