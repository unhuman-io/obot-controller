#include "../motorlib/system_log.h"
#include "../motorlib/peripheral/usb.h"
#include "../motorlib/usb_communication.h"
#include "st_device.h"
#include "../motorlib/peripheral/stm32g4/hrpwm.h"
#include "../motorlib/led.h"
#include "../motorlib/ma732_encoder.h"
#include "../motorlib/peripheral/stm32g4/spi_torque.h"
#include "Inc/main.h"
#include "param_g474_boost.h"
#include "../motorlib/peripheral/stm32g4/spi_dma.h"
#include "../motorlib/ads1235.h"

#include "../motorlib/controller/position_controller.h"
#include "../motorlib/controller/torque_controller.h"
#include "../motorlib/controller/impedance_controller.h"
#include "../motorlib/controller/velocity_controller.h"
typedef ADS1235 TorqueSensor;
typedef HRPWM PWM;
typedef EncoderBase OutputEncoder;
typedef MA732Encoder MotorEncoder;
typedef USBCommunication Communication;
#include "../motorlib/fast_loop.h"
#include "../motorlib/main_loop.h"
#include "../motorlib/actuator.h"
#include "../motorlib/system.h"

USB1 usb1;
Communication System::communication_ = {usb1};
std::queue<std::string> System::log_queue_ = {};
ParameterAPI System::api = {};

void usb_interrupt() {
    usb1.interrupt();
}

#include <functional>
uint16_t drv_regs_error = 0;

uint16_t drv_regs[] = {
  (2<<11) | 0x00,  // control_reg 0x00, 6 PWM mode
  //(3<<11) | 0x3AA, // hs_reg      0x3CC, moderate drive current
  (3<<11) | 0x333, // hs_reg      0x3CC, moderate drive current
  //(4<<11) | 0x2FF, // ls_reg      0x0CC, no cycle by cycle, 500 ns tdrive
                                // moderate drive current (.57,1.14A)
  (4<<11) | 0x2AA, // ls_reg      0x0CC, no cycle by cycle, 500 ns tdrive
                                // moderate drive current (.57,1.14A)
  (5<<11) | 0x020,  // ocp_reg     0x20 -> 50 ns dead time, 
                              //latched ocp, 4 us ocp deglitch, 0.06 Vds thresh
  //(6<<11) | 0x2C0, // csa_reg     0x2C0 -> bidirectional current, 40V/V
  (6<<11) | 0x280, // csa_reg     0x280 -> bidirectional current, 20V/V
};     

#define MASK_SET(var, item, val) var = (var & ~item##_Msk) | (val << item##_Pos)
#define GPIO_SETL(gpio, pin, mode, speed, af) \
    MASK_SET(GPIO##gpio->MODER, GPIO_MODER_MODE##pin, mode); \
    MASK_SET(GPIO##gpio->OSPEEDR, GPIO_OSPEEDR_OSPEED##pin, speed); \
    MASK_SET(GPIO##gpio->AFR[0], GPIO_AFRL_AFSEL##pin, af)

#define GPIO_SETH(gpio, pin, mode, speed, af) \
    MASK_SET(GPIO##gpio->MODER, GPIO_MODER_MODE##pin, mode); \
    MASK_SET(GPIO##gpio->OSPEEDR, GPIO_OSPEEDR_OSPEED##pin, speed); \
    MASK_SET(GPIO##gpio->AFR[1], GPIO_AFRH_AFSEL##pin, af)

volatile uint32_t * const cpu_clock = &TIM2->CNT;
struct InitCode {
    InitCode() {
        // Peripheral clock enable
        RCC->APB1ENR1 = RCC_APB1ENR1_SPI3EN | RCC_APB1ENR1_TIM2EN;
        RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;
        RCC->AHB1ENR |= RCC_AHB1ENR_DMA1EN | RCC_AHB1ENR_DMAMUX1EN;
        RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN | RCC_AHB2ENR_GPIOBEN | RCC_AHB2ENR_GPIOCEN;

        // GPIO configure
        GPIO_SETL(A, 0, 1, 3, 0);   // SPI1 CS on QEPA pin
        GPIO_SETL(A, 4, 2, 3, 5);   // SPI1 CS on boostxl J4-18
        GPIO_SETL(A, 5, 2, 3, 5);   // SPI1 CLK on boostxl J3-13
        GPIO_SETL(A, 6, 2, 3, 5);   // SPI1 DDO (Device Data Out) on boostxl J4-14
        GPIO_SETL(A, 7, 2, 3, 5);   // SPI1 DDI (Device Data In) on boostxl J4-12
     
        GPIO_SETL(B, 3, 2, 3, 6);   // SPI3 CLK
        GPIO_SETL(B, 4, 2, 3, 6);   // SPI3 DDO
        GPIO_SETL(B, 5, 2, 3, 6);   // SPI3 DDI 

        GPIO_SETH(C, 11, 1, 0, 0);  // Boostxl enable

        // TIM2 cpu clock
        TIM2->CR1 = TIM_CR1_CEN;

        // SPI1 DRV8323RS
        GPIOA->BSRR = GPIO_ODR_OD0;  // disable other spi cs
        GPIOC->BSRR = GPIO_ODR_OD11; // drv enable
        ms_delay(10);

        SPI1->CR2 = (15 << SPI_CR2_DS_Pos) | SPI_CR2_FRF;   // 16 bit TI mode
        // ORDER DEPENDANCE SPE set last
        SPI1->CR1 = SPI_CR1_MSTR | (5 << SPI_CR1_BR_Pos) | SPI_CR1_SPE;    // baud = clock/64
        for (uint8_t i=0; i<sizeof(drv_regs)/sizeof(uint16_t); i++) {
            uint16_t reg_out = drv_regs[i];
            uint16_t reg_in = 0;
            SPI1->DR = reg_out;
            while(!(SPI1->SR & SPI_SR_RXNE));
            reg_in = SPI1->DR;

            reg_out |= (1<<15); // switch to read mode
            SPI1->DR = reg_out;
            while(!(SPI1->SR & SPI_SR_RXNE));
            reg_in = SPI1->DR;
            if ((reg_in & 0x7FF) != (reg_out & 0x7FF)) {
            drv_regs_error |= 1 << i;
            }
        }
        SPI1->CR1 = 0; // clear SPE
        // SPI1 CS-> gpio
        GPIO_SETL(A, 4, 1, 0, 0);
        GPIOA->BSRR = GPIO_ODR_OD4;

        // SPI1 ADS1235
        DMAMUX1_Channel0->CCR =  DMA_REQUEST_SPI1_TX;
        DMAMUX1_Channel1->CCR =  DMA_REQUEST_SPI1_RX;
        SPI1->CR1 = SPI_CR1_CPHA | SPI_CR1_MSTR | (4 << SPI_CR1_BR_Pos) | SPI_CR1_SSI | SPI_CR1_SSM | SPI_CR1_SPE;    // baud = clock/32
        SPI1->CR2 = (7 << SPI_CR2_DS_Pos) | SPI_CR2_FRXTH;    // 8 bit   

        // SPI3 MA732
        SPI3->CR1 = SPI_CR1_MSTR | (3 << SPI_CR1_BR_Pos) | SPI_CR1_SSI | SPI_CR1_SSM | SPI_CR1_SPE;    // baud = clock/16
        SPI3->CR2 = (15 << SPI_CR2_DS_Pos);    // 16 bit 

        // SPI3 ADS1235
        // DMAMUX1_Channel0->CCR =  DMA_REQUEST_SPI3_TX;
        // DMAMUX1_Channel1->CCR =  DMA_REQUEST_SPI3_RX;
        // SPI3->CR1 = SPI_CR1_CPHA | SPI_CR1_MSTR | (4 << SPI_CR1_BR_Pos) | SPI_CR1_SSI | SPI_CR1_SSM;    // baud = clock/32
        // SPI3->CR2 = (7 << SPI_CR2_DS_Pos) | SPI_CR2_FRXTH;    // 8 bit   
    }
};

static struct {
    SystemInitClass system_init;
    InitCode init_code;
    uint32_t pwm_frequency = (double) CPU_FREQUENCY_HZ * 32.0 / (hrperiod);
    uint32_t main_loop_frequency = (double) CPU_FREQUENCY_HZ/(main_loop_period);
    GPIO motor_encoder_cs = {*GPIOA, 15, GPIO::OUTPUT};
    GPIO torque_sensor_cs = {*GPIOA, 0, GPIO::OUTPUT};
    SPIDMA spi_dma = {*SPI1, torque_sensor_cs, *DMA1_Channel1, *DMA1_Channel2};
    ADS1235 torque_sensor = {spi_dma};
    GPIO hall_a = {*GPIOC, 0, GPIO::INPUT};
    GPIO hall_b = {*GPIOC, 1, GPIO::INPUT};
    GPIO hall_c = {*GPIOC, 2, GPIO::INPUT};
    MA732Encoder motor_encoder = {*SPI3, motor_encoder_cs};
    EncoderBase output_encoder;
    GPIO enable = {*GPIOC, 11, GPIO::OUTPUT};
    HRPWM motor_pwm = {pwm_frequency, *HRTIM1, 3, 5, 4};
    FastLoop fast_loop = {(int32_t) pwm_frequency, motor_pwm, motor_encoder, param->fast_loop_param, &ADC5->JDR1, &ADC4->JDR1, &ADC3->JDR1, &ADC1->DR};
    LED led = {const_cast<uint16_t*>(reinterpret_cast<volatile uint16_t *>(&TIM4->CCR1)), 
               const_cast<uint16_t*>(reinterpret_cast<volatile uint16_t *>(&TIM4->CCR2)),
               const_cast<uint16_t*>(reinterpret_cast<volatile uint16_t *>(&TIM4->CCR3))};
    PositionController position_controller = {(float) (1.0/main_loop_frequency)};
    TorqueController torque_controller = {(float) (1.0/main_loop_frequency)};
    ImpedanceController impedance_controller = {(float) (1.0/main_loop_frequency)};
    VelocityController velocity_controller = {(float) (1.0/main_loop_frequency)};
    MainLoop main_loop = {fast_loop, position_controller, torque_controller, impedance_controller, velocity_controller, System::communication_, led, output_encoder, torque_sensor, param->main_loop_param};
} config_items;

Actuator System::actuator_ = {config_items.fast_loop, config_items.main_loop, param->startup_param};


uint32_t get_drv_status() {
        // pause main loop (due to overlap with torque sensor)
        TIM1->CR1 &= ~TIM_CR1_CEN;
        GPIO_SETL(A, 4, 2, 3, 5); 
        SPI1->CR1 = 0; // clear SPE
        SPI1->CR2 = (15 << SPI_CR2_DS_Pos) | SPI_CR2_FRF;   // 16 bit TI mode
        // ORDER DEPENDANCE SPE set last
        SPI1->CR1 = SPI_CR1_MSTR | (5 << SPI_CR1_BR_Pos) | SPI_CR1_SPE;    // baud = clock/64

        SPI1->DR = 1<<15; // fault status 1
        while(!(SPI1->SR & SPI_SR_RXNE));
        uint32_t reg_in = SPI1->DR;

        SPI1->DR = (1<<15) | (1<<11); // vgs status2
        while(!(SPI1->SR & SPI_SR_RXNE));
        reg_in |= SPI1->DR << 16;

        SPI1->CR1 = 0; // clear SPE
        // SPI1 CS-> gpio
        GPIO_SETL(A, 4, 1, 0, 0);
        GPIOA->BSRR = GPIO_ODR_OD4;

        // SPI1 ADS1235
        SPI1->CR1 = SPI_CR1_CPHA | SPI_CR1_MSTR | (4 << SPI_CR1_BR_Pos) | SPI_CR1_SSI | SPI_CR1_SSM | SPI_CR1_SPE;    // baud = clock/32
        SPI1->CR2 = (7 << SPI_CR2_DS_Pos) | SPI_CR2_FRXTH | SPI_CR2_RXDMAEN | SPI_CR2_TXDMAEN;    // 8 bit   

        // reenable main loop
        TIM1->CR1 = TIM_CR1_CEN;
        return reg_in;
}

void drv_reset(uint32_t blah) {
    GPIOC->BSRR = GPIO_BSRR_BR11; // drv enable
    ms_delay(10);
    GPIOC->BSRR = GPIO_BSRR_BS11; // drv enable
    ms_delay(10);
}

void system_init() {
    if (drv_regs_error) {
        System::log("drv configure failure");
    } else {
        System::log("drv configure success");
    }
    config_items.motor_pwm.init();
    if (config_items.motor_encoder.init()) {
        System::log("Motor encoder init success");
    } else {
        System::log("Motor encoder init failure");
    }
    config_items.torque_sensor.init();
    std::function<void(uint32_t)> setbct = std::bind(&MA732Encoder::set_bct, &config_items.motor_encoder, std::placeholders::_1);
    std::function<uint32_t(void)> getbct = std::bind(&MA732Encoder::get_bct, &config_items.motor_encoder);
    System::api.add_api_variable("mbct", new APICallbackUint32(getbct, setbct));

    std::function<void(uint32_t)> set_et = std::bind(&MA732Encoder::set_et, &config_items.motor_encoder, std::placeholders::_1);
    std::function<uint32_t(void)> get_et = std::bind(&MA732Encoder::get_et, &config_items.motor_encoder);
    System::api.add_api_variable("met", new APICallbackUint32(get_et, set_et));

    std::function<void(uint32_t)> set_mgt = std::bind(&MA732Encoder::set_mgt, &config_items.motor_encoder, std::placeholders::_1);
    std::function<uint32_t(void)> get_mgt = std::bind(&MA732Encoder::get_magnetic_field_strength, &config_items.motor_encoder);
    System::api.add_api_variable("mmgt", new APICallbackUint32(get_mgt, set_mgt));

    System::api.add_api_variable("drv_err", new APICallbackUint32(get_drv_status, drv_reset));
}

void system_maintenance() {}

#include "../motorlib/system.cpp"
