
#ifndef MOTOR_MESSAGES_H
#define MOTOR_MESSAGES_H

#include <stdint.h>

#define MAX_DATA_LENGTH 120 // bytes
typedef uint32_t mcu_time;

typedef struct {
    float kp;
    float ki;
    float ki_limit;
    float command_max;
} PIParam;

typedef struct {
    float kp;
    float ki;
    float ki_limit;
    float kd;
    float command_max;
    float velocity_filter_frequency_hz;
} PIDParam;

typedef struct {
    PIParam pi_d;
    PIParam pi_q;
    float current_filter_frequency_hz;
    float num_poles;
} FOCParam;

#define COGGING_TABLE_SIZE 8192//1024  // must be multiple of 2
typedef struct {
    int32_t pwm_frequency;
    float adc1_offset, adc2_offset, adc3_offset;
    float adc1_gain, adc2_gain, adc3_gain;
    FOCParam foc_param;
    uint8_t phase_mode;
    struct {
        float index_electrical_offset_pos;
        uint8_t use_index_electrical_offset_pos;
        uint32_t cpr;
        float dir;
    } motor_encoder;
    struct {
        float table[COGGING_TABLE_SIZE];
        float gain;
    } cogging;
    float vbus_gain;
} FastLoopParam;

enum MainControlMode {OPEN, DAMPED, CURRENT, POSITION, VELOCITY, CURRENT_TUNING, POSITION_TUNING, BOARD_RESET=255};
typedef struct {
    int32_t update_frequency;
    PIDParam controller_param;
    struct {
        float cpr;
    } output_encoder;
    float torque_gain, torque_bias;
    float kt;
    float gear_ratio;
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
    char name[64];
} Param;

typedef struct {
    struct { float i_d, i_q; } desired;
    struct { float i_a, i_b, i_c, motor_encoder; } measured;
} FOCCommand;

typedef struct {
    struct {
        float i_d, i_q;
    } desired;
    struct {
        float position;
        float i_d, i_q, i_0;
    } measured;
    struct { float v_a, v_b, v_c, v_d, v_q; } command;
} FOCStatus;



typedef struct {
    float position, velocity;
    float v_abc[3];
    FOCStatus foc_status;
} MotorStatus;

typedef struct {
    mcu_time timestamp;
    FOCStatus foc_status;
    struct {
        int32_t raw;
        float position;
        float velocity;
        int32_t index_pos;
    } motor_position;
    float motor_mechanical_position;
    FOCCommand foc_command;
    float t_seconds, dt;
    float vbus;
} FastLoopStatus;

typedef struct {
    float torque;
} MainLoopStatus;

typedef struct {
    uint16_t type;       ///< \sa CommandType
    uint8_t data[MAX_DATA_LENGTH - 2];
} MotorCommand;

enum MessageType {
    MOTOR_COMMAND = 1,
};

typedef struct {
    uint16_t length;
    uint16_t type;      ///< \sa MessageType
    uint8_t data[MAX_DATA_LENGTH];
} Message;


// Internal messages
typedef struct {
    int reserved;
} SystemUpdateCommand;

typedef struct {
    int reserved;
} SystemUpdateParam;

typedef struct {
    int reserved;
} SystemUpdateStatus;

typedef struct {
    int reserved;
} MainControlCommand;

typedef struct {
    int reserved;
} MainControlParam;

typedef struct {
    MotorStatus motor_status;
} MainControlStatus;

typedef struct {
    FOCCommand command;
} FOCControlCommand;

typedef struct {
    FOCParam param;
} FOCControlParam;

typedef struct {
    FOCStatus status;
} FOCControlStatus;

typedef struct {
    int reserved;
} SimulatorCommand;

typedef struct {
    int reserved;
} SimulatorParam;

typedef struct {
    MotorStatus motor_status;
} SimulatorStatus;

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
    float reserved;            // no position control for values < abs(position_deadband - position_desired)
} ReceiveData;

#endif