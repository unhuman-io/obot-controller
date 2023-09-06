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
    float joint_encoder_bias;
    float joint_encoder_rollover;
    uint8_t pwm_multiplier;
    char git_hash[41];
} Param;

extern const Param * const param;
