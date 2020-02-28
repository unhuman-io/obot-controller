#ifndef MAIN_LOOP_H
#define MAIN_LOOP_H

#include "messages.h"
class LED;
class PIDDeadbandController;
class Communication;
class Encoder;
#include <cmath>
#include "control_fun.h"

class MainLoop {
 public:
    MainLoop(PIDDeadbandController &controller, Communication &communication, LED &led, Encoder &output_encoder) : 
        controller_(controller), communication_(communication), led_(led), output_encoder_(output_encoder) {}
    void init();
    void update();
    void set_param(MainLoopParam &);
    void get_status(MainLoopStatus * const main_loop_status) const;
    void set_mode(MainControlMode mode);
    LED* led() { return &led_; }
 private:
    MainLoopParam param_;
    PIDDeadbandController &controller_;
    Communication &communication_;
    LED &led_;
    ReceiveData receive_data_ = {};
    ReceiveData last_receive_data_ = {};
    uint64_t count_ = 0;
    FastLoopStatus fast_loop_status_ = {};
    MainControlMode mode_ = OPEN;
    Encoder &output_encoder_;
    float dt_ = 0;
    KahanSum phi_;
    uint32_t timestamp_ = 0;
    uint32_t last_timestamp_ = 0;
    friend class Actuator;


inline uint16_t minu16(uint16_t a, uint16_t b) {
  if (a > b) {
    return b;
  } else {
    return a;
  }
}
};

#endif
