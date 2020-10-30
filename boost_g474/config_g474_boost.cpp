#include "../motorlib/usb_communication.h"
#include "st_device.h"
#include "../motorlib/system.h"
#include "../motorlib/qep_encoder.h"
#include "../motorlib/peripheral/stm32g4/hrpwm.h"
#include "../motorlib/peripheral/spi_encoder.h"
#include "../motorlib/peripheral/stm32g4/ams_encoder.h"
#include "../motorlib/hall.h"
#include "../motorlib/fast_loop.h"
#include "../motorlib/main_loop.h"
#include "../motorlib/led.h"
#include "../motorlib/peripheral/usb.h"
#include "../motorlib/actuator.h"
#include "../motorlib/ma732_encoder.h"
#include "../motorlib/peripheral/stm32g4/spi_torque.h"
#include "Inc/main.h"
#include "param_g474_boost.h"
#include "../motorlib/motor_torque_sensor.h"
#include <functional>
#include "../motorlib/peripheral/stm32g4/spi_debug.h"

typedef FastLoop<HRPWM, QEPEncoder> FastLoopConfig;
typedef MainLoop<FastLoopConfig> MainLoopConfig;
typedef Actuator<FastLoopConfig, MainLoopConfig> ActuatorConfig;
typedef System<ActuatorConfig, USB1> SystemConfig;

template<>
USB1 SystemConfig ::usb_ = {};
template<>
std::queue<std::string> SystemConfig ::log_queue_ = {};
template<>
ParameterAPI SystemConfig ::api = {};

static struct {
    SystemInitClass system_init;
    uint32_t pwm_frequency = (double) CPU_FREQUENCY_HZ * 32.0 / (hrperiod);
    uint32_t main_loop_frequency = (double) CPU_FREQUENCY_HZ/(main_loop_period);
    QEPEncoder motor_encoder = {*TIM5};
    GPIO motor_encoder_cs = {*GPIOA, 15, GPIO::OUTPUT};
    
    //GPIO torque_cs = {*GPIOA, 15, GPIO::OUTPUT};
    //SPITorque torque_sensor = {*SPI3, torque_cs, *DMA1_Channel1, *DMA1_Channel2};
    MotorTorqueSensor torque_sensor;
    GPIO hall_a = {*GPIOC, 0, GPIO::INPUT};
    GPIO hall_b = {*GPIOC, 1, GPIO::INPUT};
    GPIO hall_c = {*GPIOC, 2, GPIO::INPUT};
   // MA732Encoder motor_encoder = {*SPI1, hall_a};
    HallEncoder output_encoder = {hall_a, hall_b, hall_c};
    //AMSEncoder motor_encoder = {*SPI3, motor_encoder_cs};
    GPIO enable = {*GPIOC, 11, GPIO::OUTPUT};
    HRPWM motor_pwm = {pwm_frequency, *HRTIM1, 3, 5, 4};
    SPIDebug spi_debug = {*SPI3, motor_encoder_cs, *DMA1_Channel1, *DMA1_Channel2};
    FastLoopConfig fast_loop = {(int32_t) pwm_frequency, motor_pwm, motor_encoder, param->fast_loop_param};
    LED led = {const_cast<uint16_t*>(reinterpret_cast<volatile uint16_t *>(&TIM4->CCR1)), 
               const_cast<uint16_t*>(reinterpret_cast<volatile uint16_t *>(&TIM4->CCR2)),
               const_cast<uint16_t*>(reinterpret_cast<volatile uint16_t *>(&TIM4->CCR3))};
    PIDDeadbandController controller = {(float) (1.0/main_loop_frequency)};
    PIDController torque_controller = {(float) (1.0/main_loop_frequency)};
    PIDDeadbandController impedance_controller = {(float) (1.0/main_loop_frequency)};
    USBCommunication<USB1> communication = {SystemConfig::usb_};
    MainLoopConfig main_loop = {fast_loop, controller, torque_controller, impedance_controller, communication, led, output_encoder, torque_sensor, param->main_loop_param};
} config_items;

template<>
ActuatorConfig SystemConfig::actuator_ = {config_items.fast_loop, config_items.main_loop, param->startup_param};

std::string spi_reg_val;
void set_spi_debug(std::string val) { spi_reg_val = val; }
//std::string get_spi_debug() { return spi_reg_val; }
std::string get_spi_debug() { return config_items.spi_debug.read(spi_reg_val); }

void system_init() {
    // if (config_items.motor_encoder.init()) {
    //     SystemConfig::log("Motor encoder init success");
    // } else {
    //     SystemConfig::log("Motor encoder init failure");
    // }
   // config_items.torque_sensor.init();
    // std::function<void(uint32_t)> setbct = std::bind(&MA732Encoder::set_bct, &config_items.motor_encoder, std::placeholders::_1);
    // std::function<uint32_t(void)> getbct = std::bind(&MA732Encoder::get_bct, &config_items.motor_encoder);
    // SystemConfig::api.add_api_variable("mbct", new APICallbackUint32(getbct, setbct));

    // std::function<void(uint32_t)> set_et = std::bind(&MA732Encoder::set_et, &config_items.motor_encoder, std::placeholders::_1);
    // std::function<uint32_t(void)> get_et = std::bind(&MA732Encoder::get_et, &config_items.motor_encoder);
    // SystemConfig::api.add_api_variable("met", new APICallbackUint32(get_et, set_et));

    // std::function<void(uint32_t)> set_mgt = std::bind(&MA732Encoder::set_mgt, &config_items.motor_encoder, std::placeholders::_1);
    // std::function<uint32_t(void)> get_mgt = std::bind(&MA732Encoder::get_magnetic_field_strength, &config_items.motor_encoder);
    // SystemConfig::api.add_api_variable("mmgt", new APICallbackUint32(get_mgt, set_mgt));

    config_items.spi_debug.init();
    SystemConfig::api.add_api_variable("spi", new APICallback<std::string>(get_spi_debug, set_spi_debug) );
    config_items.motor_pwm.init();
}

void system_maintenance() {}

#include "../motorlib/system.cpp"
