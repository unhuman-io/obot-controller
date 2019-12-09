#include "usb_communication.h"
#include "peripheral/usb.h"


int USBCommunication::receive_data(ReceiveData * const data) {
    return usb_.receive_data(2, (uint8_t *const) data, sizeof(*data));
}

void USBCommunication::send_data(const SendData &data) {
    usb_.send_data(2, reinterpret_cast<const uint8_t *>(&data), sizeof(SendData), false);
}
