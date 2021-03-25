#include "param_freebot_g474_col.h"
#include "../motorlib/peripheral/usb.h"
#include "../motorlib/usb_communication.h"
#include "st_device.h"
#include "../motorlib/peripheral/stm32g4/hrpwm.h"

#include "../motorlib/led.h"
#include "../motorlib/peripheral/stm32g4/spi_dma.h"
#include "../motorlib/qep_encoder.h"
#include "../motorlib/torque_sensor.h"
#include "../motorlib/phony_encoder.h"
#include "Inc/main.h"
#include "../motorlib/sensor_multiplex.h"
#include <functional>
#include "../motorlib/peripheral/stm32g4/max31875.h"
#include "../motorlib/gpio.h"

#include "../motorlib/controller/position_controller.h"
#include "../motorlib/controller/torque_controller.h"
#include "../motorlib/controller/impedance_controller.h"
#include "../motorlib/controller/velocity_controller.h"
using TorqueSensor = TorqueSensorBase;
using PWM = HRPWM;
using MotorEncoder = QEPEncoder;
using OutputEncoder = EncoderBase;
using Communication = USBCommunication;
#include "../motorlib/fast_loop.h"
#include "../motorlib/main_loop.h"
#include "../motorlib/actuator.h"
#include "../motorlib/system.h"

#include "../motorlib/peripheral/stm32g4/spi_debug.h"

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
#include "../motorlib/peripheral/stm32g4/pin_config.h"
#include "pin_config_freebot_g474_motor_r0.h"



#define I_A_DR  ADC5->JDR1
#define I_B_DR  ADC4->JDR1
#define I_C_DR  ADC3->JDR1
#define V_BUS_DR ADC1->DR

#include "../motorlib/peripheral/stm32g4/temp_sensor.h"

#define TIM_R TIM4->CCR1
#define TIM_G TIM4->CCR3
#define TIM_B TIM4->CCR2
#ifdef R1
    #undef TIM_R
    #undef TIM_G
    #undef TIM_B
    #define TIM_R TIM4->CCR1
    #define TIM_G TIM4->CCR2
    #define TIM_B TIM4->CCR3
#endif

struct InitCode {
    InitCode() {
        pin_config_freebot_g474_motor_r0();
 
    }
};

volatile uint32_t * const cpu_clock = &DWT->CYCCNT;
static struct {    
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
    TempSensor temp_sensor;
    HRPWM motor_pwm = {pwm_frequency, *HRTIM1, 4, 5, 3, false, 200};

    FastLoop fast_loop = {(int32_t) pwm_frequency, motor_pwm, motor_encoder, param->fast_loop_param, &I_A_DR, &I_B_DR, &I_C_DR, &V_BUS_DR};
    LED led = {const_cast<uint16_t*>(reinterpret_cast<volatile uint16_t *>(&TIM4->CCR1)), 
               const_cast<uint16_t*>(reinterpret_cast<volatile uint16_t *>(&TIM_G)),
               const_cast<uint16_t*>(reinterpret_cast<volatile uint16_t *>(&TIM_B))};
    PositionController position_controller = {(float) (1.0/main_loop_frequency)};
    TorqueController torque_controller = {(float) (1.0/main_loop_frequency)};
    ImpedanceController impedance_controller = {(float) (1.0/main_loop_frequency)};
    VelocityController velocity_controller = {(float) (1.0/main_loop_frequency)};
    MainLoop main_loop = {fast_loop, position_controller, torque_controller, impedance_controller, velocity_controller, System::communication_, led, output_encoder, torque_sensor, param->main_loop_param};
} config_items;

USB1 usb1;
Communication System::communication_ = {usb1};
std::queue<std::string> System::log_queue_ = {};
ParameterAPI System::api = {};
void usb_interrupt() {
    usb1.interrupt();
}


Actuator System::actuator_ = {config_items.fast_loop, config_items.main_loop, param->startup_param};

float v3v3 = 3.3;

int32_t index_mod = 0;

void system_init() {
    if (config_items.motor_encoder.init()) {
        System::log("Motor encoder init success");
    } else {
        System::log("Motor encoder init failure");
    }
    // if (config_items.output_encoder.init()) {
    //     System::log("Output encoder init success");
    // } else {
    //     System::log("Output encoder init failure");
    // }
    if (drv_regs_error) {
        System::log("drv configure failure");
    } else {
        System::log("drv configure success");
    }

    System::api.add_api_variable("3v3", new APIFloat(&v3v3));
    std::function<float()> get_t = std::bind(&TempSensor::get_value, &config_items.temp_sensor);
    std::function<void(float)> set_t = std::bind(&TempSensor::set_value, &config_items.temp_sensor, std::placeholders::_1);
    System::api.add_api_variable("T", new APICallbackFloat(get_t, set_t));
    System::api.add_api_variable("index_mod", new APIInt32(&index_mod));
    System::api.add_api_variable("drv_err", new APICallbackUint32(get_drv_status, drv_reset));

    config_items.torque_sensor.init();
    FLASH->ACR |= FLASH_ACR_PRFTEN;
    MASK_SET(FLASH->ACR, FLASH_ACR_LATENCY, 4);

    for (auto regs : std::vector<ADC_TypeDef*>{ADC1, ADC3, ADC4, ADC5}) {
        regs->CR = ADC_CR_ADVREGEN;
        ns_delay(20000);
        regs->CR |= ADC_CR_ADCAL;
        while(regs->CR & ADC_CR_ADCAL);
        ns_delay(100);

        regs->ISR = ADC_ISR_ADRDY;
        regs->CR |= ADC_CR_ADEN;
    }
    while(!(ADC1->ISR & ADC_ISR_ADRDY));
    ADC1->CR |= ADC_CR_JADSTART;
    while(ADC1->CR & ADC_CR_JADSTART);

    v3v3 =  *((uint16_t *) (0x1FFF75AA)) * 3.0 / ADC1->JDR2;
    System::log("3v3: " + std::to_string(v3v3));

    ADC1->GCOMP = v3v3*4096;
    ADC1->CFGR2 |= ADC_CFGR2_GCOMP;
    ADC1->CR |= ADC_CR_ADSTART;
    ADC5->CR |= ADC_CR_JADSTART;
    ADC5->IER |= ADC_IER_JEOCIE;
    ADC4->CR |= ADC_CR_JADSTART;
    ADC3->CR |= ADC_CR_JADSTART;
   
    TIM1->CR1 = TIM_CR1_CEN; // start main loop interrupt
    usb1.connect();
    HRTIM1->sMasterRegs.MCR = HRTIM_MCR_TDCEN + HRTIM_MCR_TECEN + HRTIM_MCR_TFCEN; // start high res timer
}

FrequencyLimiter temp_rate = {10};

void system_maintenance() {
    if (temp_rate.run()) {
        config_items.temp_sensor.read();
        v3v3 =  *((uint16_t *) (0x1FFF75AA)) * 3.0 * ADC1->GCOMP / 4096.0 / ADC1->JDR2;
    }
    index_mod = config_items.motor_encoder.index_error(param->fast_loop_param.motor_encoder.cpr);
}


#include "../motorlib/system.cpp"
