#ifndef HALL_H
#define HALL_H

#include "encoder.h"

class GPIO;
#include "gpio.h"

class HallEncoder : public Encoder {
 public:
    HallEncoder(GPIO &gpio_a, GPIO &gpio_b, GPIO &gpio_c) :
        gpio_a_(gpio_a), gpio_b_(gpio_b), gpio_c_(gpio_c) {}
    int32_t read() __attribute__((section (".ccmram"))) { 
        uint8_t hall_read = gpio_a_.get_value() | (gpio_b_.get_value() << 1) | (gpio_c_.get_value() << 2);
        uint8_t hall_count = hall_table_[hall_read];
        if (hall_count != 0) {
            int8_t diff = hall_count - last_hall_count_;
            last_hall_count_ = hall_count;
            if (diff < -3) {
                diff += 6;
            } else if (diff > 3) {
                diff -= 6;
            }
            count_ += diff;
        }
        return count_;
    }
    int32_t get_value() const __attribute__((section (".ccmram"))) { return count_; }
    void trigger() __attribute__((section (".ccmram"))) {}
    int32_t get_index_pos() { return 0; }
    bool index_received() { return true; }
 private:
    GPIO &gpio_a_, &gpio_b_, &gpio_c_;
    int32_t count_ = 0;
    uint8_t last_hall_count_ = 0;
    uint8_t hall_table_[8] = {
    0, 1, 3, 2, 5, 6, 4, 0 //0, 1, 3, 2, 6, 4, 5, 0
};
};

#endif
