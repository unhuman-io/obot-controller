#ifndef MOTOR_FOC_H
#define MOTOR_FOC_H

#include "messages.h"

class PIController;
class FirstOrderLowPassFilter;

class FOC {
public:
    FOC(float dt);
    ~FOC();

    FOCStatus * const step(const FOCCommand &command)  __attribute__((section (".ccmram")));
    void set_param(const FOCParam &param);
    void get_status(FOCStatus *status) const { *status = status_; }

private:
    uint16_t num_poles_ = 7;
    PIController *pi_id_, *pi_iq_;
    FOCStatus status_;
    static const float Kc[2][3];
    float dt_;
    FirstOrderLowPassFilter *id_filter_, *iq_filter_;
};


#endif //MOTOR_FOC_H
