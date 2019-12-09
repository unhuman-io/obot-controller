#include "ams_encoder.h"
#include "../../gpio.h"

void AMSEncoder::trigger() {
    gpio_cs_.clear();
    asm("NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP;");
    asm("NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP;");
    asm("NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP;");
    asm("NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP;");
    asm("NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP;");
    // 350 ns delay as5147p
    regs_.DR = 0x7FFE;
}

int32_t AMSEncoder::read() {
    while(!(regs_.SR & SPI_SR_RXNE)); // RXNE: 1 -> data available
    uint16_t data = (regs_.DR & 0x3FFF)<<2;

    count_ += (int16_t) (data - last_data_); // rollover summing
    last_data_ = data;

    asm("NOP; NOP; NOP; NOP;");
    gpio_cs_.set();
    return count_;
}

int32_t AMSEncoder::get_value() const {
  return count_;
}