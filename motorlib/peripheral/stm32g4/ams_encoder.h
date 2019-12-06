#ifndef AMS_ENCODER
#define AMS_ENCODER

#include "../../encoder.h"
#include "../st_device.h"


class GPIO;

class AMSEncoder : public Encoder {
 public:
    AMSEncoder(SPI_TypeDef &regs, GPIO &gpio_cs) : Encoder(), regs_(regs), gpio_cs_(gpio_cs) {} 
    //void init() {}
    virtual int32_t read()  __attribute__((section (".ccmram")));
    virtual int32_t get_value()  const __attribute__((section (".ccmram")));
    virtual void trigger()  __attribute__((section (".ccmram")));
    virtual bool index_received() { return true; }
 protected:
    SPI_TypeDef &regs_;
    GPIO &gpio_cs_;
    uint16_t last_data_ = 0;
    int32_t count_ = 0;
};

#endif
