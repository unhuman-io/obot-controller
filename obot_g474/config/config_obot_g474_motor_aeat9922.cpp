#include "../param/param_obot_g474.h"
#include "st_device.h"
#include "../../motorlib/torque_sensor.h"
#include "../../motorlib/gpio.h"
#include "../../motorlib/peripheral/stm32g4/spi_dma.h"
#include "../../motorlib/sensors/encoders/aeat9922.h"
#include "../../motorlib/peripheral/stm32g4/pin_config.h"
#include "../../motorlib/ltc4332.h"
#define COMMS   COMMS_USB

using TorqueSensor = TorqueSensorBase;
using OutputEncoder = AEAT9922<>;
using MotorEncoder = EncoderBase;

struct InitCode {
    InitCode() {
        //SPI3 AEAT9922
        DMAMUX1_Channel0->CCR =  DMA_REQUEST_SPI1_TX;
        DMAMUX1_Channel1->CCR =  DMA_REQUEST_SPI1_RX;
        SPI1->CR1 = SPI_CR1_MSTR | (6 << SPI_CR1_BR_Pos) | SPI_CR1_SSI | SPI_CR1_SSM;    // baud = clock/128
        SPI1->CR2 = (7 << SPI_CR2_DS_Pos) | SPI_CR2_FRXTH;    // 8 bit   
 
      // INTERFACE_BBL
      // PA0 TCS
      // PC12 TTCS (torque temperature CS)
      GPIO_SETL(A, 0, GPIO_MODE::OUTPUT, GPIO_SPEED::MEDIUM, 0); 
      GPIOA->BSRR = GPIO_BSRR_BS0;

      GPIO_SETH(C, 12, GPIO_MODE::OUTPUT, GPIO_SPEED::MEDIUM, 0); 
      GPIOC->BSRR = GPIO_BSRR_BS12;

      // JOINT_ENCODER_BITS
      GPIO_SETL(C, 2, GPIO_MODE::OUTPUT, GPIO_SPEED::MEDIUM, 0); // A3 used as joint encoder cs
      GPIOC->BSRR = GPIO_BSRR_BS2;

      // OUTPUT ENCODER CS
      GPIOC->BSRR = GPIO_BSRR_BS3;

      // LTC4332 CS
      GPIO_SETL(B, 4, GPIO_MODE::OUTPUT, GPIO_SPEED::MEDIUM, 0);
      GPIOB->BSRR = GPIO_BSRR_BS4; // PB4 (header 4 short to T3 on interface bbl)
    }
};

namespace config {
    const uint32_t main_loop_frequency = 10000;    
    const uint32_t pwm_frequency = 50000;
    InitCode init_code;

    GPIO ltc4332_cs = {*GPIOB, 4, GPIO::OUTPUT};
    SPIDMA spi_dma_ltc4332(*SPI1, ltc4332_cs, *DMA1_Channel1, *DMA1_Channel2, 2000);
    LTC4332 ltc4332(spi_dma_ltc4332);
    GPIO output_encoder_cs = {*GPIOA, 0, GPIO::OUTPUT};
    SPIDMA spi_dma = {*SPI1, output_encoder_cs, *DMA1_Channel1, *DMA1_Channel2, 
        2000, 350, nullptr, SPI_CR1_MSTR | (6 << SPI_CR1_BR_Pos) | SPI_CR1_SSI | SPI_CR1_SSM | SPI_CR1_CPHA};
    AEAT9922<> output_encoder(spi_dma);
    
    TorqueSensor torque_sensor;
    EncoderBase motor_encoder;
};

#include "../../motorlib/boards/config_obot_g474_motor.cpp"

void config_init() {
    System::api.add_api_variable("oraw", new const APIUint32(&config::output_encoder.raw_value_));
    System::api.add_api_variable("oerr_reg", new const APICallbackHex<uint8_t>([](){ return config::output_encoder.get_error_register(); }));
    System::api.add_api_variable("oerr", new const APIUint32(&config::output_encoder.error_count_));
    System::api.add_api_variable("ocrc_cnt", new const APIUint32(&config::output_encoder.crc_error_count_));
    System::api.add_api_variable("odebug_parity", new const APICallback([]() {
        uint32_t raw = config::output_encoder.raw_value_;
        char c[100];
        c[99] = 0;
        snprintf(c, 99, "raw: %03lx, bit_count: %d, parity: %d", raw, config::output_encoder.bit_sum(raw), config::output_encoder.get_parity(raw));
        std::string s(c);
        return s;
    }));
}

void config_maintenance() {}