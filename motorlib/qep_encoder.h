#ifndef QEP_ENCODER_H
#define QEP_ENCODER_H

#include <cstdint>
#include "encoder.h"
#include "../st_device.h"

class QEPEncoder : public Encoder {
 public:
   QEPEncoder(TIM_TypeDef &regs) : Encoder(), regs_(regs) { 
      counter_reg_ = reinterpret_cast<volatile int32_t *>(&(regs_.CNT));
      index_reg_ = reinterpret_cast<volatile int32_t *>(&(regs_.CCR3));
   }
   int32_t read() { return *counter_reg_; } __attribute__((section (".ccmram")));
   int32_t get_value() const { return *counter_reg_; } __attribute__((section (".ccmram")));
   void trigger() {} __attribute__((section (".ccmram")));
   int32_t get_index_pos() { return *index_reg_; }
   bool index_received() { return regs_.SR & TIM_SR_CC3IF; }
 private:
   TIM_TypeDef &regs_;
   volatile int32_t *counter_reg_, *index_reg_;
};

#endif
