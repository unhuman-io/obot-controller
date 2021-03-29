#include "../param/param_freebot_g474.h"
#include "../../motorlib/peripheral/usb.h"
#include "../../motorlib/usb_communication.h"
#include "../st_device.h"
#include "../../motorlib/peripheral/stm32g4/hrpwm.h"

#include "../../motorlib/led.h"
#include "../../motorlib/ma732_encoder.h"
#include "../../motorlib/peripheral/stm32g4/spi_torque.h"
#include "../../motorlib/sensor_multiplex.h"
#include <functional>
#include "../../motorlib/peripheral/stm32g4/max31875.h"

#include "../../motorlib/controller/position_controller.h"
#include "../../motorlib/controller/torque_controller.h"
#include "../../motorlib/controller/impedance_controller.h"
#include "../../motorlib/controller/velocity_controller.h"
typedef SPITorque TorqueSensor;
typedef HRPWM PWM;
typedef SensorMultiplex<MA732Encoder, MA732Encoder> MotorEncoder;
typedef MotorEncoder::SecondarySensor OutputEncoder;
typedef USBCommunication Communication;
#include "../../motorlib/fast_loop.h"
#include "../../motorlib/main_loop.h"
#include "../../motorlib/actuator.h"
#include "../../motorlib/system.h"
const uint32_t main_loop_frequency = 10000;

#include "pin_config_freebot_g474_col.h"
struct InitCode {
    InitCode() {
        pin_config_freebot_g474_col();

        //SPI3 PZ
        DMAMUX1_Channel0->CCR =  DMA_REQUEST_SPI3_TX;
        DMAMUX1_Channel1->CCR =  DMA_REQUEST_SPI3_RX;
        SPI3->CR1 = SPI_CR1_MSTR | (3 << SPI_CR1_BR_Pos) | SPI_CR1_SSI | SPI_CR1_SSM;    // baud = clock/16 spi mode 0
        SPI3->CR2 = (7 << SPI_CR2_DS_Pos) | SPI_CR2_FRXTH;    // 8 bit   
    }
};

volatile uint32_t * const cpu_clock = &DWT->CYCCNT;
namespace config {
    SystemInitClass system_init; // first item to enable clocks, etc.
    InitCode init_code;
    uint32_t pwm_frequency = (double) 100000;
    GPIO motor_encoder_cs = {*GPIOB, 4, GPIO::OUTPUT};
    volatile int spi3_register_operation = 0;
    MA732Encoder motor_encoder = {*SPI3, motor_encoder_cs, 102, &spi3_register_operation};
    GPIO torque_cs = {*GPIOA, 4, GPIO::OUTPUT};
    SPITorque torque_sensor = {*SPI1, torque_cs, *DMA1_Channel1, *DMA1_Channel2, 4};
    GPIO output_encoder_cs = {*GPIOD, 2, GPIO::OUTPUT};
    MA732Encoder output_encoder = {*SPI3, output_encoder_cs, 153, &spi3_register_operation}; // need to make sure this doesn't collide with motor encoder
    I2C i2c = {*I2C1};
    I2C i2c2 = {*I2C2};
    MAX31875 temp_sensor = {i2c, 0x48};        // R0
    MAX31875 temp_sensor1 = {i2c2, 0x48};      // R0
    MAX31875 temp_sensor2 = {i2c2, 0x49};      // R1
    MAX31875 temp_sensor3 = {i2c2, 0x4A};      // R2
    HRPWM motor_pwm = {pwm_frequency, *HRTIM1, 4, 5, 3, false};
    USB1 usb;
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
};

volatile float T1=0, T2=0, T3=0;

void config_init() {
    std::function<void(uint32_t)> setbct = std::bind(&MA732Encoder::set_bct, &config::motor_encoder, std::placeholders::_1);
    std::function<uint32_t(void)> getbct = std::bind(&MA732Encoder::get_bct, &config::motor_encoder);
    System::api.add_api_variable("mbct", new APICallbackUint32(getbct, setbct));

    std::function<void(uint32_t)> set_et = std::bind(&MA732Encoder::set_et, &config::motor_encoder, std::placeholders::_1);
    std::function<uint32_t(void)> get_et = std::bind(&MA732Encoder::get_et, &config::motor_encoder);
    System::api.add_api_variable("met", new APICallbackUint32(get_et, set_et));

    std::function<void(uint32_t)> set_mgt = std::bind(&MA732Encoder::set_mgt, &config::motor_encoder, std::placeholders::_1);
    std::function<uint32_t(void)> get_mgt = std::bind(&MA732Encoder::get_magnetic_field_strength, &config::motor_encoder);
    System::api.add_api_variable("mmgt", new APICallbackUint32(get_mgt, set_mgt));

    std::function<void(uint32_t)> setbctj = std::bind(&MA732Encoder::set_bct, &config::output_encoder, std::placeholders::_1);
    std::function<uint32_t(void)> getbctj = std::bind(&MA732Encoder::get_bct, &config::output_encoder);
    System::api.add_api_variable("jbct", new APICallbackUint32(getbctj, setbctj));

    std::function<void(uint32_t)> set_etj = std::bind(&MA732Encoder::set_et, &config::output_encoder, std::placeholders::_1);
    std::function<uint32_t(void)> get_etj = std::bind(&MA732Encoder::get_et, &config::output_encoder);
    System::api.add_api_variable("jet", new APICallbackUint32(get_etj, set_etj));

    std::function<void(uint32_t)> set_mgtj = std::bind(&MA732Encoder::set_mgt, &config::output_encoder, std::placeholders::_1);
    std::function<uint32_t(void)> get_mgtj = std::bind(&MA732Encoder::get_magnetic_field_strength, &config::output_encoder);
    System::api.add_api_variable("jmgt", new APICallbackUint32(get_mgtj, set_mgtj));

    System::api.add_api_variable("c1",new APIUint32(&config::torque_sensor.result0_));
    System::api.add_api_variable("c2",new APIUint32(&config::torque_sensor.result1_));

    std::function<void(uint32_t)> set_t_reset = std::bind(&SPITorque::reset, &config::torque_sensor, std::placeholders::_1);
    std::function<uint32_t(void)> get_t_reset = std::bind(&SPITorque::reset2, &config::torque_sensor);
    System::api.add_api_variable("t_reset", new APICallbackUint32(get_t_reset, set_t_reset));

    System::api.add_api_variable("T1", new APIFloat((float*) &T1));
    System::api.add_api_variable("T2", new APIFloat((float*) &T2));
    System::api.add_api_variable("T3", new APIFloat((float*) &T3));


    System::actuator_.main_loop_.reserved1_ = &config::temp_sensor.value_;// &config::torque_sensor.result0_;
    System::actuator_.main_loop_.reserved2_ = &config::torque_sensor.sum_;
    config::torque_sensor.init();
    config::motor_pwm.init();
}

FrequencyLimiter temp_rate2 = {8};

void config_maintenance() {
    if (temp_rate2.run()) {
        T1 = config::temp_sensor1.read();
        T2 = config::temp_sensor2.read();
        T3 = config::temp_sensor3.read();
    }
}

#include "config_freebot_g474_col.cpp"
