#ifndef USB_H
#define USB_H

#include <cstdint>

// single to exit
extern uint8_t go_to_bootloader;

class USB1 {
 public:
    // limited to 64 bytes
    void send_data(uint8_t endpoint, const uint8_t *data, uint8_t length, bool wait = true);

    // receive up to length bytes from endpoint, return number of bytes read
    int receive_data(uint8_t endpoint, uint8_t * const data, uint8_t length);

    void send_string(uint8_t endpoint, const char *str, uint8_t length);

    bool tx_active(uint8_t endpoint);

    void interrupt();

 private:
     // send tx stall
    void send_stall(uint8_t endpoint);

    void handle_setup_packet(uint8_t *setup_data);

    uint8_t device_address_ = 0;
    uint16_t interface_ = 0;
    bool new_rx_data_ = false;
    uint8_t count_rx_ = 0;
    bool new_rx_data1_ = false;
    uint8_t count_rx1_ = 0;
    uint8_t rx_buffer1_[64];
    uint8_t rx_buffer_[64];
    uint32_t error_count_ = 0;
};

#endif
