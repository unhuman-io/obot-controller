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
#include "../motorlib/peripheral/stm32g4/spi_dma.h"
#include "../motorlib/ads1235.h"

typedef FastLoop<HRPWM, MA732Encoder> FastLoopConfig;
typedef MainLoop<FastLoopConfig> MainLoopConfig;
typedef Actuator<FastLoopConfig, MainLoopConfig> ActuatorConfig;
typedef System<ActuatorConfig, USB1> SystemConfig;

template<>
USB1 SystemConfig ::usb_ = {};
template<>
std::queue<std::string> SystemConfig ::log_queue_ = {};
template<>
ParameterAPI SystemConfig ::api = {};

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

struct InitCode {
    InitCode() {
        RCC->APB1ENR1 |= RCC_APB1ENR1_SPI3EN;
        RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;
        RCC->AHB1ENR |= RCC_AHB1ENR_DMA1EN | RCC_AHB1ENR_DMAMUX1EN;
        RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN | RCC_AHB2ENR_GPIOBEN | RCC_AHB2ENR_GPIOCEN;

        // GPIO, moder 0xABFFFFFF
        GPIOA->MODER = 0xABFF00FF | 2 << GPIO_MODER_MODE4_Pos | 2 << GPIO_MODER_MODE5_Pos | 2 << GPIO_MODER_MODE6_Pos | 2 << GPIO_MODER_MODE7_Pos;
        GPIOA->OSPEEDR = 3 << GPIO_OSPEEDR_OSPEED4_Pos | 3 << GPIO_OSPEEDR_OSPEED5_Pos | 3 << GPIO_OSPEEDR_OSPEED6_Pos | 3 << GPIO_OSPEEDR_OSPEED7_Pos;
        GPIOA->AFR[0] = 5 << GPIO_AFRL_AFSEL4_Pos | 5 << GPIO_AFRL_AFSEL5_Pos | 5 << GPIO_AFRL_AFSEL6_Pos | 5 << GPIO_AFRL_AFSEL7_Pos;
        
        // bmoder 0xFFFFFEBF
        GPIOB->MODER = 0xFFFFF03F | 2 << GPIO_MODER_MODE3_Pos | 2 << GPIO_MODER_MODE4_Pos | 2 << GPIO_MODER_MODE5_Pos;
        GPIOB->OSPEEDR = 3 << GPIO_OSPEEDR_OSPEED3_Pos | 3 << GPIO_OSPEEDR_OSPEED4_Pos | 3 << GPIO_OSPEEDR_OSPEED4_Pos;
        GPIOB->AFR[0] = 6 << GPIO_AFRL_AFSEL3_Pos | 6 << GPIO_AFRL_AFSEL4_Pos | 6 << GPIO_AFRL_AFSEL5_Pos;

        // bmoder 0xFFFFFFFF
        GPIOC->MODER = 0xFF3FFFFF | 1 << GPIO_MODER_MODE11_Pos;

        // SPI1 DRV8323RS
        GPIOC->ODR |= GPIO_ODR_OD11; // drv enable
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

        // SPI1 ADS1235
        DMAMUX1_Channel0->CCR =  DMA_REQUEST_SPI1_TX;
        DMAMUX1_Channel1->CCR =  DMA_REQUEST_SPI1_RX;
        SPI1->CR1 = SPI_CR1_CPHA | SPI_CR1_MSTR | (4 << SPI_CR1_BR_Pos) | SPI_CR1_SSI | SPI_CR1_SSM | SPI_CR1_SPE;    // baud = clock/32
        SPI1->CR2 = (7 << SPI_CR2_DS_Pos) | SPI_CR2_FRXTH;    // 8 bit   

        // SPI3 MA732
        SPI3->CR1 = SPI_CR1_MSTR | (4 << SPI_CR1_BR_Pos) | SPI_CR1_SSI | SPI_CR1_SSM | SPI_CR1_SPE;    // baud = clock/32
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
    //QEPEncoder motor_encoder = {*TIM5};
    GPIO motor_encoder_cs = {*GPIOA, 15, GPIO::OUTPUT};
    GPIO torque_sensor_cs = {*GPIOA, 15, GPIO::OUTPUT};
    SPIDMA spi_dma = {*SPI1, torque_sensor_cs, *DMA1_Channel1, *DMA1_Channel2};
    ADS1235 torque_sensor = {spi_dma};
    //GPIO torque_cs = {*GPIOA, 15, GPIO::OUTPUT};
    //SPITorque torque_sensor = {*SPI3, torque_cs, *DMA1_Channel1, *DMA1_Channel2};
    GPIO hall_a = {*GPIOC, 0, GPIO::INPUT};
    GPIO hall_b = {*GPIOC, 1, GPIO::INPUT};
    GPIO hall_c = {*GPIOC, 2, GPIO::INPUT};
    MA732Encoder motor_encoder = {*SPI3, motor_encoder_cs};
    HallEncoder output_encoder = {hall_a, hall_b, hall_c};
    //AMSEncoder motor_encoder = {*SPI3, motor_encoder_cs};
    GPIO enable = {*GPIOC, 11, GPIO::OUTPUT};
    HRPWM motor_pwm = {pwm_frequency, *HRTIM1, 3, 5, 4};
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

void system_init() {
    // if (config_items.motor_encoder.init()) {
    //     SystemConfig::log("Motor encoder init success");
    // } else {
    //     SystemConfig::log("Motor encoder init failure");
    // }
    config_items.torque_sensor.init();
    // std::function<void(uint32_t)> setbct = std::bind(&MA732Encoder::set_bct, &config_items.motor_encoder, std::placeholders::_1);
    // std::function<uint32_t(void)> getbct = std::bind(&MA732Encoder::get_bct, &config_items.motor_encoder);
    // SystemConfig::api.add_api_variable("mbct", new APICallbackUint32(getbct, setbct));

    // std::function<void(uint32_t)> set_et = std::bind(&MA732Encoder::set_et, &config_items.motor_encoder, std::placeholders::_1);
    // std::function<uint32_t(void)> get_et = std::bind(&MA732Encoder::get_et, &config_items.motor_encoder);
    // SystemConfig::api.add_api_variable("met", new APICallbackUint32(get_et, set_et));

    // std::function<void(uint32_t)> set_mgt = std::bind(&MA732Encoder::set_mgt, &config_items.motor_encoder, std::placeholders::_1);
    // std::function<uint32_t(void)> get_mgt = std::bind(&MA732Encoder::get_magnetic_field_strength, &config_items.motor_encoder);
    // SystemConfig::api.add_api_variable("mmgt", new APICallbackUint32(get_mgt, set_mgt));
}

void system_maintenance() {}

#include "../motorlib/system.cpp"
