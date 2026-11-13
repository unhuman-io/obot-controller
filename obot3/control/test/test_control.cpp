// Host test of the control code, no hardware:
//   g++ -std=c++23 -O2 -I ../.. test_control.cpp -o /tmp/test_control && /tmp/test_control
// The FOC and the controllers run against simple motor models with the
// tmotor_driver gains.
#include <cmath>
#include <cstdio>
#include <cstdint>
#include "control/motor_controller.h"

static int failures = 0;
#define CHECK(condition, ...) do { \
    const bool ok = (condition); \
    if (!ok) failures++; \
    std::printf("%s  ", ok ? "pass" : "FAIL"); std::printf(__VA_ARGS__); std::printf("\n"); } while (0)

constexpr float fast_dt = 1.f / 50000, main_dt = 1.f / 10000;
constexpr float num_poles = 14;
constexpr FOCParam foc_param = {
    .num_poles = num_poles,
    .pi_d = {.kp = 2, .ki = .1f, .ki_limit = 10, .command_max = 10},
    .pi_q = {.kp = 2, .ki = .1f, .ki_limit = 10, .command_max = 10},
    .current_filter_frequency_hz = 40000,
};
constexpr MotorControllerParam controller_param = {
    .position = {.kp = 20, .kd = .2f, .command_max = 3, .velocity_filter_frequency_hz = 200},
    .velocity = {.kp = .2f, .ki = 20, .ki_limit = 3, .command_max = 3, .output_filter_frequency_hz = 200},
    .acceleration_limit = 1000,
};

struct Measured {
    uint32_t timestamp = 0;
    float i_a = 0, i_b = 0, i_c = 0;
    float motor_electrical_position = 0, motor_position = 0, motor_velocity = 0;
    int32_t motor_encoder = 0;
};

// Three phase R-L motor with a rotor: phase voltages in, phase currents and
// position out. Torque is kt * i_q in the rotor frame.
struct Motor {
    static constexpr float R = .5f, L = 200e-6f, kt = .1f, J = 1e-4f, friction = 1e-3f;
    float i_alpha = 0, i_beta = 0, position = 0, velocity = 0, i_q = 0, i_d = 0;
    void step(float v_a, float v_b, float v_c, float dt) {
        const float v_alpha = (2.f / 3) * v_a - (1.f / 3) * v_b - (1.f / 3) * v_c;
        const float v_beta = (v_b - v_c) / std::sqrt(3.f);
        const float angle = position * num_poles;
        const float s = std::sin(angle), c = std::cos(angle);
        // back emf along q, in the same frame convention as the FOC
        const float emf = kt * velocity;
        const float e_alpha = s * emf, e_beta = c * emf;
        i_alpha += dt * (v_alpha - e_alpha - R * i_alpha) / L;
        i_beta += dt * (v_beta - e_beta - R * i_beta) / L;
        i_d = c * i_alpha - s * i_beta;
        i_q = s * i_alpha + c * i_beta;
        velocity += dt * (kt * i_q - friction * velocity) / J;
        position += dt * velocity;
    }
    Measured measure() const {
        Measured m;
        m.i_a = i_alpha;
        m.i_b = -.5f * i_alpha + std::sqrt(3.f) / 2 * i_beta;
        m.i_c = -.5f * i_alpha - std::sqrt(3.f) / 2 * i_beta;
        m.motor_electrical_position = position;
        m.motor_position = position;
        m.motor_velocity = velocity;
        return m;
    }
};

struct HostCommand {
    static inline obot::MotorCommand command{};
    static obot::MotorCommand read() { return command; }
};

// What the loops hand to the controller policy
struct Status {
    struct ExpectedCommand { FOCCommand foc; };
    struct { Measured measured; FOCStatus controller; } fast_loop;
    struct { float output_position = 0, torque = 0; } sensors;
};

using TestFOC = FOC<foc_param, fast_dt>;
using TestPolicy = MotorControllerPolicy<controller_param, main_dt, HostCommand>;

// The two loops against the motor: 5 fast loops per main loop
struct Rig {
    Motor motor;
    TestFOC foc;
    TestPolicy policy;
    Status status;
    FOCCommand foc_command;
    void run(float seconds) {
        const int steps = seconds / fast_dt;
        for (int i = 0; i < steps; i++) {
            if (i % 5 == 0) {
                status.sensors.torque = Motor::kt * motor.i_q;
                foc_command = policy.update(status).foc;
            }
            status.fast_loop.measured = motor.measure();
            status.fast_loop.controller = foc.update(foc_command, status.fast_loop.measured);
            const FOCStatus& out = status.fast_loop.controller;
            const bool driven = out.mode != FOCMode::OPEN && out.mode != FOCMode::BRAKE;
            motor.step(driven ? out.v_a : 0, driven ? out.v_b : 0, driven ? out.v_c : 0, fast_dt);
        }
    }
};

int main() {
    // sincos against libm over several turns
    float max_error = 0;
    for (float x = -20; x < 20; x += .0137f) {
        const Sincos sc = sincos1(x);
        max_error = std::fmax(max_error, std::fabs(sc.sin - std::sin(x)));
        max_error = std::fmax(max_error, std::fabs(sc.cos - std::cos(x)));
    }
    CHECK(max_error < 2e-6f, "sincos max error %g", max_error);

    // filters and limiters
    FirstOrderLowPassFilter filter(main_dt, 100);
    float filtered = 0;
    for (int i = 0; i < 16; i++) filtered = filter.update(1);   // 1.6 ms, a time constant of the 100 Hz filter
    CHECK(std::fabs(filtered - (1 - std::exp(-1.f))) < .03f, "first order filter after one time constant %g", filtered);
    RateLimiter limiter(.1f);
    limiter.step(1);
    CHECK(std::fabs(limiter.step(1) - .2f) < 1e-6f, "rate limiter");
    CHECK(wrap1_diff(3.f, -3.f, 3.14159f) < 0, "wrap1_diff across the rollover");

    // latest value hand off
    Latest<FOCStatus> latest;
    FOCStatus value;
    value.v_q = 3;
    latest.write(value);
    value.v_q = 4;
    latest.write(value);
    CHECK(latest.read().v_q == 4, "latest value");

    // open: no voltage whatever the command values
    {
        Rig rig;
        HostCommand::command = {};
        HostCommand::command.mode_desired = obot::OPEN;
        HostCommand::command.current_desired = 2;
        rig.run(.05f);
        CHECK(rig.status.fast_loop.controller.mode == FOCMode::OPEN && rig.motor.i_q == 0, "open mode: i_q %g", rig.motor.i_q);
    }

    // current mode: i_q follows, i_d stays at 0, while the motor spins up
    {
        Rig rig;
        HostCommand::command = {};
        HostCommand::command.mode_desired = obot::CURRENT;
        HostCommand::command.current_desired = 1;
        rig.run(.05f);
        CHECK(std::fabs(rig.motor.i_q - 1) < .05f, "current mode: i_q %g for 1 A", rig.motor.i_q);
        CHECK(std::fabs(rig.motor.i_d) < .05f, "current mode: i_d %g", rig.motor.i_d);
        CHECK(rig.motor.velocity > 10, "current mode: positive i_q turns the motor forward, %g rad/s", rig.motor.velocity);
        CHECK(std::fabs(rig.status.fast_loop.controller.i_q - rig.motor.i_q) < .05f, "current mode: measured i_q %g", rig.status.fast_loop.controller.i_q);
    }

    // voltage mode: v_q passes through. The inverse transform is the
    // transpose of the forward one, as in motorlib, so 2/3 of v_q reaches
    // the motor's q axis: steady state i_q = (2/3 v_q - emf) / R
    {
        Rig rig;
        HostCommand::command = {};
        HostCommand::command.mode_desired = obot::VOLTAGE;
        HostCommand::command.voltage.voltage_desired = .25f;
        rig.run(.004f);
        CHECK(std::fabs(rig.status.fast_loop.controller.v_q - .25f) < 1e-4f, "voltage mode: v_q %g", rig.status.fast_loop.controller.v_q);
        const float expected = ((2.f / 3) * .25f - Motor::kt * rig.motor.velocity) / Motor::R;
        CHECK(std::fabs(rig.motor.i_q - expected) < .02f, "voltage mode: i_q %g, expected %g", rig.motor.i_q, expected);
    }

    // phase lock: i_d at electrical angle 0 holds the rotor still. In this
    // model, where positive i_q is forward torque, that is half an electrical
    // revolution from the angle's zero: the half revolution that
    // current_direction = 1 adds on the tmotor.
    {
        Rig rig;
        rig.motor.position = .1f;
        HostCommand::command = {};
        HostCommand::command.mode_desired = obot::PHASE_LOCK;
        HostCommand::command.current_desired = 2;
        rig.run(1);
        const float electrical = std::remainder(rig.motor.position * num_poles, 2 * 3.14159265f);
        CHECK(std::fabs(std::fabs(electrical) - 3.14159265f) < .1f && std::fabs(rig.motor.velocity) < 1,    // lightly damped: still settling
            "phase lock: rotor at electrical angle %g, %g rad/s", electrical, rig.motor.velocity);
        CHECK(std::fabs(rig.status.fast_loop.controller.i_d - 2) < .1f, "phase lock: i_d %g for 2 A at angle 0", rig.status.fast_loop.controller.i_d);
    }

    // position mode: settles on the target
    {
        Rig rig;
        HostCommand::command = {};
        HostCommand::command.mode_desired = obot::POSITION;
        HostCommand::command.position_desired = 1;
        rig.run(1);
        CHECK(std::fabs(rig.motor.position - 1) < .02f, "position mode: %g for 1 rad", rig.motor.position);
        CHECK(std::fabs(rig.motor.velocity) < .1f, "position mode: at rest, %g rad/s", rig.motor.velocity);
    }

    // velocity mode
    {
        Rig rig;
        HostCommand::command = {};
        HostCommand::command.mode_desired = obot::VELOCITY;
        HostCommand::command.velocity_desired = 20;
        rig.run(1);
        CHECK(std::fabs(rig.motor.velocity - 20) < .5f, "velocity mode: %g for 20 rad/s", rig.motor.velocity);
    }

    // a command with a non finite value is dropped, the previous one stays
    {
        Rig rig;
        HostCommand::command = {};
        HostCommand::command.mode_desired = obot::CURRENT;
        HostCommand::command.current_desired = .5f;
        rig.run(.02f);
        HostCommand::command.current_desired = NAN;
        rig.run(.02f);
        CHECK(std::fabs(rig.motor.i_q - .5f) < .05f && rig.policy.invalid_command_count > 0, "invalid command dropped: i_q %g", rig.motor.i_q);
    }

    // back to open from a driven mode
    {
        Rig rig;
        HostCommand::command = {};
        HostCommand::command.mode_desired = obot::CURRENT;
        HostCommand::command.current_desired = 1;
        rig.run(.01f);
        HostCommand::command.mode_desired = obot::OPEN;
        rig.run(.01f);
        CHECK(rig.status.fast_loop.controller.mode == FOCMode::OPEN, "current to open");
        CHECK(rig.policy.status.read().flags.mode == obot::OPEN, "host status mode");
    }

    std::printf("%d failed\n", failures);
    return failures != 0;
}
