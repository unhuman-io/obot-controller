
#include "main_loop.h"
#include "led.h"
#include "control_fun.h"

//#include "../Src/pin_config.h"
#include <cmath>
//#include "stm32f4xx_hal.h"
#include "usb_communication.h"
#include "foc_i.h"

void MainLoop::init() {
    communication_.init();
}

#include "../Src/usb/usbd_rt_if.h"

void MainLoop::update() {
  count_++;
  // int count_received = communication_.receive_data(&receive_data_);
  // if (count_received) {
  //   if (mode_ != static_cast<MainControlMode>(receive_data_.mode_desired)) {
  //     mode_ = static_cast<MainControlMode>(receive_data_.mode_desired);
  //     switch (mode_) {
  //       case OPEN:
  //       default:
  //         fast_loop_open_mode();
  //         break;
  //       case BRAKE:
  //         fast_loop_brake_mode();
  //         break;
  //       case NORMAL_CONTROL:
  //         fast_loop_current_mode();
  //         break;
  //     }
  //   }
  // }
  
  fast_loop_get_status(&fast_loop_status_);

  float iq_des = controller_.step(receive_data_.position_desired, fast_loop_status_.motor_position.position) + \
              receive_data_.current_desired;

  fast_loop_set_iq_des(iq_des);
  SendData send_data;
  send_data.iq = fast_loop_status_.foc_status.measured.i_q;
  send_data.host_timestamp_received = receive_data_.host_timestamp;
  send_data.mcu_timestamp = fast_loop_status_.timestamp;
  send_data.motor_encoder = fast_loop_status_.motor_position.raw;
  send_data.motor_position = fast_loop_status_.motor_position.position;
  send_data.joint_position = 0;
  send_data.reserved[0] = fast_loop_status_.foc_status.measured.i_0;
    USB_send(reinterpret_cast<uint8_t *>(&count_),sizeof(count_));
 // communication_.send_data(send_data);
  led_.update();
}

void MainLoop::set_param(MainLoopParam &param) {
    controller_.set_param(param.controller_param);
    param_ = param;
}

void MainLoop::get_status(MainLoopStatus * const main_loop_status) const {
  //main_loop_status->torque = torque;
}
