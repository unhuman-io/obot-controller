#include "../param/param_freebot_g474.h"
#include "../../motorlib/peripheral/usb.h"
#include "../../motorlib/usb_communication.h"
#include "../st_device.h"
#include "../../motorlib/peripheral/stm32g4/hrpwm.h"

#include "../../motorlib/led.h"
#include "../../motorlib/peripheral/stm32g4/spi_dma.h"
#include "../../motorlib/icpz.h"
#include "../../motorlib/torque_sensor.h"
#include <functional>

#include "../../motorlib/gpio.h"

#include "../../motorlib/controller/position_controller.h"
#include "../../motorlib/controller/torque_controller.h"
#include "../../motorlib/controller/impedance_controller.h"
#include "../../motorlib/controller/velocity_controller.h"
typedef TorqueSensorBase TorqueSensor;
typedef HRPWM PWM;
typedef ICPZ MotorEncoder;
typedef EncoderBase OutputEncoder;
typedef USBCommunication Communication;
#include "../../motorlib/fast_loop.h"
#include "../../motorlib/main_loop.h"
#include "../../motorlib/actuator.h"
#include "../../motorlib/system.h"

#include "../../motorlib/peripheral/stm32g4/spi_debug.h"

const uint32_t main_loop_frequency = 10000;
uint16_t drv_regs_error = 0;
uint16_t drv_regs[] = {
  (2<<11) | 0x00,  // control_reg 0x00, 6 PWM mode
  //(3<<11) | 0x3AA, // hs_reg      0x3CC, moderate drive current
  (3<<11) | 0x3FF, // hs_reg      0x3CC, moderate drive current
  //(4<<11) | 0x2FF, // ls_reg      0x0CC, no cycle by cycle, 500 ns tdrive
                                // moderate drive current (.57,1.14A)
  (4<<11) | 0x37F, // ls_reg      0x0CC, no cycle by cycle, 4000 ns tdrive
                                // moderate drive current (.57,1.14A)
  (5<<11) | 0x000,  // ocp_reg     0x00 -> 50 ns dead time, 
                              //latched ocp, 2 us ocp deglitch, 0.06 Vds thresh
  //(6<<11) | 0x2C0, // csa_reg     0x2C0 -> bidirectional current, 40V/V
  //(6<<11) | 0x280,
  (6<<11) | 0x240, // csa_reg     0x240 -> bidirectional current, 10V/V
};

#include "../../motorlib/peripheral/stm32g4/pin_config.h"
#include "pin_config_freebot_g474_motor.h"

#include "../../motorlib/peripheral/stm32g4/temp_sensor.h"

struct InitCode {
    InitCode() {
        pin_config_freebot_g474_motor_r0();

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
    uint32_t pwm_frequency = 50000;

    GPIO motor_encoder_cs = {*GPIOD, 2, GPIO::OUTPUT};
    volatile int spi3_register_operation = 0;
    SPIDMA spi_dma = {*SPI3, motor_encoder_cs, *DMA1_Channel1, *DMA1_Channel2};
    ICPZ motor_encoder = {spi_dma};
    SPIDebug spi_debug = {*SPI3, motor_encoder_cs, *DMA1_Channel1, *DMA1_Channel2};
    //PhonyEncoder motor_encoder = {700};
    GPIO torque_cs = {*GPIOA, 4, GPIO::OUTPUT};
  //  SPITorque torque_sensor = {*SPI1, torque_cs, *DMA1_Channel1, *DMA1_Channel2, 4};
    TorqueSensor torque_sensor;
    GPIO output_encoder_cs = {*GPIOD, 2, GPIO::OUTPUT};
    OutputEncoder output_encoder;// = {*SPI3, output_encoder_cs, 153, &spi3_register_operation}; // need to make sure this doesn't collide with motor encoder
    //PhonyEncoder output_encoder = {100};
    //GPIO enable = {*GPIOC, 11, GPIO::OUTPUT};
    TempSensor temp_sensor;
    HRPWM motor_pwm = {pwm_frequency, *HRTIM1, 4, 5, 3, false, 200};
    USB1 usb1;
    FastLoop fast_loop = {(int32_t) pwm_frequency, motor_pwm, motor_encoder, param->fast_loop_param, &I_A_DR, &I_B_DR, &I_C_DR, &V_BUS_DR};
    LED led = {const_cast<uint16_t*>(reinterpret_cast<volatile uint16_t *>(&TIM4->CCR1)), 
               const_cast<uint16_t*>(reinterpret_cast<volatile uint16_t *>(&TIM4->CCR3)),
               const_cast<uint16_t*>(reinterpret_cast<volatile uint16_t *>(&TIM4->CCR2))};
    PositionController position_controller = {(float) (1.0/main_loop_frequency)};
    TorqueController torque_controller = {(float) (1.0/main_loop_frequency)};
    ImpedanceController impedance_controller = {(float) (1.0/main_loop_frequency)};
    VelocityController velocity_controller = {(float) (1.0/main_loop_frequency)};
    MainLoop main_loop = {fast_loop, position_controller, torque_controller, impedance_controller, velocity_controller, System::communication_, led, output_encoder, torque_sensor, param->main_loop_param};
};

std::string val;
void set_spi_debug(std::string s) {
    config::motor_encoder.set_register_operation();
    val = config::spi_debug.read(s);
    config::motor_encoder.clear_register_operation();
}
std::string get_spi_debug() {
    return val;
}

void config_init() {
    System::api.add_api_variable("spi", new APICallback(get_spi_debug, set_spi_debug));
}

void config_maintenance() {}

#include "config_freebot_g474_motor.cpp"
