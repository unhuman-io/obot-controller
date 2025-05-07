#include "../param/param_obot_g474.h"
#include "st_device.h"
#include "../../motorlib/encoder.h"
#include "../../motorlib/torque_sensor.h"
#include "../../motorlib/gpio.h"
#include <algorithm>
#include "../../motorlib/peripheral/stm32g4/pin_config.h"


using TorqueSensor = TorqueSensorBase;
using MotorEncoder = EncoderBase;
using OutputEncoder = EncoderBase;
#include "../../motorlib/logger.h"
import config_obot_g474_tracec;

class Trace2 : public TraceBoard {
public:
    Trace2() {
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

    const uint32_t main_loop_frequency = 10000;    
    const uint32_t pwm_frequency = 50000;

    MotorEncoder motor_encoder;
    TorqueSensor torque_sensor;
    OutputEncoder output_encoder;

};

Trace2 trace2;

Logger::CIndex log_index __attribute__((section(".noload")));
char log_queue[LOGGING_MAX_SIZE] __attribute__((section(".noload")));
__attribute__ ((init_priority(LOGGER_INIT_PRIORITY))) Logger logger(log_index, log_queue);
extern const char * const name = "obot_g474_trace2";

extern "C" void usb_interrupt() {
    trace2.usb.interrupt();
}
void config_init() {}

void config_maintenance() {}

extern "C" void board_init() {}