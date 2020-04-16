#include "../motorlib/usb_communication.h"
#include "st_device.h"
#include "../motorlib/system.h"
#include "../motorlib/qep_encoder.h"
#include "../motorlib/peripheral/stm32f4/pwm_en.h"
#include "../motorlib/hall.h"
#include "../motorlib/fast_loop.h"
#include "../motorlib/main_loop.h"
#include "../motorlib/led.h"
#include "../motorlib/peripheral/stm32f4/otg_usb.h"
#include "../motorlib/actuator.h"
#include "Inc/main.h"
#include "../motorlib/ma732_encoder.h"
#include "../motorlib/phony_encoder.h"
#include "../motorlib/motor_torque_sensor.h"

typedef FastLoop<PWM_EN, PhonyEncoder> FastLoopConfig;
typedef MainLoop<FastLoopConfig> MainLoopConfig;
typedef Actuator<FastLoopConfig, MainLoopConfig> ActuatorConfig;
typedef System<ActuatorConfig, USB_OTG> SystemConfig;

template<>
USB_OTG SystemConfig ::usb_ = {};
template<>
std::queue<std::string> SystemConfig ::log_queue_ = {};

static struct {
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
    FastLoopConfig fast_loop = {pwm_frequency, motor_pwm, motor_encoder};
    LED led = {const_cast<uint16_t*>(reinterpret_cast<volatile uint16_t *>(&TIM3->CCR1)), 
               const_cast<uint16_t*>(reinterpret_cast<volatile uint16_t *>(&TIM3->CCR2)),
               const_cast<uint16_t*>(reinterpret_cast<volatile uint16_t *>(&TIM3->CCR4)), .1};
    PIDDeadbandController controller = {(float) (1.0/main_loop_frequency)};
    USBCommunication<USB_OTG> communication = {SystemConfig::usb_};
    PIDController torque_controller = {(float) (1.0/main_loop_frequency)};
    PIDDeadbandController impedance_controller = {(float) (1.0/main_loop_frequency)};
    MainLoopConfig main_loop = {fast_loop, controller, torque_controller, impedance_controller, communication, led, output_encoder, torque_sensor};
} config_items;

template<>
ActuatorConfig SystemConfig::actuator_ = {config_items.fast_loop, config_items.main_loop};

void system_init() {
    // if (config_items.motor_encoder.init()) {
    //     SystemConfig::log("Motor encoder init success");
    // } else {
    //     SystemConfig::log("Motor encoder init failure");
    // }
    SystemConfig::init();
}

#include "../motorlib/system.cpp"
