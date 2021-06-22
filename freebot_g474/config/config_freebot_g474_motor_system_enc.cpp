#include "../param/param_freebot_g474.h"
#include "../st_device.h"
#include "../../motorlib/qep_encoder.h"
#include "../../motorlib/torque_sensor.h"
#include "../../motorlib/gpio.h"
#include "../../motorlib/ma732_encoder.h"

using TorqueSensor = TorqueSensorBase;
using MotorEncoder = QEPEncoder;
using OutputEncoder = MA732Encoder;

extern "C" void SystemClock_Config();
void pin_config_freebot_g474_motor_r0();

struct InitCode {
    InitCode() {
      SystemClock_Config();
      pin_config_freebot_g474_motor_r0();

      RCC->APB1ENR1 |= RCC_APB1ENR1_TIM3EN;
      // TIM3 system control interrupt        
      TIM3->ARR = CPU_FREQUENCY_HZ / 1000 - 1;    // 1000 Hz
      TIM3->DIER = TIM_DIER_UIE;
      NVIC_SetPriority(TIM3_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 3, 0));
      NVIC_EnableIRQ(TIM3_IRQn);

      SPI3->CR2 = (15 << SPI_CR2_DS_Pos);   // 16 bit
      // ORDER DEPENDANCE SPE set last
      SPI3->CR1 = SPI_CR1_MSTR | (3 << SPI_CR1_BR_Pos) | SPI_CR1_SSI | SPI_CR1_SSM | SPI_CR1_SPE;    // baud = clock/16
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
};

#include "config_freebot_g474_motor_system.cpp"

void config_init() {
    System::api.add_api_variable("jbct", new APICallbackUint32([](){ return config::output_encoder.get_bct(); },
                    [](uint32_t u){ config::output_encoder.set_bct(u); }));
    System::api.add_api_variable("jet", new APICallbackUint32([](){ return config::output_encoder.get_et(); },
                    [](uint32_t u){ config::output_encoder.set_et(u); }));
    System::api.add_api_variable("jmgt", new APICallbackUint32([](){ return config::output_encoder.get_magnetic_field_strength(); },
                    [](uint32_t u){ config::output_encoder.set_mgt(u); }));
    System::api.add_api_variable("jfilt", new APICallbackUint32([](){ return config::output_encoder.get_filt(); }, 
                [](uint32_t u){ config::output_encoder.set_filt(u); }));
}

void config_maintenance() {}