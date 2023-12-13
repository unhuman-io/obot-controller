#pragma once
#include "../../motorlib/messages.h"
#define XSTR(x) STR(x)
#define STR(x) #x

typedef struct {
    char name[64];    // name available through usb string
    FastLoopParam fast_loop_param;
    MainLoopParam main_loop_param;
    StartupParam startup_param;
    char obot_hash[41];
} Param;

extern const Param * const param;