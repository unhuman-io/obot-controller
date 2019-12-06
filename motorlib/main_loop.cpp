
#include "main_loop.h"
#include "led.h"
#include "control_fun.h"

#include <cmath>

#include "usb_communication.h"
#include "foc_i.h"
#include "encoder.h"
#include "sincos.h"
#include "../st_device.h"

void MainLoop::init() {
}

void MainLoop::set_mode(MainControlMode mode) {
  mode_ = mode;
  switch (mode) {
    case OPEN:
    default:
      fast_loop_open_mode();
      led_.set_color(LED::AZURE);
      break;
    case DAMPED:
      fast_loop_brake_mode();
      led_.set_color(LED::ORANGE);
      break;
    case CURRENT:
    case CURRENT_TUNING:
      fast_loop_current_mode();
      led_.set_color(LED::GREEN);
      break;
    case POSITION:
    case POSITION_TUNING:
      fast_loop_current_mode();
      led_.set_color(LED::BLUE);
      break;
  }
  receive_data_.mode_desired = mode;
}

void MainLoop::update() {
  count_++;
  int count_received = communication_.receive_data(&receive_data_);
  if (count_received) {
    if (mode_ != static_cast<MainControlMode>(receive_data_.mode_desired)) {
      set_mode(static_cast<MainControlMode>(receive_data_.mode_desired));
      dt_sum_ = 0;
    }
  }
  output_encoder_.trigger();
  fast_loop_get_status(&fast_loop_status_);
  dt_sum_ += fast_loop_status_.dt;

  float iq_des = 0;
  switch (mode_) {
    case CURRENT:
      iq_des = receive_data_.current_desired;
      break;
    case POSITION:
      iq_des = controller_.step(receive_data_.position_desired, receive_data_.reserved, fast_loop_status_.motor_position.position) + \
              receive_data_.current_desired;
      break;
    case POSITION_TUNING: 
    {
      Sincos sincos;
      if (dt_sum_ > 1.0f/receive_data_.reserved) {
        dt_sum_ -= 1.0f/receive_data_.reserved;
      }
      if (receive_data_.reserved != last_receive_data_.reserved) {
        dt_sum_ = 0;
      }
      sincos = sincos1(2 * (float) M_PI * receive_data_.reserved * fast_loop_status_.t_seconds); //dt_sum_);
      float pos_desired = receive_data_.position_desired*(receive_data_.reserved > 0 ? sincos.sin : ((sincos.sin > 0) - (sincos.sin < 0)));
      float vel_desired = receive_data_.reserved > 0 ? 2 * (float) M_PI * receive_data_.reserved * (1.0f/CPU_FREQUENCY_HZ) * sincos.cos : 0;
      iq_des = controller_.step(pos_desired, vel_desired, fast_loop_status_.motor_position.position);
      break;
    }
    default:
      break;
  }

  fast_loop_set_iq_des(iq_des);
  SendData send_data;
  send_data.iq = fast_loop_status_.foc_status.measured.i_q;
  send_data.host_timestamp_received = receive_data_.host_timestamp;
  send_data.mcu_timestamp = fast_loop_status_.timestamp;
  send_data.motor_encoder = fast_loop_status_.motor_position.raw;
  send_data.motor_position = fast_loop_status_.motor_position.position;
  send_data.joint_position = output_encoder_.get_value();//*2.0*(float) M_PI/param_.output_encoder.cpr;
  send_data.reserved[0] = fast_loop_status_.foc_status.measured.i_0;
  communication_.send_data(send_data);
  led_.update();
  last_receive_data_ = receive_data_;
}

void MainLoop::set_param(MainLoopParam &param) {
    controller_.set_param(param.controller_param);
    param_ = param;
}

void MainLoop::get_status(MainLoopStatus * const main_loop_status) const {
  //main_loop_status->torque = torque;
}
