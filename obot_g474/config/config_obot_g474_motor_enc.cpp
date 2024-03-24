#include "../param/param_obot_g474.h"
#include "st_device.h"
#include "../../motorlib/qep_encoder.h"
#include "../../motorlib/torque_sensor.h"
#include "../../motorlib/gpio.h"
#include "../../motorlib/ma732_encoder.h"
#include "../../motorlib/peripheral/stm32g4/pin_config.h"
#define COMMS   COMMS_USB

volatile uint16_t spi3_dr;
class OutputEncoder : public MA732Encoder {
 public:
    OutputEncoder(SPI_TypeDef& s, GPIO& g) : MA732Encoder(s, g) {}
    // bypass some issue with CI encoder filter setting
    bool init() { return true; }
    void reinit() { SPI3->CR2 = (15 << SPI_CR2_DS_Pos) | SPI_CR2_TXDMAEN | SPI_CR2_RXDMAEN;}
    void trigger() { reinit(); }
    int32_t read() {
        data_ = spi3_dr;
        count_ += (int16_t) (data_ - last_data_); // rollover summing
        last_data_ = data_;
        return count_;
    }
};

using TorqueSensor = TorqueSensorBase;
using MotorEncoder = QEPEncoder;

uint32_t dma_gpiod_reg_data[2] = {4<<16, 4};
uint16_t tmp = 0;

struct InitCode {
    InitCode() {
      SPI3->CR2 = (15 << SPI_CR2_DS_Pos) | SPI_CR2_TXDMAEN;   // 16 bit
      // ORDER DEPENDANCE SPE set last
      SPI3->CR1 = SPI_CR1_MSTR | (3 << SPI_CR1_BR_Pos) | SPI_CR1_SSI | SPI_CR1_SSM | SPI_CR1_SPE;    // baud = clock/16

      HRTIM1->sMasterRegs.MCMP2R = 100;
      HRTIM1->sMasterRegs.MCMP3R = 180;
      HRTIM1->sMasterRegs.MDIER |= HRTIM_MDIER_MCMP2DE | HRTIM_MDIER_MCMP3DE;
      DMAMUX1_Channel0->CCR = 95;

      DMA1_Channel1->CNDTR = 2;
      DMA1_Channel1->CPAR = (uint32_t)&GPIOD->BSRR;
      DMA1_Channel1->CMAR = (uint32_t)dma_gpiod_reg_data;
      DMA1_Channel1->CCR = DMA_CCR_CIRC | DMA_CCR_MINC | DMA_CCR_DIR | DMA_CCR_PSIZE_1 | DMA_CCR_MSIZE_1 | DMA_CCR_EN;


      DMAMUX1_Channel4->CCR = 2 << DMAMUX_CxCR_SYNC_ID_Pos | 0 << DMAMUX_CxCR_NBREQ_Pos | 2 << DMAMUX_CxCR_SPOL_Pos | DMAMUX_CxCR_SE | DMA_REQUEST_SPI3_TX; 
      DMA1_Channel5->CNDTR = 1;
      DMA1_Channel5->CPAR = (uint32_t)&SPI3->DR;
      DMA1_Channel5->CMAR = (uint32_t)&tmp;
      DMA1_Channel5->CCR = DMA_CCR_CIRC | DMA_CCR_MINC | DMA_CCR_DIR | DMA_CCR_PSIZE_0 | DMA_CCR_MSIZE_0 | DMA_CCR_EN;

      MASK_SET(SYSCFG->EXTICR[0], SYSCFG_EXTICR1_EXTI2, 3);              // EXTI PD2

      DMAMUX1_Channel5->CCR = DMA_REQUEST_SPI3_RX; 
      DMA1_Channel6->CNDTR = 1;
      DMA1_Channel6->CPAR = (uint32_t)&SPI3->DR;
      DMA1_Channel6->CMAR = (uint32_t)&spi3_dr;
      DMA1_Channel6->CCR = DMA_CCR_CIRC | DMA_CCR_MINC | DMA_CCR_PSIZE_0 | DMA_CCR_MSIZE_0 | DMA_CCR_EN;
    }
};

namespace config {
    const uint32_t main_loop_frequency = 10000;    
    const uint32_t pwm_frequency = 50000;
    InitCode init_code;

    QEPEncoder motor_encoder(*TIM2);
    TorqueSensor torque_sensor;
    GPIO motor_encoder_cs(*GPIOD, 2, GPIO::OUTPUT);
    OutputEncoder output_encoder(*SPI3, motor_encoder_cs);
};

#include "../../motorlib/boards/config_obot_g474_motor.cpp"

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
}

void config_maintenance() {}