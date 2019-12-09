#include "spi_encoder.h"
#include "../../gpio.h"

void SPIEncoder::trigger() {
    gpio_cs_.clear();
    asm("NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP;");
    // wait tdelay, 80 ns ma732 (14.4 cycles at 180 MHz)
    // 300 ns AEAT-8800
    regs_.DR = 0;
}

int32_t SPIEncoder::read() {
    // wait until SPI_FLAG_RXNE, ma 732 max frequency 25 Mbps, 640 ns (115 cycles at 180 MHz)
    while(!(regs_.SR & SPI_SR_RXNE)); // RXNE: 1 -> data available
    uint16_t data = regs_.DR;

    count_ += (int16_t) (data - last_data_); // rollover summing
    last_data_ = data;
    // wait tdelay, 25 ns ma732
    // 200 ns AEAT-8800
    asm("NOP; NOP; NOP; NOP;");
    gpio_cs_.set();
    return count_;
}

int32_t SPIEncoder::get_value() const {
  return count_;
}