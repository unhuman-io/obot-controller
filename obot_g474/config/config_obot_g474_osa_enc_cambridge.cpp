#include "../param/param_obot_g474_osa.h"
#include "../st_device.h"
#include "../../motorlib/peripheral/stm32g4/spi_dma.h"
#include "../../motorlib/cambridge_ic.h"
#include "../../motorlib/peripheral/stm32g4/spi_torque.h"
#include "../../motorlib/gpio.h"
#include "../../motorlib/qep_encoder.h"
#include "../../motorlib/peripheral/stm32g4/pin_config.h"

using TorqueSensor = TorqueSensorBase;
using MotorEncoder = QEPEncoder;
using OutputEncoder = CambridgeIC;

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
      MASK_SET(GPIOA->PUPDR, GPIO_PUPDR_PUPD0, GPIO_PULL::DOWN);       // reject noise with pull down
      MASK_SET(GPIOA->PUPDR, GPIO_PUPDR_PUPD1, GPIO_PULL::DOWN);
      MASK_SET(GPIOA->PUPDR, GPIO_PUPDR_PUPD2, GPIO_PULL::DOWN);


      // cambridge ic spi settings        
      SPI3->CR2 = (7 << SPI_CR2_DS_Pos);   // 8 bit
      // ORDER DEPENDANCE SPE set last
      SPI3->CR1 = SPI_CR1_MSTR | (3 << SPI_CR1_BR_Pos) | SPI_CR1_SSI | SPI_CR1_SSM | SPI_CR1_SPE;    // baud = clock/16, mode 0
      DMAMUX1_Channel0->CCR =  DMA_REQUEST_SPI3_TX;
      DMAMUX1_Channel1->CCR =  DMA_REQUEST_SPI3_RX;
    }
};

namespace config {
    const uint32_t main_loop_frequency = 10000;    
    const uint32_t pwm_frequency = 50000;
    InitCode init_code;

    QEPEncoder motor_encoder(*TIM2);
    TorqueSensor torque_sensor;
    GPIO output_encoder_cs(*GPIOD, 2, GPIO::OUTPUT);
    SPIDMA spi3(*SPI3, output_encoder_cs, *DMA1_Channel1, *DMA1_Channel2, 120, 90);
    CambridgeIC output_encoder(spi3);
};

#include "config_obot_g474_osa.cpp"

void config_init() {

}

void config_maintenance() {}