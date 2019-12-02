
#ifndef MAIN_LOOP_H
#define MAIN_LOOP_H

#include "messages.h"
class LED;
class PIDController;
class Communication;
#include <cmath>

class MainLoop {
 public:
    MainLoop(PIDController &controller, Communication &communication, LED &led) : 
        controller_(controller), communication_(communication), led_(led) {}
    void init();
    void update();
    void set_param(MainLoopParam &);
    void get_status(MainLoopStatus * const main_loop_status) const;
 private:
    MainLoopParam param_;
    PIDController &controller_;
    Communication &communication_;
    LED &led_;
    ReceiveData receive_data_ = {};
    uint64_t count_ = 0;
    FastLoopStatus fast_loop_status_ = {};
    MainControlMode mode_ = OPEN;


inline uint16_t minu16(uint16_t a, uint16_t b) {
  if (a > b) {
    return b;
  } else {
    return a;
  }
}
};

#endif
