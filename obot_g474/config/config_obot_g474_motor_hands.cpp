#include "../param/param_obot_g474_motor_hands.h"
#include "../st_device.h"
#include "../../motorlib/torque_sensor.h"
#include "../../motorlib/gpio.h"
#include "../../motorlib/qep_encoder.h"
#include "../../motorlib/peripheral/stm32g4/pin_config.h"

using TorqueSensor = TorqueSensorBase;
using MotorEncoder = QEPEncoder;
using OutputEncoder = EncoderBase;

extern "C" void SystemClock_Config();
void pin_config_obot_g474_motor_hands();

struct InitCode
{
    InitCode()
    {
        SystemClock_Config();

        pin_config_obot_g474_motor_hands();
    }
};

namespace config
{
    const uint32_t main_loop_frequency = 10000;
    const uint32_t pwm_frequency = 50000;
    InitCode init_code;

    QEPEncoder motor_encoder(*TIM2);

    TorqueSensor torque_sensor;

    EncoderBase output_encoder;
};

#include "../../motorlib/peripheral/usb.h"
#include "../../motorlib/spi_communication.h"
#include "../../motorlib/peripheral/stm32g4/hrpwm.h"
#include "../../motorlib/util.h"
#include "../../motorlib/driver_mps.h"

using PWM = HRPWM3;
using Communication = SPICommunication;
using Driver = DriverMPS;
volatile uint32_t *const cpu_clock = &DWT->CYCCNT;

#include "../../motorlib/led.h"
#include "../../motorlib/controller/position_controller.h"
#include "../../motorlib/controller/torque_controller.h"
#include "../../motorlib/controller/impedance_controller.h"
#include "../../motorlib/controller/velocity_controller.h"
#include "../../motorlib/controller/state_controller.h"
#include "../../motorlib/controller/joint_position_controller.h"
#include "../../motorlib/fast_loop.h"
#include "../../motorlib/main_loop.h"
#include "../../motorlib/actuator.h"
#include "../../motorlib/system.h"
#include "pin_config_obot_g474_motor_hands.h"
#include "../../motorlib/peripheral/stm32g4/temp_sensor.h"
#include "../../motorlib/peripheral/stm32g4/max31875.h"
#include "../../motorlib/peripheral/stm32g4/spi_slave.h"
#include "../../motorlib/peripheral/spi_protocol.h"
#include "../../motorlib/spi_communication.h"

HardwareBrakeBase MainLoop::no_brake_;

namespace config
{
    static_assert(((double)CPU_FREQUENCY_HZ * 8 / 2) / pwm_frequency < 65535); // check pwm frequency
    /* TempSensor temp_sensor; */
    /* I2C i2c1(*I2C1, 1000); */
    /* MAX31875 board_temperature(i2c1); */
    DriverMPS driver;

    SpiSlave spi_slave({
      .spi          = SPI1,
      .gpioPort     = GPIOA,
      .gpioPinSs    = 4U,
      .gpioPinSck   = 5U,
      .gpioPinMosi  = 7U,
      .gpioPinMiso  = 6U,

      .gpioAlternateFunction = 5U,

      .gpioRccEnableRegister = &RCC->AHB2ENR,
      .gpioRccEnableBit      = RCC_AHB2ENR_GPIOAEN_Pos,
      .spiRccEnableRegister = &RCC->APB2ENR,
      .spiRccEnableBit      = RCC_APB2ENR_SPI1EN_Pos,
      .spiRccResetRegister  = &RCC->APB2RSTR,
      .spiRccResetBit       = RCC_APB2RSTR_SPI1RST_Pos,

      .rxDma            = DMA2,
      .rxDmaIfcrCgif    = DMA_IFCR_CGIF1,
      .rxDmaChannel     = DMA2_Channel1,
      .rxDmaMuxChannel  = DMAMUX1_Channel8,
      .rxDmaMuxId       = 10U,
      .rxDmaIrqN        = DMA2_Channel1_IRQn,
      .rxDmaIrqPriority = 1U,

      .txDma            = DMA2,
      .txDmaIfcrCgif    = DMA_IFCR_CGIF2,
      .txDmaChannel     = DMA2_Channel2,
      .txDmaMuxChannel  = DMAMUX1_Channel9,
      .txDmaMuxId       = 11U,
      .txDmaIrqN        = DMA2_Channel2_IRQn,
      .txDmaIrqPriority = 5U,
    });

    // SPI communication protocol buffers and pools allocation
    SpiMailbox::Buffer spi_data_to_host_buffers[2];
    SpiMailbox::Buffer spi_data_from_host_buffers[2];
    SpiMailbox::Buffer spi_string_buffers[8];

    SpiMailbox::Pool spi_pools[] = {
       {
         .mailboxIds = {SPICommunication::MAILBOX_ID_DATA_TO_HOST},
         .buffers = spi_data_to_host_buffers,
         .buffersCount = FIGURE_COUNTOF(spi_data_to_host_buffers)
       },
       {
         .mailboxIds = {SPICommunication::MAILBOX_ID_DATA_FROM_HOST},
         .buffers = spi_data_from_host_buffers,
         .buffersCount = FIGURE_COUNTOF(spi_data_from_host_buffers)
       },
       {
         .mailboxIds = {SPICommunication::MAILBOX_ID_SERIAL_TO_HOST, SPICommunication::MAILBOX_ID_SERIAL_FROM_HOST},
         .buffers = spi_string_buffers,
         .buffersCount = FIGURE_COUNTOF(spi_string_buffers)
       }
    };

    SpiProtocol protocol(spi_slave, spi_pools, FIGURE_COUNTOF(spi_pools));

    HRPWM3 motor_pwm(pwm_frequency, *HRTIM1, 3, 3, 0, 1000, 0);

    FastLoop fast_loop = {(int32_t)pwm_frequency, motor_pwm, motor_encoder, param->fast_loop_param, &I_A_DR, &I_B_DR, &I_C_DR, &V_BUS_DR};
    LED led = {const_cast<uint16_t *>(reinterpret_cast<volatile uint16_t *>(&TIM_R)),
               const_cast<uint16_t *>(reinterpret_cast<volatile uint16_t *>(&TIM_G)),
               const_cast<uint16_t *>(reinterpret_cast<volatile uint16_t *>(&TIM_B))};
    PositionController position_controller = {(float)(1.0 / main_loop_frequency)};
    TorqueController torque_controller = {(float)(1.0 / main_loop_frequency)};
    ImpedanceController impedance_controller = {(float)(1.0 / main_loop_frequency)};
    VelocityController velocity_controller = {(float)(1.0 / main_loop_frequency)};
    StateController state_controller = {(float)(1.0 / main_loop_frequency)};
    JointPositionController joint_position_controller(1.0 / main_loop_frequency);
    MainLoop main_loop(fast_loop, position_controller, torque_controller, impedance_controller, velocity_controller, state_controller, joint_position_controller, System::communication_, led, output_encoder, torque_sensor, driver, param->main_loop_param);
};

Communication System::communication_(config::protocol);
Actuator System::actuator_ = {config::fast_loop, config::main_loop, param->startup_param};

void usb_interrupt(){}

float v3v3 = 3.3;

int32_t index_mod = 0;

void config_init();

void system_init()
{
    if (config::motor_encoder.init())
    {
        System::log("Motor encoder init success");
    }
    else
    {
        System::log("Motor encoder init failure");
    }
    if (config::output_encoder.init())
    {
        System::log("Output encoder init success");
    }
    else
    {
        System::log("Output encoder init failure");
    }
    if (config::torque_sensor.init())
    {
        System::log("Torque sensor init success");
    }
    else
    {
        System::log("Torque sensor init failure");
    }

    System::api.add_api_variable("3v3", new APIFloat(&v3v3));
    /* std::function<float()> get_t = std::bind(&TempSensor::get_value, &config::temp_sensor); */
    /* std::function<void(float)> set_t = std::bind(&TempSensor::set_value, &config::temp_sensor, std::placeholders::_1); */
    /* System::api.add_api_variable("T", new APICallbackFloat(get_t, set_t)); */
    /* System::api.add_api_variable("Tboard", new const APICallbackFloat([]() */
    /*                                                                   { return config::board_temperature.get_temperature(); })); */
    // System::api.add_api_variable("Tboard", new const APICallbackFloat([]()
    //                                                                   { return (TS_CAL2_TEMP - TS_CAL1_TEMP)/(*(uint16_t*)TS_CAL2_REG - *(uint16_t*)TS_CAL1_REG) * (V_TEMP_DR - *(uint16_t*)TS_CAL1_REG) + 30.0f; }));
    System::api.add_api_variable("SG1", new const APICallbackFloat([]()
                                                                      { return V_SG1_DR; }));
    System::api.add_api_variable("SG2", new const APICallbackFloat([]()
                                                                      { return V_SG2_DR; }));
    System::api.add_api_variable("index_mod", new APIInt32(&index_mod));
    System::api.add_api_variable("drv_reset", new const APICallback([]()
                                                                    { return config::driver.reset(); }));
    System::api.add_api_variable("shutdown", new const APICallback([]()
                                                                   {
        // requires power cycle to return 
        setup_sleep();
        SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk;
        PWR->CR1 |= 0b100 << PWR_CR1_LPMS_Pos;
        __WFI();
        return ""; }));
    System::api.add_api_variable("deadtime", new APICallbackUint16([]()
                                                                   { return config::motor_pwm.deadtime_ns_; },
                                                                   [](uint16_t u)
                                                                   { config::motor_pwm.set_deadtime(u); }));

    for (auto regs : std::vector<ADC_TypeDef *>{ADC1, ADC2, ADC3, ADC4, ADC5})
    {
        
        regs->CR = ADC_CR_ADVREGEN;
        ns_delay(20000);
        regs->CR |= ADC_CR_ADCAL;
        while (regs->CR & ADC_CR_ADCAL)
            ;
        ns_delay(100);
        regs->CR |= ADC_CR_ADCALDIF;
        regs->CR |= ADC_CR_ADCAL;
        while (regs->CR & ADC_CR_ADCAL)
            ;
        ns_delay(100);

        regs->ISR = ADC_ISR_ADRDY;
        regs->CR |= ADC_CR_ADEN;
        while (!(regs->ISR & ADC_ISR_ADRDY))
            ;
    }

    ADC1->CR |= ADC_CR_JADSTART;
    while (ADC1->CR & ADC_CR_JADSTART)
        ;
    // ADC1->ISR = ADC_ISR_EOC;
    // ADC1->CR |= ADC_CR_ADSTART;
    
    // while (!(ADC1->ISR & ADC_ISR_EOC));
    // System::log("VREFINT_MEAS_1:" + std::to_string(V_REF_DR));



    v3v3 =   *VREFINT_CAL_ADDR * VREFINT_CAL_VREF / V_REF_DR / 1000.0;
    /* v3v3 = 3.3; */
    System::log("VREFINT_CAL:" + std::to_string(*VREFINT_CAL_ADDR));
    System::log("VREFINT_MEAS_2:" + std::to_string(V_REF_DR));
    System::log("3v3: " + std::to_string(v3v3));

    ADC1->GCOMP = v3v3 * 4096;
    ADC1->CFGR2 |= ADC_CFGR2_GCOMP;
    ADC2->GCOMP = v3v3 * 4096;
    ADC2->CFGR2 |= ADC_CFGR2_GCOMP;

    ADC1->CR |= ADC_CR_ADSTART;
    ADC2->CR |= ADC_CR_JADSTART;
    ADC5->CR |= ADC_CR_JADSTART;
    ADC5->IER |= ADC_IER_JEOCIE;
    ADC4->CR |= ADC_CR_JADSTART;
    ADC3->CR |= ADC_CR_JADSTART;

    config_init();

    TIM1->CR1 = TIM_CR1_CEN; // start main loop interrupt
    HRTIM1->sMasterRegs.MCR = HRTIM_MCR_TDCEN + HRTIM_MCR_TACEN + HRTIM_MCR_TFCEN; // start high res timer
}

FrequencyLimiter temp_rate = {10};
float T = 0;

void config_maintenance();
void system_maintenance()
{
    static bool driver_fault = false;
    if (temp_rate.run())
    {
        ADC1->CR |= ADC_CR_JADSTART;
        while (ADC1->CR & ADC_CR_JADSTART);

        /* T = config::temp_sensor.read(); */
        v3v3 = *VREFINT_CAL_ADDR * VREFINT_CAL_VREF/1000.0 * ADC1->GCOMP / 4096.0 / V_REF_DR;
        /* if (T > 100) */
        /* { */
        /*     // config::main_loop.status_.error.microcontroller_temperature = 1; */
        /* } */
    }
    // pc 14 is low there is a fault
    if (!(GPIOC->IDR & 1 << 14))
    {
        driver_fault = true;
    } else if (param->main_loop_param.no_latch_driver_fault) {
        driver_fault = false;
    }
    config::main_loop.status_.error.driver_fault |= driver_fault;
    index_mod = config::motor_encoder.index_error(param->fast_loop_param.motor_encoder.cpr);
    config_maintenance();
}

void setup_sleep()
{
    NVIC_DisableIRQ(TIM1_UP_TIM16_IRQn);
    NVIC_DisableIRQ(ADC5_IRQn);
    config::driver.disable();
    NVIC_SetPriority(USB_LP_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 0, 1));
    NVIC_EnableIRQ(RTC_WKUP_IRQn);
    MASK_SET(RCC->CFGR, RCC_CFGR_SW, 2); // HSE is system clock source
    RTC->SCR = RTC_SCR_CWUTF;
}

void finish_sleep()
{
    MASK_SET(RCC->CFGR, RCC_CFGR_SW, 3); // PLL is system clock source
    config::driver.enable();
    NVIC_DisableIRQ(RTC_WKUP_IRQn);
    NVIC_SetPriority(USB_LP_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 2, 0));
    NVIC_EnableIRQ(TIM1_UP_TIM16_IRQn);
    NVIC_EnableIRQ(ADC5_IRQn);
}

#include "../../motorlib/system.cpp"

void config_init()
{
    System::api.add_api_variable("3v3_bus", new APIUint32(&V_3V3_BUS));

    // Init SPI protocol
    config::spi_slave.init();
    config::protocol.init();

    // System::api.add_api_variable("torque1", new const APIFloat(&config::torque_sensor.torque1_));
    // System::api.add_api_variable("torque2", new const APIFloat(&config::torque_sensor.torque2_));
    // System::api.add_api_variable("torque_decimation", new APIUint16(&config::torque_sensor.decimation_));
}

void config_maintenance() {}
