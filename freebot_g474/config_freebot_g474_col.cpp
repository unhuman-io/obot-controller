#include "param_freebot_g474_col.h"
#include "../motorlib/peripheral/usb.h"
#include "../motorlib/usb_communication.h"
#include "st_device.h"
#include "../motorlib/peripheral/stm32g4/hrpwm.h"

#include "../motorlib/led.h"
#include "../motorlib/ma732_encoder.h"
#include "../motorlib/peripheral/stm32g4/spi_torque.h"
#include "../motorlib/phony_encoder.h"
#include "Inc/main.h"
#include "../motorlib/sensor_multiplex.h"
#include <functional>
#include "../motorlib/peripheral/stm32g4/max31875.h"

#include "../motorlib/controller/position_controller.h"
#include "../motorlib/controller/torque_controller.h"
#include "../motorlib/controller/impedance_controller.h"
#include "../motorlib/controller/velocity_controller.h"
typedef SPITorque TorqueSensor;
typedef HRPWM PWM;
typedef SensorMultiplex<MA732Encoder, MA732Encoder> MotorEncoder;
typedef MotorEncoder::SecondarySensor OutputEncoder;
typedef USBCommunication Communication;
#include "../motorlib/fast_loop.h"
#include "../motorlib/main_loop.h"
#include "../motorlib/actuator.h"
#include "../motorlib/system.h"

USB1 usb1;
Communication System::communication_ = {usb1};
std::queue<std::string> System::log_queue_ = {};
ParameterAPI System::api = {};

#define I_A_DR  ADC5->JDR1
#define I_B_DR  ADC4->JDR1
#define I_C_DR  ADC3->JDR1
#define V_BUS_DR ADC1->DR
#define V_A_DR  ADC1->JDR1
#define V_B_DR  ADC1->JDR2
#define V_C_DR  ADC2->JDR3

void usb_interrupt() {
    usb1.interrupt();
}

volatile uint32_t * const cpu_clock = &DWT->CYCCNT;
static struct {
    SystemInitClass system_init; // first item to enable clocks, etc.
    uint32_t pwm_frequency = (double) CPU_FREQUENCY_HZ * 32.0 / (hrperiod);
    uint32_t main_loop_frequency = (double) CPU_FREQUENCY_HZ/(main_loop_period);
    GPIO motor_encoder_cs = {*GPIOB, 4, GPIO::OUTPUT};
    volatile int spi3_register_operation = 0;
    MA732Encoder motor_encoder = {*SPI3, motor_encoder_cs, 102, &spi3_register_operation};
    //PhonyEncoder motor_encoder = {700};
    GPIO torque_cs = {*GPIOA, 4, GPIO::OUTPUT};
    SPITorque torque_sensor = {*SPI1, torque_cs, *DMA1_Channel1, *DMA1_Channel2, 4};
    GPIO output_encoder_cs = {*GPIOD, 2, GPIO::OUTPUT};
    MA732Encoder output_encoder = {*SPI3, output_encoder_cs, 153, &spi3_register_operation}; // need to make sure this doesn't collide with motor encoder
    //PhonyEncoder output_encoder = {100};
    //GPIO enable = {*GPIOC, 11, GPIO::OUTPUT};
    I2C i2c = {*I2C1};
    I2C i2c2 = {*I2C2};
    MAX31875 temp_sensor = {i2c, 0x48};        // R0
    MAX31875 temp_sensor1 = {i2c2, 0x48};      // R0
    MAX31875 temp_sensor2 = {i2c2, 0x49};      // R1
    MAX31875 temp_sensor3 = {i2c2, 0x4A};      // R2
    HRPWM motor_pwm = {pwm_frequency, *HRTIM1, 4, 5, 3, false};
    MotorEncoder encoders = {motor_encoder, output_encoder};
    FastLoop fast_loop = {(int32_t) pwm_frequency, motor_pwm, encoders, param->fast_loop_param, &I_A_DR, &I_B_DR, &I_C_DR, &V_BUS_DR};
    LED led = {const_cast<uint16_t*>(reinterpret_cast<volatile uint16_t *>(&TIM4->CCR1)), 
               const_cast<uint16_t*>(reinterpret_cast<volatile uint16_t *>(&TIM4->CCR3)),
               const_cast<uint16_t*>(reinterpret_cast<volatile uint16_t *>(&TIM4->CCR2))};
    PositionController position_controller = {(float) (1.0/main_loop_frequency)};
    TorqueController torque_controller = {(float) (1.0/main_loop_frequency)};
    ImpedanceController impedance_controller = {(float) (1.0/main_loop_frequency)};
    VelocityController velocity_controller = {(float) (1.0/main_loop_frequency)};
    MainLoop main_loop = {fast_loop, position_controller, torque_controller, impedance_controller, velocity_controller, System::communication_, led, encoders.secondary(), torque_sensor, param->main_loop_param};
} config_items;

Actuator System::actuator_ = {config_items.fast_loop, config_items.main_loop, param->startup_param};

float get_va() {
    ADC1->CR |= ADC_CR_JADSTART;
    ns_delay(1000);
    return V_A_DR * 3.0/4096*(18+2)/2.0;
}
float get_vb() {
    ADC1->CR |= ADC_CR_JADSTART;
    ns_delay(1000);
    return V_A_DR * 3.0/4096*(18+2)/2.0;
}
float get_vc() {
    ADC1->CR |= ADC_CR_JADSTART;
    ns_delay(1000);
    return V_A_DR * 3.0/4096*(18+2)/2.0;
}
void set_v(float f) {}

volatile float T1=0, T2=0, T3=0;

void system_init() {
    if (config_items.motor_encoder.init()) {
        System::log("Motor encoder init success");
    } else {
        System::log("Motor encoder init failure");
    }
    if (config_items.output_encoder.init()) {
        System::log("Output encoder init success");
    } else {
        System::log("Output encoder init failure");
    }

    // TODO I don't know if these std::functions persist after this function, but they seem to work.
    std::function<void(uint32_t)> setbct = std::bind(&MA732Encoder::set_bct, &config_items.motor_encoder, std::placeholders::_1);
    std::function<uint32_t(void)> getbct = std::bind(&MA732Encoder::get_bct, &config_items.motor_encoder);
    System::api.add_api_variable("mbct", new APICallbackUint32(getbct, setbct));

    std::function<void(uint32_t)> set_et = std::bind(&MA732Encoder::set_et, &config_items.motor_encoder, std::placeholders::_1);
    std::function<uint32_t(void)> get_et = std::bind(&MA732Encoder::get_et, &config_items.motor_encoder);
    System::api.add_api_variable("met", new APICallbackUint32(get_et, set_et));

    std::function<void(uint32_t)> set_mgt = std::bind(&MA732Encoder::set_mgt, &config_items.motor_encoder, std::placeholders::_1);
    std::function<uint32_t(void)> get_mgt = std::bind(&MA732Encoder::get_magnetic_field_strength, &config_items.motor_encoder);
    System::api.add_api_variable("mmgt", new APICallbackUint32(get_mgt, set_mgt));

    std::function<void(uint32_t)> setbctj = std::bind(&MA732Encoder::set_bct, &config_items.output_encoder, std::placeholders::_1);
    std::function<uint32_t(void)> getbctj = std::bind(&MA732Encoder::get_bct, &config_items.output_encoder);
    System::api.add_api_variable("jbct", new APICallbackUint32(getbctj, setbctj));

    std::function<void(uint32_t)> set_etj = std::bind(&MA732Encoder::set_et, &config_items.output_encoder, std::placeholders::_1);
    std::function<uint32_t(void)> get_etj = std::bind(&MA732Encoder::get_et, &config_items.output_encoder);
    System::api.add_api_variable("jet", new APICallbackUint32(get_etj, set_etj));

    std::function<void(uint32_t)> set_mgtj = std::bind(&MA732Encoder::set_mgt, &config_items.output_encoder, std::placeholders::_1);
    std::function<uint32_t(void)> get_mgtj = std::bind(&MA732Encoder::get_magnetic_field_strength, &config_items.output_encoder);
    System::api.add_api_variable("jmgt", new APICallbackUint32(get_mgtj, set_mgtj));

    System::api.add_api_variable("c1",new APIUint32(&config_items.torque_sensor.result0_));
    System::api.add_api_variable("c2",new APIUint32(&config_items.torque_sensor.result1_));

    std::function<void(uint32_t)> set_t_reset = std::bind(&SPITorque::reset, &config_items.torque_sensor, std::placeholders::_1);
    std::function<uint32_t(void)> get_t_reset = std::bind(&SPITorque::reset2, &config_items.torque_sensor);
    System::api.add_api_variable("t_reset", new APICallbackUint32(get_t_reset, set_t_reset));

    std::function<void(uint32_t)> set_temp = nullptr;
    std::function<uint32_t(void)> get_temp = std::bind(&MAX31875::read, &config_items.temp_sensor);
    System::api.add_api_variable("T", new APICallbackUint32(get_temp, set_temp));

    System::api.add_api_variable("T1", new APIFloat((float*) &T1));
    System::api.add_api_variable("T2", new APIFloat((float*) &T2));
    System::api.add_api_variable("T3", new APIFloat((float*) &T3));

    System::api.add_api_variable("vam", new APICallbackFloat(get_va, set_v));
    System::api.add_api_variable("vbm", new APICallbackFloat(get_vb, set_v));
    System::api.add_api_variable("vcm", new APICallbackFloat(get_vc, set_v));

    System::actuator_.main_loop_.reserved1_ = &config_items.temp_sensor.value_;// &config_items.torque_sensor.result0_;
    System::actuator_.main_loop_.reserved2_ = &config_items.torque_sensor.sum_;
    config_items.torque_sensor.init();
    config_items.motor_pwm.init();
}

FrequencyLimiter temp_rate = {8};

void system_maintenance() {
    if (temp_rate.run()) {
        config_items.temp_sensor.read();
        T1 = config_items.temp_sensor1.read();
        T2 = config_items.temp_sensor2.read();
        T3 = config_items.temp_sensor3.read();
    }
}

#include "../motorlib/system.cpp"
