#ifndef SPI_ENCODER
#define SPI_ENCODER

#include "../../encoder.h"
#include "../st_device.h"


class GPIO;

class SPIEncoder : public Encoder {
 public:
    SPIEncoder(SPI_TypeDef &regs, GPIO &gpio_cs) : Encoder(), regs_(regs), gpio_cs_(gpio_cs) {} 
    //void init() {}
    virtual int32_t get_value()  __attribute__((section (".ccmram")));
    virtual void trigger()  __attribute__((section (".ccmram")));
    virtual bool index_received() { return true; }
 protected:
    SPI_TypeDef &regs_;
    GPIO &gpio_cs_;
};

#endif
