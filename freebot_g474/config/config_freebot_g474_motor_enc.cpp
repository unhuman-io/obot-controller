#include "../param/param_freebot_g474.h"
#include "../../motorlib/peripheral/usb.h"
#include "../../motorlib/usb_communication.h"
#include "../st_device.h"
#include "../../motorlib/peripheral/stm32g4/hrpwm.h"

#include "../../motorlib/led.h"
#include "../../motorlib/peripheral/stm32g4/spi_dma.h"
#include "../../motorlib/qep_encoder.h"
#include "../../motorlib/torque_sensor.h"

#include <functional>

#include "../../motorlib/gpio.h"

#include "../../motorlib/controller/position_controller.h"
#include "../../motorlib/controller/torque_controller.h"
#include "../../motorlib/controller/impedance_controller.h"
#include "../../motorlib/controller/velocity_controller.h"
using TorqueSensor = TorqueSensorBase;
using PWM = HRPWM;
using MotorEncoder = QEPEncoder;
using OutputEncoder = EncoderBase;
using Communication = USBCommunication;
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

#include "pin_config_freebot_g474_motor.h"
#include "../../motorlib/peripheral/stm32g4/temp_sensor.h"


struct InitCode {
    InitCode() {
        pin_config_freebot_g474_motor_r0();
    }
};

volatile uint32_t * const cpu_clock = &DWT->CYCCNT;
namespace config {    
    uint32_t pwm_frequency = 50000;
    static_assert(((double) CPU_FREQUENCY_HZ * 32 / 2) / 50000 < 65535);    // check pwm frequency
    SystemInitClass system_init; // first item to enable clocks, etc.
    InitCode init_code;
    GPIO motor_encoder_cs = {*GPIOD, 2, GPIO::OUTPUT};
    volatile int spi3_register_operation = 0;
    //SPIDMA spi_dma = {*SPI3, motor_encoder_cs, *DMA1_Channel1, *DMA1_Channel2};
    //ICPZ motor_encoder = {spi_dma};
    QEPEncoder motor_encoder = {*TIM2};
    //SPIDebug spi_debug = {*SPI3, motor_encoder_cs, *DMA1_Channel1, *DMA1_Channel2};
    //PhonyEncoder motor_encoder = {700};
    GPIO torque_cs = {*GPIOA, 4, GPIO::OUTPUT};
  //  SPITorque torque_sensor = {*SPI1, torque_cs, *DMA1_Channel1, *DMA1_Channel2, 4};
    TorqueSensor torque_sensor;
    GPIO output_encoder_cs = {*GPIOD, 2, GPIO::OUTPUT};
    OutputEncoder output_encoder;// = {*SPI3, output_encoder_cs, 153, &spi3_register_operation}; // need to make sure this doesn't collide with motor encoder
    //PhonyEncoder output_encoder = {100};
    //GPIO enable = {*GPIOC, 11, GPIO::OUTPUT};
};

void config_init() {}
void config_maintenance() {}

#include "config_freebot_g474_motor.cpp"