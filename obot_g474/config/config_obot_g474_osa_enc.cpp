#include "../param/param_obot_g474_osa.h"
#include "../st_device.h"
#include "../../motorlib/peripheral/stm32g4/spi_dma.h"
#include "../../motorlib/ma732_encoder.h"
#include "../../motorlib/peripheral/stm32g4/spi_torque.h"
#include "../../motorlib/gpio.h"
#include "../../motorlib/qep_encoder.h"
#include "../../motorlib/peripheral/stm32g4/pin_config.h"

using TorqueSensor = SPITorque;
using MotorEncoder = QEPEncoder;
using OutputEncoder = MA732Encoder;

extern "C" void SystemClock_Config();
void pin_config_obot_g474_osa();

struct InitCode {
    InitCode() {
      SystemClock_Config();
      pin_config_obot_g474_osa();

      // qep encoder
      GPIO_SETL(A, 0, GPIO_MODE::ALT_FUN, GPIO_SPEED::VERY_HIGH, 1);   // QEPA TIM2
      GPIO_SETL(A, 1, GPIO_MODE::ALT_FUN, GPIO_SPEED::VERY_HIGH, 1);   // QEPB TIM2
      GPIO_SETL(A, 2, GPIO_MODE::ALT_FUN, GPIO_SPEED::VERY_HIGH, 1);   // QEPI TIM2

      // ma732 output encoder and torque sensor spi settings        
      SPI3->CR2 = (15 << SPI_CR2_DS_Pos);   // 16 bit
      // ORDER DEPENDANCE SPE set last
      SPI3->CR1 = SPI_CR1_MSTR | (3 << SPI_CR1_BR_Pos) | SPI_CR1_SSI | SPI_CR1_SSM | SPI_CR1_SPE;    // baud = clock/16
      DMAMUX1_Channel0->CCR =  DMA_REQUEST_SPI3_TX;
      DMAMUX1_Channel1->CCR =  DMA_REQUEST_SPI3_RX;
    }
};

namespace config {
    const uint32_t main_loop_frequency = 10000;    
    const uint32_t pwm_frequency = 50000;
    InitCode init_code;

    QEPEncoder motor_encoder(*TIM2);
    GPIO torque_sensor_cs(*GPIOB, 4, GPIO::OUTPUT);
    TorqueSensor torque_sensor(*SPI3, torque_sensor_cs, *DMA1_Channel1, *DMA1_Channel2);
    GPIO output_encoder_cs(*GPIOD, 2, GPIO::OUTPUT);
    MA732Encoder output_encoder(*SPI3, output_encoder_cs);
};

#include "config_obot_g474_osa.cpp"

void config_init() {
    System::api.add_api_variable("jbct", new APICallbackUint32([](){ return config::output_encoder.get_bct(); },
                    [](uint32_t u){ config::output_encoder.set_bct(u); }));
    System::api.add_api_variable("jet", new APICallbackUint32([](){ return config::output_encoder.get_et(); },
                    [](uint32_t u){ config::output_encoder.set_et(u); }));
    System::api.add_api_variable("jmgt", new APICallbackUint32([](){ return config::output_encoder.get_magnetic_field_strength(); },
                    [](uint32_t u){ config::output_encoder.set_mgt(u); }));
}

void config_maintenance() {}