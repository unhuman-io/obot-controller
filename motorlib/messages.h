
#ifndef MOTOR_MESSAGES_H
#define MOTOR_MESSAGES_H

#include <stdint.h>

typedef uint32_t mcu_time;  // a timestamp in cpu cycles

typedef struct {
    float kp;               // proportional gain, units of V/A for current control
    float ki;               // integral gain, same units as proportional gain but per cycle, note this may change to per second
    float ki_limit;         // integrator saturation, same units as output saturation
    float command_max;      // Output saturation, units of V for current control, A for position control
} PIParam;

typedef struct {
    float kp;               // proportional gain, units of A/rad for position control
    float ki;               // \sa PIParam.ki 
    float ki_limit;         // \sa PIParam.ki_limit
    float kd;               // derivative gain, implemented on error units same as kp * seconds
    float command_max;      // \sa PIParam.command_max
    float velocity_filter_frequency_hz; // First order filter on velocity feedback
} PIDParam;

typedef struct {
    PIParam pi_d;           // PIParam for d axis current - often make the same as pi_q
    PIParam pi_q;           // PIParam for q axis current
    float current_filter_frequency_hz;  // First order filter on current measurements
    float num_poles;        // number of motor pole pairs - i.e. number of motor magnets/2
} FOCParam;

#define COGGING_TABLE_SIZE 8192  // must be multiple of 2
typedef struct {
    int32_t pwm_frequency;  // PWM frequency in Hz, default usually 100000
    float adc1_offset, adc2_offset, adc3_offset;    // initial guess at current sensor bias in counts - default 2048
    float adc1_gain, adc2_gain, adc3_gain;          // current sensor linear gain units A/count
    FOCParam foc_param;
    uint8_t phase_mode;     // two possible motor wiring states: 0: standard, 1: reverse (i.e. two motor leads flipped)
    struct {
        float index_electrical_offset_pos;          // index offset electrial zero in encoder counts
                                                    // can obtain this value from  motor_index_pos_ - motor_electrical_zero_pos_ 
                                                    // in fast_loop if use_index_electrical_offset_pos == 0 
        uint8_t use_index_electrical_offset_pos;    // Set to 1 to enable using the index_electrical_offset_pos above, 0 to disable
                                                    // Allows for more repeatable commutation from a quadrature encoder with index or absolute encoder
        uint32_t cpr;                               // Counts/revolution for encoder, for quadrature encoders 4x lines per revolution
        float dir;                                  // Set to 1 for positive output, -1 for negative
    } motor_encoder;
    struct {
        float table[COGGING_TABLE_SIZE];            // cogging table in A
        float gain;                                 // cogging table multiplier - 0 to disable, 1 for 1:1 ratio
    } cogging;
    float vbus_gain;                                // vbus sensor gain units V/count
} FastLoopParam;

enum MainControlMode {OPEN, DAMPED, CURRENT, POSITION, VELOCITY, CURRENT_TUNING, POSITION_TUNING, BOARD_RESET=255};
typedef struct {
    int32_t update_frequency;                       // main loop update frequency in Hz
    PIDParam controller_param;
    struct {
        float cpr;                                  // output encoder cpr \sa FastLoopParam.motor_encoder.cpr
    } output_encoder;
    float torque_gain, torque_bias;                 // not currently used
    float kt;                                       // not currently used
    float gear_ratio;                               // not currently used
} MainLoopParam;

typedef struct {
    uint8_t do_phase_lock;          // 1: yes, 0: no
    float phase_lock_current;       // current in A
    float phase_lock_duration;      // duration in seconds
    enum MainControlMode startup_mode;
} StartupParam;

typedef struct {
    FastLoopParam fast_loop_param;
    MainLoopParam main_loop_param;
    StartupParam startup_param;
    char name[64];                 // name available through usb string
} Param;

typedef struct {
    struct { float i_d, i_q; } desired;         // desired current in A, i_d typically 0, i_q creates torque
    struct { float i_a, i_b, i_c, motor_encoder; } measured;    // sensor currents in A, motor_encoder in rad referenced to electrical zero
} FOCCommand;

typedef struct {
    struct {
        float i_d, i_q;                         // \sa FOCCommand.desired
    } desired;
    struct {
        float position;                         // motor electrical position
        float i_d, i_q, i_0;                    // measured processed currents, unfiltered
    } measured;
    struct { float v_a, v_b, v_c, v_d, v_q; } command;  // command in V to PWM
} FOCStatus;

typedef struct {
    mcu_time timestamp;                 // timestamp in microcontroller clock cycles
    FOCStatus foc_status;
    struct {
        int32_t raw;                    // raw counts since startup
        float position;                 // position in radians, 0 at startup or absolute value
        float velocity;                 // velocity in rad/s some filter        
    } motor_position;
    float motor_mechanical_position;    // counts referenced to index
    FOCCommand foc_command;
    float t_seconds, dt;                // time since startup in seconds, will lose resolution, dt is measured time in seconds
    float vbus;                         // bus voltage V
} FastLoopStatus;

typedef struct {} MainLoopStatus;       // not currently used

typedef struct {
    mcu_time mcu_timestamp;             // timestamp in microcontroller clock cycles
    uint32_t host_timestamp_received;   // return of host_timestamp from ReceiveData
    float motor_position;               // motor position in radians
    float joint_position;               // joint position in radians
    float iq;                           // Measured motor current in A line-line
    int32_t motor_encoder;              // motor position in raw counts
    float reserved[2];
} SendData;

typedef struct {
    uint32_t host_timestamp;            // Value from host
    uint8_t mode_desired;               // 0: open, 1: damped, 2: active
    float current_desired;              // motor current desired in A line-line
    float position_desired;             // motor position desired in rad
    float velocity_desired;             // motor velocity desired in rad/s
    float reserved;                     // no position control for values < abs(position_deadband - position_desired)
} ReceiveData;

#endif