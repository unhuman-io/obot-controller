#include "../../motorlib/peripheral/usb.h"
#include "../../motorlib/usb_communication.h"
#include "../../motorlib/peripheral/stm32g4/hrpwm.h"
#include "../../motorlib/util.h"

using PWM = HRPWM;
using Communication = USBCommunication;
volatile uint32_t * const cpu_clock = &DWT->CYCCNT;
uint16_t drv_regs_error = 0;  

#include "../../motorlib/led.h"
#include "../../motorlib/controller/position_controller.h"
#include "../../motorlib/controller/torque_controller.h"
#include "../../motorlib/controller/impedance_controller.h"
#include "../../motorlib/controller/velocity_controller.h"
#include "../../motorlib/fast_loop.h"
#include "../../motorlib/main_loop.h"
#include "../../motorlib/actuator.h"
#include "../../motorlib/system.h"
#include "pin_config_freebot_g474_motor.h"
#include "../../motorlib/peripheral/stm32g4/temp_sensor.h"

namespace config {
    static_assert(((double) CPU_FREQUENCY_HZ * 32 / 2) / pwm_frequency < 65535);    // check pwm frequency
    TempSensor temp_sensor;
    HRPWM motor_pwm = {pwm_frequency, *HRTIM1, 3, 5, 4, false, 200, 1000, 0};
    USB1 usb;
    FastLoop fast_loop = {(int32_t) pwm_frequency, motor_pwm, motor_encoder, param->fast_loop_param, &I_A_DR, &I_B_DR, &I_C_DR, &V_BUS_DR};
    LED led = {const_cast<uint16_t*>(reinterpret_cast<volatile uint16_t *>(&TIM_R)), 
               const_cast<uint16_t*>(reinterpret_cast<volatile uint16_t *>(&TIM_G)),
               const_cast<uint16_t*>(reinterpret_cast<volatile uint16_t *>(&TIM_B))};
    PositionController position_controller = {(float) (1.0/main_loop_frequency)};
    TorqueController torque_controller = {(float) (1.0/main_loop_frequency)};
    ImpedanceController impedance_controller = {(float) (1.0/main_loop_frequency)};
    VelocityController velocity_controller = {(float) (1.0/main_loop_frequency)};
    MainLoop main_loop = {fast_loop, position_controller, torque_controller, impedance_controller, velocity_controller, System::communication_, led, output_encoder, torque_sensor, param->main_loop_param};
};

Communication System::communication_ = {config::usb};
void usb_interrupt() {
    config::usb.interrupt();
}
Actuator System::actuator_ = {config::fast_loop, config::main_loop, param->startup_param};

float v3v3 = 3.3;

int32_t index_mod = 0;

void config_init();

void system_init() {
    if (config::motor_encoder.init()) {
        System::log("Motor encoder init success");
    } else {
        System::log("Motor encoder init failure");
    }
    if (config::output_encoder.init()) {
        System::log("Output encoder init success");
    } else {
        System::log("Output encoder init failure");
    }
    if (drv_regs_error) {
        System::log("drv configure failure");
    } else {
        System::log("drv configure success");
    }
    config::torque_sensor.init();
}


#include "../motorlib/system.cpp"

void setup_sleep() {} //todo
void finish_sleep() {}
