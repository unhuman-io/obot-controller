#ifndef ACTUATOR_H
#define ACTUATOR_H

//#include "fast_loop.h"
template<class Encoder, class PWM> class FastLoop;
class MainLoop;

template<class MotorEncoder, class PWM>
class Actuator {
 public:
    Actuator(FastLoop<MotorEncoder, PWM> &fast_loop, MainLoop &main_loop) : fast_loop_(fast_loop), main_loop_(main_loop) {}
    void run();
 private:
    FastLoop<MotorEncoder, PWM> &fast_loop_;
    MainLoop &main_loop_;
};

#include "actuator.cpp"

#endif
