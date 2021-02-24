#include "../motorlib/peripheral/stm32f4/otg_usb.h"
typedef USB_OTG USB1;
#include "../motorlib/usb_communication.h"
#include "st_device.h"
#include "../motorlib/qep_encoder.h"
#include "../motorlib/peripheral/stm32f4/pwm_en.h"
#include "../motorlib/hall.h"
#include "../motorlib/led.h"
#include "Inc/main.h"
#include "../motorlib/ma732_encoder.h"
#include "../motorlib/phony_encoder.h"
#include "../motorlib/motor_torque_sensor.h"
#include "param_fabulab_f446.h"

#include "../motorlib/controller/position_controller.h"
#include "../motorlib/controller/torque_controller.h"
#include "../motorlib/controller/impedance_controller.h"
#include "../motorlib/controller/velocity_controller.h"
typedef MotorTorqueSensor TorqueSensor;
typedef PWM_EN PWM;
typedef EncoderBase OutputEncoder;
typedef PhonyEncoder MotorEncoder;
typedef USBCommunication Communication;
#include "../motorlib/fast_loop.h"
#include "../motorlib/main_loop.h"
#include "../motorlib/actuator.h"
#include "../motorlib/system.h"

USB_OTG usb;
Communication System::communication_ = {usb};
std::queue<std::string> System::log_queue_ = {};
ParameterAPI System::api = {};

#define I_A_DR  ADC3->JDR1
#define I_B_DR  ADC2->JDR1
#define I_C_DR  ADC1->JDR1
#define V_BUS_DR ADC1->DR

void usb_interrupt() {
    usb.interrupt();
}

volatile uint32_t * const cpu_clock = &DWT->CYCCNT;
static struct {
    SystemInitClass system_init;
    int32_t pwm_frequency = (double) CPU_FREQUENCY_HZ / (pwm_period);
    uint32_t main_loop_frequency = (double) CPU_FREQUENCY_HZ/(main_period);
    GPIO enable = {*GPIOC, 14, GPIO::OUTPUT};
    PhonyEncoder motor_encoder = {100};
    QEPEncoder output_encoder = {*TIM2};
    GPIO spi1_cs = {*GPIOA, 15, GPIO::OUTPUT};
    //MA732Encoder motor_encoder = {*SPI1, spi1_cs};
    MotorTorqueSensor torque_sensor;    // TODO implement for F446
    PWM_EN motor_pwm = {pwm_frequency, *const_cast<uint32_t*>(&TIM8->CCR3), 
                        *const_cast<uint32_t*>(&TIM8->CCR2), 
                        *const_cast<uint32_t*>(&TIM8->CCR1),
                        *TIM8, enable};
    FastLoop fast_loop = {pwm_frequency, motor_pwm, motor_encoder, param->fast_loop_param, &I_A_DR, &I_B_DR, &I_C_DR, &V_BUS_DR};
    LED led = {const_cast<uint16_t*>(reinterpret_cast<volatile uint16_t *>(&TIM3->CCR1)), 
               const_cast<uint16_t*>(reinterpret_cast<volatile uint16_t *>(&TIM3->CCR2)),
               const_cast<uint16_t*>(reinterpret_cast<volatile uint16_t *>(&TIM3->CCR4)), .1};
    PositionController position_controller = {(float) (1.0/main_loop_frequency)};
    TorqueController torque_controller = {(float) (1.0/main_loop_frequency)};
    ImpedanceController impedance_controller = {(float) (1.0/main_loop_frequency)};
    VelocityController velocity_controller = {(float) (1.0/main_loop_frequency)};
    MainLoop main_loop = {fast_loop, position_controller, torque_controller, impedance_controller, velocity_controller, System::communication_, led, output_encoder, torque_sensor, param->main_loop_param};
} config_items;

Actuator System::actuator_ = {config_items.fast_loop, config_items.main_loop, param->startup_param};

void system_init() {
    // if (config_items.motor_encoder.init()) {
    //     SystemConfig::log("Motor encoder init success");
    // } else {
    //     SystemConfig::log("Motor encoder init failure");
    // }
}

void system_maintenance() {}

#include "../motorlib/system.cpp"
