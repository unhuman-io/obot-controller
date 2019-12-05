#ifndef PWM_H
#define PWM_H

#include <cstdint>

class PWM {
 public:
   virtual void set_voltage(float v_abc[3]) = 0;
   virtual void set_vbus(float vbus) = 0;
   virtual void open_mode() = 0;
   virtual void brake_mode() = 0;
   virtual void voltage_mode() = 0;
   virtual void set_frequency_hz(uint32_t frequency_hz) = 0;
};

#endif
