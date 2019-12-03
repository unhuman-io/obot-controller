#include "foc.h"
//#include "hal_fun.h"
//#include "hal_adc.h"

//#include "hal_pwm.h"
//#include "../sensor/encoder.h"
#include "control_fun.h"
#include "sincos.h"

#include <cmath>

FOC::FOC() { //hal::PWM *pwm, const hal::ADC &adc, const Encoder &encoder) : pwm_(pwm), adc_(adc), encoder_(encoder) {
    pi_id_ = new PIController();
    pi_iq_ = new PIController();
}

FOC::~FOC() {
    delete pi_id_;
    delete pi_iq_;
}

#define SQRT3 (float) std::sqrt(3)
const float FOC::Kc[2][3] = {{2.0/3, -1.0/3, -1.0/3},
                               {0,      1/SQRT3, -1/SQRT3}};


#include "../st_device.h"

float alpha_current = 1;
float id_last = 0, iq_last=0;
float alpha_current2 = 1;
float ial_last = 0, ibe_last=0;
FOCStatus * const FOC::step(const FOCCommand &command) {
    status_.measured.position = command.measured.motor_encoder;
    status_.desired.i_d = command.desired.i_d;
    status_.desired.i_q = command.desired.i_q;
    float i_abc_measured[3] = {command.measured.i_a, command.measured.i_b, command.measured.i_c};
    float electrical_angle = status_.measured.position * num_poles_;

    //float sin_t = std::sin(electrical_angle);
    //float cos_t = std::cos(electrical_angle);

    Sincos sincos = sincos1(electrical_angle);
    float &sin_t = sincos.sin;
    float &cos_t = sincos.cos;
    float  i_alpha_measured = Kc[0][0] * i_abc_measured[0] +
            Kc[0][1] * i_abc_measured[1] +
            Kc[0][2] * i_abc_measured[2];
    float  i_beta_measured = Kc[1][0] * i_abc_measured[0] +
                     Kc[1][1] * i_abc_measured[1] +
                     Kc[1][2] * i_abc_measured[2];

    i_alpha_measured = alpha_current2*i_alpha_measured + (1-alpha_current2)*ial_last;
    ial_last = i_alpha_measured;
    i_beta_measured = alpha_current2*i_beta_measured + (1-alpha_current2)*ibe_last;
    ibe_last = i_beta_measured;

    float i_d_measured = cos_t * i_alpha_measured - sin_t * i_beta_measured;
    float i_q_measured = sin_t * i_alpha_measured + cos_t * i_beta_measured;

    i_d_measured = alpha_current*i_d_measured + (1-alpha_current)*id_last;
    id_last = i_d_measured;
    i_q_measured = alpha_current*i_q_measured + (1-alpha_current)*iq_last;
    iq_last = i_q_measured;

   float v_d_desired = pi_id_->step(status_.desired.i_d, i_d_measured);
   float v_q_desired = pi_iq_->step(status_.desired.i_q, i_q_measured);
    // float v_d_desired = status_.desired.i_d;
    // float v_q_desired =status_.desired.i_q;
    //float v_d_desired = 0;
    //float v_q_desired = status_.desired.i_q*((float) rand()/RAND_MAX-.5);

    float v_alpha_desired = cos_t * v_d_desired + sin_t * v_q_desired;
    float v_beta_desired = -sin_t * v_d_desired + cos_t * v_q_desired;

    float v_a_desired = Kc[0][0] * v_alpha_desired + Kc[1][0] * v_beta_desired;
    float v_b_desired = Kc[0][1] * v_alpha_desired + Kc[1][1] * v_beta_desired;
    float v_c_desired = Kc[0][2] * v_alpha_desired + Kc[1][2] * v_beta_desired;

    status_.command.v_a = v_a_desired;
    status_.command.v_b = v_b_desired;
    status_.command.v_c = v_c_desired;
    status_.command.v_d = v_d_desired;
    status_.command.v_q = v_q_desired;
    status_.measured.i_d = i_d_measured;
    status_.measured.i_q = i_q_measured;
    status_.measured.i_0 = i_abc_measured[0] + i_abc_measured[1] + i_abc_measured[2];
    return &status_;
}

void FOC::set_param(const FOCParam &param) {
    pi_id_->set_param(param.pi_d);
    pi_iq_->set_param(param.pi_q);
    num_poles_ = param.num_poles;
}