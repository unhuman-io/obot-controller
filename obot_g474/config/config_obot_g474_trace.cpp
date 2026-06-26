#include "../param/param_obot_g474.h"
#include "st_device.h"
#include "../../motorlib/encoder.h"
#include "../../motorlib/torque_sensor.h"
#include "../../motorlib/gpio.h"
#include <algorithm>
#include "../../motorlib/peripheral/stm32g4/pin_config.h"
#include "../../motorlib/boards/param_obot_g474_trace.h"
#include "../../motorlib/boards/config_obot_g474_trace_board.h"

struct InitCode {
    InitCode() {
        // trace pins enable
        RCC->AHB2ENR |= RCC_AHB2ENR_GPIOEEN;
        GPIO_SETL(E, 2, GPIO_MODE::ALT_FUN, GPIO_SPEED::VERY_HIGH, 0);
        GPIO_SETL(E, 3, GPIO_MODE::ALT_FUN, GPIO_SPEED::VERY_HIGH, 0);
        GPIO_SETL(E, 4, GPIO_MODE::ALT_FUN, GPIO_SPEED::VERY_HIGH, 0);
        GPIO_SETL(E, 5, GPIO_MODE::ALT_FUN, GPIO_SPEED::VERY_HIGH, 0);
        GPIO_SETL(E, 6, GPIO_MODE::ALT_FUN, GPIO_SPEED::VERY_HIGH, 0);
        //ETM->CR |= ETM_CR_ETMEN;
        uint32_t *etmcr = (uint32_t *)0xE0041000;
        *etmcr |= 1 << 11;
        uint32_t *etmteevr = (uint32_t *)0xE0041020;
        *etmteevr = 0x000037ef; // ON
    }
};

InitCode init_code;

struct Config {
    static constexpr uint32_t main_loop_frequency = 10000;    
    static constexpr uint32_t pwm_frequency = 50000;
    static constexpr uint32_t system_loop_frequency =  1000;
    using TorqueSensor = TorqueSensorBase;
    using MotorEncoder = EncoderBase;
    using OutputEncoder = EncoderBase;
    MotorEncoder motor_encoder;
    TorqueSensor torque_sensor;
    OutputEncoder output_encoder;
};

struct TraceProject : TraceBoard<TraceConfig> {
} trace_project;

void config_init() {}

void config_maintenance() {}