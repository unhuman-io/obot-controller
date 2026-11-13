#pragma once
// Field oriented current control, the math of motorlib/foc.cpp. It is the
// fast loop's controller: update(command, measured) takes the phase currents
// and the motor angle and returns the phase voltages.
#include <cmath>
#include "control_fun.h"
#include "sincos.h"

// What the output stage does
enum class FOCMode : uint8_t {
    OPEN,           // all switches off
    BRAKE,          // low sides on: the windings are shorted
    VOLTAGE,        // v_q applied, no current control
    CURRENT,        // i_d and i_q controlled, v_q is a feed forward
    PHASE_LOCK,     // i_d controlled at electrical angle 0, to find that angle
};

struct FOCCommand {
    FOCMode mode = FOCMode::OPEN;
    float i_d = 0, i_q = 0;     // A
    float v_q = 0;              // V
};

struct FOCParam {
    float num_poles = 7;                        // electrical revolutions per motor revolution
    PIParam pi_d{}, pi_q{};
    float current_filter_frequency_hz = 0;      // on the measured i_d and i_q
    float id_rate_limit = 0, iq_rate_limit = 0; // A/s on the desired currents, 0: none
    float voltage_limit = 0;                    // V on the magnitude of v_d, v_q, 0: none
};

struct FOCStatus {
    FOCMode mode = FOCMode::OPEN;
    float v_a = 0, v_b = 0, v_c = 0;                    // output, V
    float v_d = 0, v_q = 0;
    float i_d_desired = 0, i_q_desired = 0;             // after the rate limit
    float i_d = 0, i_q = 0, i_0 = 0;                    // measured, filtered
};

// The measurement type needs i_a, i_b, i_c (A) and motor_electrical_position
// (rad at the motor shaft: the electrical angle is num_poles times it)
template<FOCParam param, float dt>
class FOC {
 public:
    using Command = FOCCommand;
    using Output = FOCStatus;

    constexpr FOC() = default;

    template<typename Measured>
    Output update(const Command& command, const Measured& measured) {
        constexpr float sqrt3 = std::numbers::sqrt3_v<float>;
        if (command.mode != status_.mode) {
            const bool current_control = command.mode == FOCMode::CURRENT || command.mode == FOCMode::PHASE_LOCK;
            if (current_control) {
                pi_id_.init();
                pi_iq_.init();
            }
            i_gain_ = current_control ? 1 : 0;
            status_.mode = command.mode;
        }
        const bool phase_lock = command.mode == FOCMode::PHASE_LOCK;
        const Sincos sincos = sincos1(phase_lock ? 0 : measured.motor_electrical_position * param.num_poles);
        const float sin_t = sincos.sin, cos_t = sincos.cos;

        const float i_alpha = (2.f / 3) * measured.i_a - (1.f / 3) * measured.i_b - (1.f / 3) * measured.i_c;
        const float i_beta = (1 / sqrt3) * measured.i_b - (1 / sqrt3) * measured.i_c;
        const float i_d_measured = id_filter_.update(cos_t * i_alpha - sin_t * i_beta);
        const float i_q_measured = iq_filter_.update(sin_t * i_alpha + cos_t * i_beta);

        const float i_d_desired = id_limiter_.step(command.i_d);
        const float i_q_desired = iq_limiter_.step(phase_lock ? 0 : command.i_q);

        const float v_d_desired = i_gain_ * pi_id_.step(i_d_desired, i_d_measured);
        const float v_q_desired = i_gain_ * pi_iq_.step(i_q_desired, i_q_measured) + command.v_q;

        const float magnitude = std::sqrt(v_d_desired * v_d_desired + v_q_desired * v_q_desired);
        const float magnitude_limited = magnitude > voltage_limit ? voltage_limit : magnitude;
        const float scale = magnitude_limited / (magnitude + std::numeric_limits<float>::epsilon());
        const float v_d = v_d_desired * scale;
        const float v_q = v_q_desired * scale;

        const float v_alpha = cos_t * v_d + sin_t * v_q;
        const float v_beta = -sin_t * v_d + cos_t * v_q;

        status_.v_a = (2.f / 3) * v_alpha;
        status_.v_b = (-1.f / 3) * v_alpha + (1 / sqrt3) * v_beta;
        status_.v_c = (-1.f / 3) * v_alpha - (1 / sqrt3) * v_beta;
        status_.v_d = v_d;
        status_.v_q = v_q;
        status_.i_d_desired = i_d_desired;
        status_.i_q_desired = i_q_desired;
        status_.i_d = i_d_measured;
        status_.i_q = i_q_measured;
        status_.i_0 = measured.i_a + measured.i_b + measured.i_c;
        return status_;
    }

 private:
    static constexpr float voltage_limit = param.voltage_limit == 0 ? control_infinity : param.voltage_limit;
    float i_gain_ = 0;
    PIController pi_id_{param.pi_d}, pi_iq_{param.pi_q};
    FirstOrderLowPassFilter id_filter_{dt, param.current_filter_frequency_hz}, iq_filter_{dt, param.current_filter_frequency_hz};
    RateLimiter id_limiter_{param.id_rate_limit * dt}, iq_limiter_{param.iq_rate_limit * dt};
    FOCStatus status_{};
};
