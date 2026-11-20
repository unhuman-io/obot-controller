#pragma once

// Placeholder with the interface of a fast loop controller, see control/foc.h
// for the real one
struct BasicFOCController {
    struct Command {
        float iq, id, vq, vd;
    };
    struct Output {};
    template<typename Measured>
    Output update(const Command &command, const Measured &measured) { kp++; return {}; }
    int kp = 2;
};
