#include "usb_communication.h"
#include "usb.h"
#include <cstring>

extern uint32_t data2[16];
extern uint8_t data2_count;

void USBCommunication::init() {
    usb_ = new USB1;
}

int USBCommunication::receive_data(ReceiveData * const data) {
    // std::memcpy(data, data2, data2_count);
    // int count = data2_count;
    // data2_count = 0;
    //return count;
}

void USBCommunication::send_data(const SendData &data) {
    usb_->send_data(2, reinterpret_cast<const uint8_t *>(&data), sizeof(SendData), false);
}
