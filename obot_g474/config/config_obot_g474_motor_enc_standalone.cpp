#include "../param/param_obot_g474.h"
#include "../st_device.h"
#include "../../motorlib/qep_encoder.h"
#include "../../motorlib/torque_sensor.h"
#include "../../motorlib/gpio.h"
#include "../../motorlib/ma732_encoder.h"
#include "../../motorlib/peripheral/stm32g4/pin_config.h"
#include "../../motorlib/peripheral/stm32g4/spi_dma.h"
#include "../../motorlib/ads1235_2.h"


using TorqueSensor = ADS1235_2;
using MotorEncoder = QEPEncoder;
using OutputEncoder = EncoderBase;

extern "C" void SystemClock_Config();
void pin_config_obot_g474_motor_r0();

const uint16_t program_frequency = 1000;

struct InitCode {
    InitCode() {
      SystemClock_Config();
      pin_config_obot_g474_motor_r0();

        //SPI3 ADS1235
        DMAMUX1_Channel0->CCR =  DMA_REQUEST_SPI3_TX;
        DMAMUX1_Channel1->CCR =  DMA_REQUEST_SPI3_RX;
        SPI3->CR1 = SPI_CR1_CPHA | SPI_CR1_MSTR | (4 << SPI_CR1_BR_Pos) | SPI_CR1_SSI | SPI_CR1_SSM | SPI_CR1_SPE;    // baud = clock/32
        SPI3->CR2 = (7 << SPI_CR2_DS_Pos) | SPI_CR2_FRXTH;    // 8 bit   

        // set up ~1 kHz interrupt on TIM3
        // priority 3 is lower than all others
        RCC->APB1ENR1 |= RCC_APB1ENR1_TIM3EN;
        const uint16_t prescale = 4;
        static_assert(CPU_FREQUENCY_HZ / program_frequency / prescale < 65536, "Program frequency too low");
        TIM3->ARR = CPU_FREQUENCY_HZ / program_frequency / prescale - 1;
        TIM3->DIER = TIM_DIER_UIE;
        TIM3->PSC = prescale - 1;
        NVIC_SetPriority(TIM3_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 3, 0));
        NVIC_EnableIRQ(TIM3_IRQn);

        // gpio
        GPIO_SETL(C, 0, GPIO_MODE::INPUT, GPIO_SPEED::VERY_HIGH, 0); // gpio 1
        MASK_SET(GPIOC->PUPDR, GPIO_PUPDR_PUPD0, 1); // pull up
        GPIO_SETL(C, 1, GPIO_MODE::INPUT, GPIO_SPEED::VERY_HIGH, 0); // gpio 2
        MASK_SET(GPIOC->PUPDR, GPIO_PUPDR_PUPD1, 1); // pull up
    }
};

namespace config {
    const uint32_t main_loop_frequency = 10000;    
    const uint32_t pwm_frequency = 50000;
    InitCode init_code;

    QEPEncoder motor_encoder(*TIM2);

    GPIO torque_sensor_cs(*GPIOD, 2, GPIO::OUTPUT);
    SPIDMA spi_dma(*SPI3, torque_sensor_cs, *DMA1_Channel1, *DMA1_Channel2, 1000, 1000);
    ADS1235_2 torque_sensor(spi_dma);
    OutputEncoder output_encoder;

    GPIODebounce gpio1(*GPIOC, 0);
    GPIODebounce gpio2(*GPIOC, 1);
};

#include "config_obot_g474_motor.cpp"

void config_init() {
    System::log("torque_sensor_init: " + std::to_string(config::torque_sensor.init()));

    System::api.add_api_variable("torque1", new const APIFloat(&config::torque_sensor.torque1_));
    System::api.add_api_variable("torque2", new const APIFloat(&config::torque_sensor.torque2_));
    System::api.add_api_variable("decimation", new APIUint16(&config::torque_sensor.decimation_));
    System::api.add_api_variable("gpio", new const APICallback([]() { 
        return "gpio 1: " + std::to_string(config::gpio1.is_set()) + "\tgpio 2: " + std::to_string(config::gpio2.is_set());
    }));
    TIM3->CR1 = TIM_CR1_CEN; // start TIM3 program           
}

void config_maintenance() {}

#include "../tension_program.h"
namespace config {
    TensionProgram tension_program;
};

struct InitCode2 {
    InitCode2() {    System::api.add_api_variable("state", new const APICallback([](){ return config::tension_program.get_state(); }));
     }
};

namespace config {
    InitCode2 init_code2;
};

extern "C" { void TIM3_IRQHandler(); }
void TIM3_IRQHandler() {
    config::tension_program.loop();
    TIM3->SR = 0;
}