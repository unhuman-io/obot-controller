#pragma once

#include "messages.h"

class PIDDeadbandController;
class Communication;
class Encoder;

#include <cmath>
#include "control_fun.h"
#include "sincos.h"
#include "led.h"
#include "util.h"

template<typename FastLoop>
class MainLoop {
 public:
    MainLoop(FastLoop &fast_loop, PIDDeadbandController &controller, Communication &communication, LED &led, Encoder &output_encoder) : 
        fast_loop_(fast_loop), controller_(controller), communication_(communication), led_(led), output_encoder_(output_encoder) {}
    void init() {}
    void update() {
      count_++;
      output_encoder_.trigger();
      
      last_timestamp_ = timestamp_;
      timestamp_ = get_clock();
      dt_ = (timestamp_ - last_timestamp_) * (1.0f/CPU_FREQUENCY_HZ);

      fast_loop_.get_status(&fast_loop_status_);

      int count_received = communication_.receive_data(&receive_data_);
      if (count_received) {
        if (mode_ != static_cast<MainControlMode>(receive_data_.mode_desired)) {
          set_mode(static_cast<MainControlMode>(receive_data_.mode_desired));
          controller_.init(fast_loop_status_.motor_position.position);
        }
      }

      float iq_des = 0;
      switch (mode_) {
        case CURRENT:
          iq_des = receive_data_.current_desired;
          break;
        case POSITION:
          iq_des = controller_.step(receive_data_.position_desired, receive_data_.velocity_desired, receive_data_.reserved, fast_loop_status_.motor_position.position) + \
                  receive_data_.current_desired;
          break;
        case VELOCITY:
          // saturate position so that current = current max due to kp, so error max = 
          iq_des = controller_.step(fast_loop_status_.motor_position.position + param_.controller_param.command_max/param_.controller_param.kp*fsignf(receive_data_.velocity_desired), 
                  receive_data_.velocity_desired, receive_data_.reserved, fast_loop_status_.motor_position.position, receive_data_.velocity_desired) + \
                  receive_data_.current_desired;
          break;
        case POSITION_TUNING: 
        {
          // phi_ is a radian counter at the command frequency doesn't get larger than 2*pi
          float frequency_hz = receive_data_.reserved;  // negative frequencies are square waves, positive sine waves
          float amplitude = receive_data_.position_desired;

          // KahanSum allows for and summing of dt_ allows for low frequencies without losing resolution
          phi_.add(2 * (float) M_PI * fabsf(frequency_hz) * dt_);
          if (phi_.value() > 2 * (float) M_PI) {
            phi_.add(-2 * (float) M_PI);
          }
          Sincos sincos;
          sincos = sincos1(phi_.value());
          float position_desired, velocity_desired;
          if (frequency_hz > 0) { // sin wave
            position_desired = amplitude * sincos.sin;
            velocity_desired = 2 * (float) M_PI * frequency_hz * amplitude * sincos.cos;
          } else { // square wave
            position_desired = amplitude * fsignf(sincos.sin);
            velocity_desired = 0;
          }
          iq_des = controller_.step(position_desired, velocity_desired, 0, fast_loop_status_.motor_position.position);
          break;
        }
        case CURRENT_TUNING: 
          fast_loop_.set_tuning_amplitude(receive_data_.current_desired);
          fast_loop_.set_tuning_frequency(receive_data_.reserved);
        default:
          break;
      }

      fast_loop_.set_iq_des(iq_des);
      SendData send_data;
      send_data.iq = fast_loop_status_.foc_status.measured.i_q;
      send_data.host_timestamp_received = receive_data_.host_timestamp;
      send_data.mcu_timestamp = fast_loop_status_.timestamp;
      send_data.motor_encoder = fast_loop_status_.motor_mechanical_position;
      send_data.motor_position = fast_loop_status_.motor_position.position;
      send_data.joint_position = output_encoder_.get_value();//*2.0*(float) M_PI/param_.output_encoder.cpr;
      send_data.reserved[0] = iq_des; //fast_loop_status_.foc_status.measured.i_0;
      communication_.send_data(send_data);
      led_.update();
      last_receive_data_ = receive_data_;
    }
    void set_param(MainLoopParam &param) {
      controller_.set_param(param.controller_param);
      param_ = param;
    }
    void get_status(MainLoopStatus * const main_loop_status) const {}
    void set_mode(MainControlMode mode) {
        mode_ = mode;
      switch (mode) {
        case OPEN:
        default:
          fast_loop_.open_mode();
          led_.set_color(LED::AZURE);
          break;
        case DAMPED:
          fast_loop_.brake_mode();
          led_.set_color(LED::ORANGE);
          break;
        case CURRENT:
          fast_loop_.current_mode();
          led_.set_color(LED::GREEN);
          break;
        case CURRENT_TUNING:
          fast_loop_.current_tuning_mode();
          led_.set_color(LED::SPRING);
          break;
        case POSITION_TUNING:
          phi_.init();
        case POSITION:
        case VELOCITY:
          fast_loop_.current_mode();
          led_.set_color(LED::BLUE);
          break;
        case BOARD_RESET:
          NVIC_SystemReset();
          break;
  }
  receive_data_.mode_desired = mode;
    }
    LED* led() { return &led_; }
 private:
    MainLoopParam param_;
    FastLoop &fast_loop_;
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
    friend class System;


inline uint16_t minu16(uint16_t a, uint16_t b) {
  if (a > b) {
    return b;
  } else {
    return a;
  }
}
};
