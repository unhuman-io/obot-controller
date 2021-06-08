#include "../motorlib/system_log.h"
#include "../motorlib/peripheral/usb.h"
#include "../motorlib/usb_communication.h"
#include "st_device.h"
#include "../motorlib/peripheral/stm32g4/hrpwm.h"
#include "../motorlib/led.h"
#include "../motorlib/qep_encoder.h"
#include "../motorlib/torque_sensor.h"
#include "Inc/main.h"
#include "param_g474_boost.h"
#include "../motorlib/peripheral/stm32g4/spi_dma.h"
#include "../motorlib/ads1235.h"

#include "../motorlib/controller/position_controller.h"
#include "../motorlib/controller/torque_controller.h"
#include "../motorlib/controller/impedance_controller.h"
#include "../motorlib/controller/velocity_controller.h"
typedef TorqueSensorBase TorqueSensor;
typedef HRPWM PWM;
typedef EncoderBase OutputEncoder;
typedef QEPEncoder MotorEncoder;
typedef USBCommunication Communication;
#include "../motorlib/fast_loop.h"
#include "../motorlib/main_loop.h"
#include "../motorlib/actuator.h"
#include "../motorlib/system.h"

USB1 usb1;
Communication System::communication_ = {usb1};
void usb_interrupt() {
    usb1.interrupt();
}


#define MASK_SET(var, item, val) var = (var & ~item##_Msk) | (val << item##_Pos)
#define GPIO_SETL(gpio, pin, mode, speed, af) \
    MASK_SET(GPIO##gpio->MODER, GPIO_MODER_MODE##pin, mode); \
    MASK_SET(GPIO##gpio->OSPEEDR, GPIO_OSPEEDR_OSPEED##pin, speed); \
    MASK_SET(GPIO##gpio->AFR[0], GPIO_AFRL_AFSEL##pin, af)

#define GPIO_SETH(gpio, pin, mode, speed, af) \
    MASK_SET(GPIO##gpio->MODER, GPIO_MODER_MODE##pin, mode); \
    MASK_SET(GPIO##gpio->OSPEEDR, GPIO_OSPEEDR_OSPEED##pin, speed); \
    MASK_SET(GPIO##gpio->AFR[1], GPIO_AFRH_AFSEL##pin, af)

extern "C" void SystemClock_Config();

volatile uint32_t * const cpu_clock = &TIM2->CNT;
struct InitCode {
    InitCode() {
        SystemClock_Config();
        // Peripheral clock enable
        RCC->APB1ENR1 = RCC_APB1ENR1_SPI3EN | RCC_APB1ENR1_TIM2EN | RCC_APB1ENR1_TIM5EN;
        RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;
        RCC->AHB1ENR |= RCC_AHB1ENR_DMA1EN | RCC_AHB1ENR_DMAMUX1EN;
        RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN | RCC_AHB2ENR_GPIOBEN | RCC_AHB2ENR_GPIOCEN;

        // GPIO configure
        GPIO_SETL(A, 0, 2, 3, 2);   // QEPA TIM5
        GPIO_SETL(A, 1, 2, 3, 2);   // QEPB TIM5
        GPIO_SETL(A, 2, 2, 3, 2);   // QEPI TIM5
     
        GPIO_SETL(B, 3, 2, 3, 6);   // SPI3 CLK
        GPIO_SETL(B, 4, 2, 3, 6);   // SPI3 DDO
        GPIO_SETL(B, 5, 2, 3, 6);   // SPI3 DDI 

        GPIO_SETH(C, 11, 1, 0, 0);  // Boostxl enable

        // TIM2 cpu clock
        TIM2->CR1 = TIM_CR1_CEN;

        // TIM5 configure

        // SPI1 DRV8323RS
        GPIOC->BSRR = GPIO_ODR_OD11; // drv enable

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
    InitCode init_code;
    uint32_t pwm_frequency = (double) CPU_FREQUENCY_HZ * 32.0 / (hrperiod);
    uint32_t main_loop_frequency = (double) CPU_FREQUENCY_HZ/(main_loop_period);
    GPIO motor_encoder_cs = {*GPIOA, 15, GPIO::OUTPUT};
    GPIO torque_sensor_cs = {*GPIOA, 0, GPIO::OUTPUT};
    SPIDMA spi_dma = {*SPI1, torque_sensor_cs, *DMA1_Channel1, *DMA1_Channel2};
    TorqueSensor torque_sensor;
    GPIO hall_a = {*GPIOC, 0, GPIO::INPUT};
    GPIO hall_b = {*GPIOC, 1, GPIO::INPUT};
    GPIO hall_c = {*GPIOC, 2, GPIO::INPUT};
    QEPEncoder motor_encoder = {*TIM5};
    EncoderBase output_encoder;
    GPIO enable = {*GPIOC, 11, GPIO::OUTPUT};
    HRPWM motor_pwm{pwm_frequency, *HRTIM1, 3, 5, 4, false, 50};
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

void system_init() {

    config_items.motor_pwm.init();
 

    System::api.add_api_variable("qepi", new APIUint32((uint32_t *) &TIM5->CCR3));
}

void system_maintenance() {}

#include "../motorlib/system.cpp"
