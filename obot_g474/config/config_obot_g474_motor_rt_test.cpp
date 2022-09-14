#include "../param/param_obot_g474.h"
#include "../st_device.h"
#include "../../motorlib/qep_encoder.h"
#include "../../motorlib/torque_sensor.h"
#include "../../motorlib/gpio.h"
#include "../../motorlib/ma732_encoder.h"
#include "../../motorlib/peripheral/stm32g4/pin_config.h"
#include "../../motorlib/gpio_hardware_brake.h"

using TorqueSensor = TorqueSensorBase;
using MotorEncoder = QEPEncoder;
using OutputEncoder = MA732Encoder;
#define HARDWARE_BRAKE_DEFINED
using HardwareBrake = GPIOHardwareBrake;

extern "C" void SystemClock_Config();
void pin_config_obot_g474_motor_r0();

struct InitCode {
    InitCode() {
      SystemClock_Config();
      pin_config_obot_g474_motor_r0();
      SPI3->CR2 = (15 << SPI_CR2_DS_Pos);   // 16 bit
      // ORDER DEPENDANCE SPE set last
      SPI3->CR1 = SPI_CR1_MSTR | (3 << SPI_CR1_BR_Pos) | SPI_CR1_SSI | SPI_CR1_SSM | SPI_CR1_SPE;    // baud = clock/16

      GPIO_SETL(A, 1, GPIO_MODE::ALT_FUN, GPIO_SPEED::VERY_HIGH, 2);   // QEPB TIM5
      TIM5->CCER = TIM_CCER_CC2E;
      TIM5->ARR = CPU_FREQUENCY_HZ/51+1;
      TIM5->CCR2= CPU_FREQUENCY_HZ/2/51+1;
      TIM5->CCMR1 = 0b110 << TIM_CCMR1_OC2M_Pos;
      TIM5->CR1 = TIM_CR1_CEN;

      GPIO_SETL(A, 2, GPIO_MODE::OUTPUT, GPIO_SPEED::VERY_HIGH, 0);   // QEPI output
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
    GPIO hardware_brake_gpio(*GPIOA, 2, GPIO::OUTPUT);
    GPIOHardwareBrake hardware_brake(hardware_brake_gpio);
};

static uint32_t fn = 0;
#include "config_obot_g474_motor.cpp"
uint32_t get_fn() {
    
    static int32_t last_ufn = 0;
    int16_t ufn = USB->FNR & USB_FNR_FN;
    int16_t diff = ufn - last_ufn;
    last_ufn = ufn;
    if (diff < 0)
        diff += 0x800;
    fn += diff;
    return fn;
}



void config_init() {
    System::api.add_api_variable("jbct", new APICallbackUint32([](){ return config::output_encoder.get_bct(); },
                    [](uint32_t u){ config::output_encoder.set_bct(u); }));
    System::api.add_api_variable("jet", new APICallbackUint32([](){ return config::output_encoder.get_et(); },
                    [](uint32_t u){ config::output_encoder.set_et(u); }));
    System::api.add_api_variable("jmgt", new APICallbackUint32([](){ return config::output_encoder.get_magnetic_field_strength(); },
                    [](uint32_t u){ config::output_encoder.set_mgt(u); }));
    System::api.add_api_variable("jfilt", new APICallbackUint32([](){ return config::output_encoder.get_filt(); }, 
                [](uint32_t u){ config::output_encoder.set_filt(u); }));
    System::api.add_api_variable("index_count", new APIUint32(&config::motor_encoder.index_count_));
    System::api.add_api_variable("sof", new const APICallbackUint32(get_fn));
    config::main_loop.reserved1_ = &fn;
    
}

void config_maintenance() {}