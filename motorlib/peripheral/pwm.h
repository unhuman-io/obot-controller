#ifndef PWM_H
#define PWM_H

#include <cstdint>

class PWM {
 public:
   void set_voltage(float v_abc[3]) {}
   void set_vbus(float vbus) {}
   void open_mode() {}
   void brake_mode() {}
   void voltage_mode() {}
   void set_frequency_hz(uint32_t frequency_hz) {}
};

#endif
