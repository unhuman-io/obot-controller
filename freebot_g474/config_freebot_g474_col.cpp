#include "param_freebot_g474_col.h"
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
#include "../motorlib/phony_encoder.h"
#include "Inc/main.h"
#include "../motorlib/sensor_multiplex.h"
#include <functional>

//typedef SensorMultiplex<PhonyEncoder, PhonyEncoder> EncoderConfig;
typedef SensorMultiplex<MA732Encoder, MA732Encoder> EncoderConfig;
typedef FastLoop<HRPWM, EncoderConfig> FastLoopConfig;
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
    SystemInitClass system_init; // first item to enable clocks, etc.
    uint32_t pwm_frequency = (double) CPU_FREQUENCY_HZ * 32.0 / (hrperiod);
    uint32_t main_loop_frequency = (double) CPU_FREQUENCY_HZ/(main_loop_period);
    GPIO motor_encoder_cs = {*GPIOB, 4, GPIO::OUTPUT};
    volatile int spi3_register_operation = 0;
    MA732Encoder motor_encoder = {*SPI3, motor_encoder_cs, 102, &spi3_register_operation};
    //PhonyEncoder motor_encoder = {700};
    GPIO torque_cs = {*GPIOA, 4, GPIO::OUTPUT};
    SPITorque torque_sensor = {*SPI1, torque_cs, *DMA1_Channel1, *DMA1_Channel2};
    GPIO output_encoder_cs = {*GPIOD, 2, GPIO::OUTPUT};
    MA732Encoder output_encoder = {*SPI3, output_encoder_cs, 153, &spi3_register_operation}; // need to make sure this doesn't collide with motor encoder
    //PhonyEncoder output_encoder = {100};
    //GPIO enable = {*GPIOC, 11, GPIO::OUTPUT};
    HRPWM motor_pwm = {pwm_frequency, *HRTIM1, 4, 5, 3, false};
    EncoderConfig encoders = {motor_encoder, output_encoder};
    FastLoopConfig fast_loop = {(int32_t) pwm_frequency, motor_pwm, encoders, param->fast_loop_param};
    LED led = {const_cast<uint16_t*>(reinterpret_cast<volatile uint16_t *>(&TIM4->CCR1)), 
               const_cast<uint16_t*>(reinterpret_cast<volatile uint16_t *>(&TIM4->CCR3)),
               const_cast<uint16_t*>(reinterpret_cast<volatile uint16_t *>(&TIM4->CCR2))};
    //PIDInterpolateController controller = {(float) (1.0/main_loop_frequency), 10};
    PIDController controller = {(float) (1.0/main_loop_frequency)};
    PIDController torque_controller = {(float) (1.0/main_loop_frequency)};
    PIDDeadbandController impedance_controller = {(float) (1.0/main_loop_frequency)};
    USBCommunication<USB1> communication = {SystemConfig::usb_};
    MainLoopConfig main_loop = {fast_loop, controller, torque_controller, impedance_controller, communication, led, encoders.secondary(), torque_sensor, param->main_loop_param};
} config_items;

template<>
ActuatorConfig SystemConfig::actuator_ = {config_items.fast_loop, config_items.main_loop, param->startup_param};

void system_init() {
    if (config_items.motor_encoder.init()) {
        SystemConfig::log("Motor encoder init success");
    } else {
        SystemConfig::log("Motor encoder init failure");
    }
    if (config_items.output_encoder.init()) {
        SystemConfig::log("Output encoder init success");
    } else {
        SystemConfig::log("Output encoder init failure");
    }

    // TODO I don't know if these std::functions persist after this function, but they seem to work.
    std::function<void(uint32_t)> setbct = std::bind(&MA732Encoder::set_bct, &config_items.motor_encoder, std::placeholders::_1);
    std::function<uint32_t(void)> getbct = std::bind(&MA732Encoder::get_bct, &config_items.motor_encoder);
    SystemConfig::api.add_api_variable("mbct", new APICallbackUint32(getbct, setbct));

    std::function<void(uint32_t)> set_et = std::bind(&MA732Encoder::set_et, &config_items.motor_encoder, std::placeholders::_1);
    std::function<uint32_t(void)> get_et = std::bind(&MA732Encoder::get_et, &config_items.motor_encoder);
    SystemConfig::api.add_api_variable("met", new APICallbackUint32(get_et, set_et));

    std::function<void(uint32_t)> set_mgt = std::bind(&MA732Encoder::set_mgt, &config_items.motor_encoder, std::placeholders::_1);
    std::function<uint32_t(void)> get_mgt = std::bind(&MA732Encoder::get_magnetic_field_strength, &config_items.motor_encoder);
    SystemConfig::api.add_api_variable("mmgt", new APICallbackUint32(get_mgt, set_mgt));

    std::function<void(uint32_t)> setbctj = std::bind(&MA732Encoder::set_bct, &config_items.output_encoder, std::placeholders::_1);
    std::function<uint32_t(void)> getbctj = std::bind(&MA732Encoder::get_bct, &config_items.output_encoder);
    SystemConfig::api.add_api_variable("jbct", new APICallbackUint32(getbctj, setbctj));

    std::function<void(uint32_t)> set_etj = std::bind(&MA732Encoder::set_et, &config_items.output_encoder, std::placeholders::_1);
    std::function<uint32_t(void)> get_etj = std::bind(&MA732Encoder::get_et, &config_items.output_encoder);
    SystemConfig::api.add_api_variable("jet", new APICallbackUint32(get_etj, set_etj));

    std::function<void(uint32_t)> set_mgtj = std::bind(&MA732Encoder::set_mgt, &config_items.output_encoder, std::placeholders::_1);
    std::function<uint32_t(void)> get_mgtj = std::bind(&MA732Encoder::get_magnetic_field_strength, &config_items.output_encoder);
    SystemConfig::api.add_api_variable("jmgt", new APICallbackUint32(get_mgtj, set_mgtj));

    SystemConfig::api.add_api_variable("c1",new APIUint32(&config_items.torque_sensor.result0_));
    SystemConfig::api.add_api_variable("c2",new APIUint32(&config_items.torque_sensor.result1_));
    SystemConfig::actuator_.main_loop_.reserved1_ = &config_items.torque_sensor.result0_;
    SystemConfig::actuator_.main_loop_.reserved2_ = &config_items.torque_sensor.result1_;
    config_items.torque_sensor.init();
    config_items.motor_pwm.init();
}

#include "../motorlib/system.cpp"
