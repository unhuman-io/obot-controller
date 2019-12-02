#include "spi_encoder.h"

// max clock currently working is 180 MHz/128 = 1.4 MHz
class Aksim2Encoder : public SPIEncoder {
 public:
    Aksim2Encoder(SPI_TypeDef &regs, GPIO &gpio_cs) : SPIEncoder(regs, gpio_cs) {} 
    void init();
    virtual int32_t get_value()  __attribute__((section (".ccmram")));;
    virtual void trigger()  __attribute__((section (".ccmram")));;
 private:
    int count = 0;
    uint32_t raw_value_ = 0;
    uint32_t last_raw_value_ = 0;
    int32_t value_ = 0;
    uint16_t rx_data_[4] = {}, tx_data_[4] = {};
    bool init_ = false;
};