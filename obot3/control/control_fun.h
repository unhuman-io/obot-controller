#pragma once
// The control building blocks of motorlib/control_fun.h, header only and with
// their parameters fixed at construction: an object built from a constexpr
// parameter struct is constant initialized, so it can be a member of the
// constinit loops.
#include <cstdint>
#include <limits>
#include <numbers>

inline constexpr float control_infinity = std::numeric_limits<float>::infinity();

constexpr float fsat(float a, float sat) {
    float b = a > sat ? sat : a;
    b = b < -sat ? -sat : b;
    return b;
}

constexpr float fsat2(float a, float min, float max) {
    float b = a > max ? max : a;
    b = b < min ? min : b;
    return b;
}

constexpr float fabsf2(float f) { return f >= 0 ? f : -f; }

constexpr bool is_finite(float f) { return f - f == 0; }

// value in -rollover..rollover
template<class T>
constexpr T wrap1(T value, T rollover) {
    T diff = 2 * rollover;
    if (value > rollover) value -= diff;
    if (value < -rollover) value += diff;
    return value;
}

// value - value2 across the rollover
template<class T>
constexpr T wrap1_diff(T value, T value2, T rollover) {
    T diff = value - value2;
    T diff2 = 2 * rollover;
    if (diff > rollover) diff = diff - diff2;
    if (diff < -rollover) diff = diff + diff2;
    return diff;
}

// frequency_hz 0: no filtering
class FirstOrderLowPassFilter {
 public:
    constexpr FirstOrderLowPassFilter(float dt, float frequency_hz = 0) :
        alpha_(frequency_hz == 0 ? 1 : 2 * std::numbers::pi_v<float> * dt * frequency_hz / (2 * std::numbers::pi_v<float> * dt * frequency_hz + 1)) {}
    constexpr void init(float value) { value_ = value; }
    constexpr float update(float value) {
        value_ = alpha_ * value + (1 - alpha_) * value_;
        return value_;
    }
    constexpr float get_value() const { return value_; }
 private:
    float value_ = 0;
    float alpha_;
};

class SecondOrderLowPassFilter {
 public:
    constexpr SecondOrderLowPassFilter(float dt, float frequency_hz = 0) : low_pass_1_(dt, frequency_hz), low_pass_2_(dt, frequency_hz) {}
    constexpr void init(float value) {
        low_pass_1_.init(value);
        low_pass_2_.init(value);
    }
    constexpr float update(float value) { return low_pass_2_.update(low_pass_1_.update(value)); }
    constexpr float get_value() const { return low_pass_2_.get_value(); }
 private:
    FirstOrderLowPassFilter low_pass_1_, low_pass_2_;
};

// limit per step, 0: no limit
class RateLimiter {
 public:
    constexpr RateLimiter(float limit = 0) : limit_(limit == 0 ? control_infinity : limit) {}
    constexpr void set_limit(float limit) { limit_ = limit == 0 ? control_infinity : limit; }
    constexpr void init(float value) { last_value_ = value; }
    constexpr float step(float value) {
        float out_value = value;
        if (value > last_value_ + limit_) {
            out_value = last_value_ + limit_;
        } else if (value < last_value_ - limit_) {
            out_value = last_value_ - limit_;
        }
        last_value_ = out_value;
        return out_value;
    }
 private:
    float limit_;
    float last_value_ = 0;
};

// ki is per step, as in motorlib
struct PIParam {
    float kp = 0, ki = 0, ki_limit = 0, command_max = 0;
};

class PIController {
 public:
    constexpr PIController(const PIParam& param) : param_(param) {}
    constexpr void init() { ki_sum_ = 0; }
    constexpr float step(float desired, float measured) {
        float error = desired - measured;
        ki_sum_ = fsat(ki_sum_ + param_.ki * error, param_.ki_limit);
        return fsat(param_.kp * error + ki_sum_, param_.command_max);
    }
 private:
    PIParam param_;
    float ki_sum_ = 0;
};

struct PIDParam {
    float kp = 0, ki = 0, ki_limit = 0, kd = 0, command_max = 0;
    float velocity_filter_frequency_hz = 0, output_filter_frequency_hz = 0;
};

// out = (ki/s + kp + kd*s) * error
// with s*error from velocity_desired and the velocity of measured through a
// second order filter, ki/s * error saturated at ki_limit, out through a
// first order filter (for a lead controller) and saturated at command_max.
// velocity_limit limits the rate of the desired value.
class PIDController {
 public:
    constexpr PIDController(float dt, const PIDParam& param) : kp_(param.kp), kd_(param.kd), ki_(param.ki), ki_limit_(param.ki_limit),
        command_max_(param.command_max), velocity_filter_(dt, param.velocity_filter_frequency_hz),
        output_filter_(dt, param.output_filter_frequency_hz), dt_(dt) {}
    constexpr void init(float measured) {
        rate_limit_.init(measured);
        ki_sum_ = 0;
        measured_last_ = measured;
        velocity_filter_.init(0);
        output_filter_.init(0);
    }
    constexpr float step(float desired, float velocity_desired, float measured, float velocity_limit = control_infinity) {
        rate_limit_.set_limit(fabsf2(velocity_limit * dt_));
        float proxy_desired = rate_limit_.step(desired);
        float proxy_velocity_desired = fsat(velocity_desired, velocity_limit);
        float error = proxy_desired - measured;
        float velocity_measured = velocity_filter_.update((measured - measured_last_) / dt_);
        float error_dot = proxy_velocity_desired - velocity_measured;
        measured_last_ = measured;
        ki_sum_ = fsat(ki_sum_ + ki_ * dt_ * error, ki_limit_);
        float filtered_out = output_filter_.update(kp_ * error + ki_sum_ + kd_ * error_dot);
        return fsat(filtered_out, command_max_);
    }

    float kp_, kd_, ki_, ki_limit_, command_max_;
 private:
    float ki_sum_ = 0;
    SecondOrderLowPassFilter velocity_filter_;
    FirstOrderLowPassFilter output_filter_;
    float measured_last_ = 0;
    float dt_;
    RateLimiter rate_limit_;
};

// The newest value of a T, between two interrupt priorities (or an interrupt
// and the background) in either direction. The writer fills the slot that is
// not current and then makes it current with one store. A reader that
// interrupts the writer gets the previous value; a reader that is interrupted
// reads a slot the writer leaves alone until its second write, and repeats in
// that case. One writer; T is copied whole.
template<typename T>
class Latest {
 public:
    constexpr Latest() = default;
    void write(const T& value) {
        const uint32_t sequence = sequence_;
        slots_[(sequence + 1) & 1] = value;
        sequence_ = sequence + 1;
    }
    T read() const {
        T value;
        uint32_t sequence;
        do {
            sequence = sequence_;
            value = slots_[sequence & 1];
        } while (sequence_ - sequence >= 2);
        return value;
    }
 private:
    T slots_[2] = {};
    volatile uint32_t sequence_ = 0;
};
