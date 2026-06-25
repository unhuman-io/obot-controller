#pragma once

#include "../../../motorlib/boards/param_obot_g474_trace.h"

constexpr MainLoopParam2 param2 {
    .position_controller {
        .position {
            .kd = 2,
        },
    },
};
