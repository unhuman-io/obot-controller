#include "../param/param_obot_g474_osa.h"
#include "st_device.h"
#include "../../motorlib/peripheral/stm32g4/spi_dma.h"
#include "../../motorlib/sensors/encoders/ma7xx_encoder.h"
#include "../../motorlib/sensors/encoders/ma782_encoder.h"
#include "../../motorlib/peripheral/stm32g4/spi_torque.h"
#include "../../motorlib/gpio.h"
#include "../../motorlib/sensor_multiplex.h"
#include "../../motorlib/peripheral/stm32g4/pin_config.h"

using TorqueSensor = TorqueSensorMultiplex<SPITorque, MA782Encoder>;
using MotorEncoder = MA782Encoder;
using OutputEncoder = TorqueSensorMultiplex<SPITorque, MA782Encoder>::SecondarySensor;

const uint16_t program_frequency = 1000;

struct InitCode {
    InitCode() {
      // dma setup for spi torque
      DMAMUX1_Channel0->CCR =  DMA_REQUEST_SPI3_TX;
      DMAMUX1_Channel1->CCR =  DMA_REQUEST_SPI3_RX;

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
        GPIO_SETL(A, 0, GPIO_MODE::INPUT, GPIO_SPEED::VERY_HIGH, 0); // gpio 1
        MASK_SET(GPIOA->PUPDR, GPIO_PUPDR_PUPD0, 1); // pull up
        GPIO_SETL(A, 1, GPIO_MODE::OUTPUT, GPIO_SPEED::VERY_HIGH, 0); // gpio 2
        MASK_SET(GPIOA->PUPDR, GPIO_PUPDR_PUPD1, 1); // pull up
        GPIOA->ODR = 0;
    }
};

namespace config {
    const uint32_t main_loop_frequency = 10000;    
    const uint32_t pwm_frequency = 50000;
    InitCode init_code;

    GPIO motor_encoder_cs(*GPIOA, 4, GPIO::OUTPUT);
    MA782Encoder motor_encoder(*SPI1, motor_encoder_cs);
    GPIO torque_sensor_cs(*GPIOD, 2, GPIO::OUTPUT); // spi3 cs1
    SPITorque torque_sensor_direct(*SPI3, torque_sensor_cs, *DMA1_Channel1, *DMA1_Channel2, SPIDMA::spi_pause[SPIDMA::SP3], 0);
    GPIO output_encoder_cs(*GPIOB, 4, GPIO::OUTPUT); // spi3 cs2
    MA782Encoder output_encoder_direct(*SPI3, output_encoder_cs, SPIDMA::spi_pause[SPIDMA::SP3], 119);
    TorqueSensorMultiplex<SPITorque, MA782Encoder> torque_sensor(torque_sensor_direct, output_encoder_direct, 5);
    OutputEncoder &output_encoder = torque_sensor.secondary();

    GPIODebounce gpio2(*GPIOA, 0);
    GPIODebounce gpio1(*GPIOA, 1);
};

#include "../../motorlib/boards/config_obot_g474_osa.cpp"

void config_init() {
    System::api.add_api_variable<APICallbackUint32>("mbct",[](){ return config::motor_encoder.get_bct(); },
                    [](uint32_t u){ config::motor_encoder.set_bct(u); });
    System::api.add_api_variable<APICallbackUint32>("met",[](){ return config::motor_encoder.get_et(); },
                    [](uint32_t u){ config::motor_encoder.set_et(u); });
    System::api.add_api_variable<APICallbackUint32>("mmgt",[](){ return config::motor_encoder.get_magnetic_field_strength(); },
                    [](uint32_t u){ config::motor_encoder.set_mgt(u); });
    System::api.add_api_variable<APICallbackUint32>("jbct",[](){ return config::output_encoder_direct.get_bct(); },
                    [](uint32_t u){ config::motor_encoder.set_bct(u); });
    System::api.add_api_variable<APICallbackUint32>("jet",[](){ return config::output_encoder_direct.get_et(); },
                    [](uint32_t u){ config::motor_encoder.set_et(u); });
    System::api.add_api_variable<APICallbackUint32>("jmgt",[](){ return config::output_encoder_direct.get_magnetic_field_strength(); },
                    [](uint32_t u){ config::motor_encoder.set_mgt(u); });
    System::api.add_api_variable<const APIUint32>("C1", &config::torque_sensor_direct.result0_);
    System::api.add_api_variable<const APIUint32>("C2", &config::torque_sensor_direct.result1_);
    System::api.add_api_variable<const APICallback>("gpio",[]() { 
        return "gpio 1: " + std::to_string(config::gpio1.is_set()) + "\tgpio 2: " + std::to_string(config::gpio2.is_set());
    });
    TIM3->CR1 = TIM_CR1_CEN; // start TIM3 program      
}

void config_maintenance() {}

#include "../tension_program.h"
namespace config {
    TensionProgram tension_program;
};

struct InitCode2 {
    InitCode2() {    
        System::api.add_api_variable<const APICallback>("state",[](){ return config::tension_program.get_state(); });
        System::api.add_api_variable<APIFloat>("start_velocity", &config::tension_program.start_velocity);
        System::api.add_api_variable<APIFloat>("low_velocity", &config::tension_program.low_velocity);
        System::api.add_api_variable<APIFloat>("torque_desired", &config::tension_program.torque_desired);
        System::api.add_api_variable<APIFloat>("start_torque", &config::tension_program.start_torque);
        System::api.add_api_variable<APIFloat>("dithering_torque", &config::tension_program.dithering_torque);
        System::api.add_api_variable<APIFloat>("dithering_frequency_hz", &config::tension_program.dithering_frequency_hz);
        System::api.add_api_variable<APIFloat>("torque_vs_position_ramp", &config::tension_program.torque_vs_position_ramp);
        System::api.add_api_variable<APIFloat>("min_torque_desired", &config::tension_program.min_torque_desired);
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