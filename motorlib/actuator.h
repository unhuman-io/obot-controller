#pragma once

#include "messages.h"

class FastLoop;
class MainLoop;
class Actuator {
 public:
    Actuator(FastLoop &fast_loop, MainLoop &main_loop) : fast_loop_(fast_loop), main_loop_(main_loop) {}
    void run();
    void set_param(FastLoopParam &, MainLoopParam &);
    
    FastLoop &fast_loop_;
    MainLoop &main_loop_;
};