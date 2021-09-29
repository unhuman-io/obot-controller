#include "../param/param_freebot_g474.h"
#include "../st_device.h"
#include "../../motorlib/qep_encoder.h"
#include "../../motorlib/torque_sensor.h"
#include "../../motorlib/gpio.h"
#include "../../motorlib/ma732_encoder.h"
#include "../../motorlib/peripheral/stm32g4/pin_config.h"

using TorqueSensor = TorqueSensorBase;
using MotorEncoder = QEPEncoder;
using OutputEncoder = MA732Encoder;

extern "C" void SystemClock_Config();
void pin_config_freebot_g474_motor_r0();

const uint16_t program_frequency = 1000;

struct InitCode {
    InitCode() {
      SystemClock_Config();
      pin_config_freebot_g474_motor_r0();
      SPI3->CR2 = (15 << SPI_CR2_DS_Pos);   // 16 bit
      // ORDER DEPENDANCE SPE set last
      SPI3->CR1 = SPI_CR1_MSTR | (3 << SPI_CR1_BR_Pos) | SPI_CR1_SSI | SPI_CR1_SSM | SPI_CR1_SPE;    // baud = clock/16

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
    TorqueSensor torque_sensor;
    GPIO motor_encoder_cs(*GPIOD, 2, GPIO::OUTPUT);
    MA732Encoder output_encoder(*SPI3, motor_encoder_cs);

    GPIODebounce gpio1(*GPIOC, 0);
    GPIODebounce gpio2(*GPIOC, 1);
};

#include "config_freebot_g474_motor.cpp"

void config_init() {
    System::api.add_api_variable("jbct", new APICallbackUint32([](){ return config::output_encoder.get_bct(); },
                    [](uint32_t u){ config::output_encoder.set_bct(u); }));
    System::api.add_api_variable("jet", new APICallbackUint32([](){ return config::output_encoder.get_et(); },
                    [](uint32_t u){ config::output_encoder.set_et(u); }));
    System::api.add_api_variable("jmgt", new APICallbackUint32([](){ return config::output_encoder.get_magnetic_field_strength(); },
                    [](uint32_t u){ config::output_encoder.set_mgt(u); }));
    System::api.add_api_variable("jfilt", new APICallbackUint32([](){ return config::output_encoder.get_filt(); }, 
                [](uint32_t u){ config::output_encoder.set_filt(u); }));
    TIM3->CR1 = TIM_CR1_CEN; // start TIM3 program           
}

void config_maintenance() {}

#include "../tension_program.h"
namespace config {
    TensionProgram tension_program;
};

extern "C" { void TIM3_IRQHandler(); }
void TIM3_IRQHandler() {
    config::tension_program.loop();
    TIM3->SR = 0;
}