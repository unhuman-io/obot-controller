
#include "fast_loop.h"
#include "foc.h"
#include <cmath>
#include "peripheral/pwm.h"
#include "util.h"
#include "encoder.h"
#include "../st_device.h"

template<class Encoder, class PWM>
FastLoop<Encoder, PWM>::FastLoop(PWM &pwm, Encoder &encoder) : pwm_(pwm), encoder_(encoder) {
    foc_ = new FOC;
}

template<class Encoder, class PWM>
FastLoop<Encoder, PWM>::~FastLoop() {
    delete foc_;
}

// called at fixed frequency in an interrupt
template<class Encoder, class PWM>
void FastLoop<Encoder, PWM>::update() {
    // trigger encoder read
    encoder_.trigger();

    timestamp_ = get_clock();

    // get ADC
    adc1 = ADC2->JDR1;
    adc2 = ADC1->JDR1;
    adc3 = ADC3->JDR2;
    adc1 = ADC5->JDR1;  // op amp channels
    adc2 = ADC4->JDR1;
    adc3 = ADC3->JDR1;
    foc_command_.measured.i_a = param_.adc1_gain*(adc1-param_.adc1_offset) - ia_bias_;
    foc_command_.measured.i_b = param_.adc2_gain*(adc2-param_.adc2_offset) - ib_bias_;
    foc_command_.measured.i_c = param_.adc3_gain*(adc3-param_.adc3_offset) - ic_bias_;
    foc_command_.desired.i_d = id_des;
    
    // get encoder value, may wait a little
    motor_enc = encoder_.read();

    motor_position_ = param_.motor_encoder.dir * 2 * (float) M_PI * inv_motor_encoder_cpr_ * motor_enc;
    motor_velocity =  param_.motor_encoder.dir * (motor_enc-last_motor_enc)*(2*(float) M_PI * inv_motor_encoder_cpr_ * frequency_hz_);
    motor_velocity_filtered = (1-alpha)*motor_velocity_filtered + alpha*motor_velocity;
    last_motor_enc = motor_enc;

    // cogging compensation, interpolate in the table
    motor_mechanical_position_ = (motor_enc - motor_index_pos_); 
    float i_pos = motor_mechanical_position_*COGGING_TABLE_SIZE*inv_motor_encoder_cpr_;
    uint16_t i = (int16_t) i_pos & (COGGING_TABLE_SIZE - 1);
 //   float ifrac = i_pos - i;  // TODO fix for negative values
    // Note (i+1) & (COGGING_TABLE_SIZE-1) allows wrap around, requires COGGING_TABLE_SIZE is multiple of 2
 //   float iq_ff = param_.cogging.gain * (param_.cogging.table[i] + ifrac * (param_.cogging.table[(i+1) & (COGGING_TABLE_SIZE-1)] - param_.cogging.table[i]));
    float iq_ff = param_.cogging.gain * param_.cogging.table[i];

    // update FOC
    foc_command_.measured.motor_encoder = phase_mode_*(motor_enc - motor_electrical_zero_pos_)*(2*(float) M_PI  * inv_motor_encoder_cpr_);
    foc_command_.desired.i_q = iq_des_gain_ * (iq_des + iq_ff);
    
    FOCStatus *foc_status = foc_->step(foc_command_);

    // output pwm
    // TODO better voltage mode
    // if (mode_ == VOLTAGE_MODE) {
    //     float abc[3] = {};
    //     pwm_.set_voltage(abc);
    // } else {
        pwm_.set_voltage(&foc_status->command.v_a);
    //}

    dt_ = (timestamp_ - last_timestamp_)*(float) (1.0f/CPU_FREQUENCY_HZ);
    dt_sum_ += dt_;
    last_timestamp_ = timestamp_;
    t_seconds_.add(dt_);
}

// called at a slow frequency in a non interrupt
template<class Encoder, class PWM>
void FastLoop<Encoder, PWM>::maintenance() {
    if (encoder_.index_received()) {
        motor_index_pos_ = encoder_.get_index_pos();
        if (param_.motor_encoder.use_index_electrical_offset_pos) {
          // motor_index_electrical_offset_pos is the value of an electrical zero minus the index position
          // motor_electrical_zero_pos is the offset to the initial encoder value
          motor_electrical_zero_pos_ = param_.motor_encoder.index_electrical_offset_pos + motor_index_pos_;
        }
    }

    if (mode_ == PHASE_LOCK_MODE) {
        motor_electrical_zero_pos_ = encoder_.get_value();
    }

    v_bus_ = ADC1->DR*param_.vbus_gain;
    v_bus_ = fmaxf(10, v_bus_);
    pwm_.set_vbus(v_bus_);
}

template<class Encoder, class PWM>
void FastLoop<Encoder, PWM>::set_param(const FastLoopParam &fast_loop_param) {
    foc_->set_param(fast_loop_param.foc_param);
    param_ = fast_loop_param;
    set_phase_mode();
    inv_motor_encoder_cpr_ = param_.motor_encoder.cpr != 0 ? 1.f/param_.motor_encoder.cpr : 0;
    frequency_hz_ = param_.pwm_frequency;
}

template<class Encoder, class PWM>
void FastLoop<Encoder, PWM>::voltage_mode() {
    pwm_.voltage_mode();
    mode_ = VOLTAGE_MODE;
}

template<class Encoder, class PWM>
void FastLoop<Encoder, PWM>::zero_current_sensors() {
    ia_bias_ = (1-alpha_zero_)*ia_bias_ + alpha_zero_* param_.adc1_gain*(adc1-param_.adc1_offset);
    ib_bias_ = (1-alpha_zero_)*ib_bias_ + alpha_zero_* param_.adc2_gain*(adc2-param_.adc2_offset);
    ic_bias_ = (1-alpha_zero_)*ic_bias_ + alpha_zero_* param_.adc3_gain*(adc3-param_.adc3_offset);
}

template<class Encoder, class PWM>
void FastLoop<Encoder, PWM>::phase_lock_mode(float id) {
    phase_mode_ = 0;
    id_des = id;
    iq_des_gain_ = 0;
    pwm_.voltage_mode();
    mode_ = PHASE_LOCK_MODE;
}

template<class Encoder, class PWM>
void FastLoop<Encoder, PWM>::current_mode() {
    phase_mode_ = phase_mode_desired_;
    id_des = 0;
    iq_des_gain_ = 1;
    pwm_.voltage_mode();
    mode_ = CURRENT_MODE;
}

template<class Encoder, class PWM>
void FastLoop<Encoder, PWM>::brake_mode() {
    pwm_.brake_mode();
    mode_ = BRAKE_MODE;
}

template<class Encoder, class PWM>
void FastLoop<Encoder, PWM>::open_mode() {
    pwm_.open_mode();
    mode_ = OPEN_MODE;
}

template<class Encoder, class PWM>
void FastLoop<Encoder, PWM>::get_status(FastLoopStatus *fast_loop_status) {
    foc_->get_status(&(fast_loop_status->foc_status));
    fast_loop_status->motor_mechanical_position = motor_mechanical_position_;
    fast_loop_status->foc_command = foc_command_;
    fast_loop_status->motor_position.position = motor_position_;
    fast_loop_status->motor_position.velocity = motor_velocity_filtered;
    fast_loop_status->motor_position.raw = motor_enc;
    fast_loop_status->timestamp = timestamp_;
    fast_loop_status->t_seconds = t_seconds_.value();
    fast_loop_status->dt = dt_sum_;
    fast_loop_status->vbus = v_bus_;
    dt_sum_ = 0;
}

template<class Encoder, class PWM>
void FastLoop<Encoder, PWM>::set_phase_mode() {
    phase_mode_desired_ = param_.phase_mode == 0 ? 1 : -1;
}