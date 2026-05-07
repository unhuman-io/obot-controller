#include "../param/param_obot_g474_can.h"
#include "st_device.h"
#include "../../motorlib/encoder.h"
#include "../../motorlib/torque_sensor.h"
#include <coroutine>
#include "task.h"

#define COMMS   COMMS_CAN

using TorqueSensor = TorqueSensorBase;
using MotorEncoder = EncoderBase;
using OutputEncoder = EncoderBase;

struct InitCode {
    InitCode() {}
};

namespace config {
    const uint32_t main_loop_frequency = 10000;    
    const uint32_t pwm_frequency = 30000;
    InitCode init_code;

    TorqueSensor torque_sensor;
    OutputEncoder output_encoder;
    MotorEncoder motor_encoder;
};

#include "../../motorlib/boards/config_obot_g474_trace.cpp"


Task init_sensor(CycleScheduler& sched) {
    co_await sched.async_delay_us(500);
    logger.log("sensor mid");
    co_await sched.async_delay_us(500);
    logger.log("sensor done");
}

Task init_adc(CycleScheduler& sched) {
    co_await sched.async_delay_us(1200); 
    logger.log("adc done");
}

void config_init() {
    CycleScheduler init_scheduler;

    Task sensor_task = init_sensor(init_scheduler);
    Task adc_task = init_adc(init_scheduler);

    logger.log("init start");
    while (!sensor_task.is_done() || !adc_task.is_done()) {
        init_scheduler.poll();
    }
    logger.log("init done");
}

void config_maintenance() {}

