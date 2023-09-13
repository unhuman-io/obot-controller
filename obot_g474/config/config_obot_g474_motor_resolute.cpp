#include "../param/param_obot_g474.h"
#include "../st_device.h"
#include "../../motorlib/qep_encoder.h"
#include "../../motorlib/torque_sensor.h"
#include "../../motorlib/gpio.h"

#include "../../motorlib/peripheral/stm32g4/spi_dma.h"
//#include "../../motorlib/aksim2_encoder.h"
#include "../../motorlib/resolute_encoder.h"
#include "../../motorlib/peripheral/stm32g4/pin_config.h"

using TorqueSensor = TorqueSensorBase;
using MotorEncoder = QEPEncoder;
using OutputEncoder = ResoluteEncoder;
//using OutputEncoder = Aksim2Encoder<18>;

extern "C" void SystemClock_Config();
void pin_config_obot_g474_motor_r0();

struct InitCode {
    InitCode() {
      SystemClock_Config();
      pin_config_obot_g474_motor_r0();
      SPI1->CR2 = (7 << SPI_CR2_DS_Pos) | SPI_CR2_FRXTH;   // 8 bit
      // ORDER DEPENDANCE SPE set last
      SPI1->CR1 = SPI_CR1_MSTR | (4 << SPI_CR1_BR_Pos) | SPI_CR1_SSI | SPI_CR1_SSM | SPI_CR1_CPOL | SPI_CR1_CPHA;    // baud = clock/32
    //   DMAMUX1_Channel0->CCR =  DMA_REQUEST_SPI3_TX;
    //   DMAMUX1_Channel1->CCR =  DMA_REQUEST_SPI3_RX;
      DMAMUX1_Channel2->CCR =  DMA_REQUEST_SPI1_TX;
      DMAMUX1_Channel3->CCR =  DMA_REQUEST_SPI1_RX;
              GPIO_SETL(C, 0, GPIO_MODE::OUTPUT, GPIO_SPEED::VERY_HIGH, 0); // main loop scope
        GPIO_SETL(C, 1, GPIO_MODE::OUTPUT, GPIO_SPEED::VERY_HIGH, 0); // fast loop scope
        GPIO_SETL(C, 2, GPIO_MODE::OUTPUT, GPIO_SPEED::VERY_HIGH, 0); // usb int scope
    }
};

namespace config {
    const uint32_t main_loop_frequency = 10000;    
    const uint32_t pwm_frequency = 50000;
    InitCode init_code;

    QEPEncoder motor_encoder(*TIM2);
    TorqueSensor torque_sensor;

    GPIO output_encoder_cs(*GPIOC, 3, GPIO::OUTPUT);
    SPIDMA spi1_dma(*SPI1, output_encoder_cs, *DMA1_Channel3, *DMA1_Channel4, 100, 100, nullptr,
    SPI_CR1_MSTR | (4 << SPI_CR1_BR_Pos) | SPI_CR1_SSI | SPI_CR1_SSM | SPI_CR1_CPOL | SPI_CR1_CPHA);
    OutputEncoder output_encoder(spi1_dma);
    //Aksim2Encoder<18> output_encoder(spi1_dma);

};

#define SPI1_REINIT_CALLBACK
void spi1_reinit_callback() {
   SPI1->CR1=0;
    SPI1->CR2 = (7 << SPI_CR2_DS_Pos) | SPI_CR2_FRXTH;   // 8 bit
    // ORDER DEPENDANCE SPE set last
    SPI1->CR1 = SPI_CR1_MSTR | (4 << SPI_CR1_BR_Pos) | SPI_CR1_SSI | SPI_CR1_SSM | SPI_CR1_CPOL | SPI_CR1_CPHA;    // baud = clock/64
    config::spi1_dma.reinit();
}

#include "../../motorlib/boards/config_obot_g474_motor.cpp"

void config_init() {
    System::api.add_api_variable("index_count", new APIUint32(&config::motor_encoder.index_count_));

    config::output_encoder.spi_dma_.register_operation_ = config::drv.register_operation_;
    System::api.add_api_variable("oerr", new APIUint32(&config::output_encoder.diag_err_count_));
    System::api.add_api_variable("owarn", new APIUint32(&config::output_encoder.diag_warn_count_));
    System::api.add_api_variable("ocrc_cnt", new APIUint32(&config::output_encoder.crc_err_count_));
    System::api.add_api_variable("oraw", new APIUint32(&config::output_encoder.raw_value_));
    System::api.add_api_variable("orawh", new const APICallback([](){ return u32_to_hex(config::output_encoder.raw_value_); }));
    System::api.add_api_variable("oraw2h", new const APICallback([](){ return u32_to_hex(config::output_encoder.raw_value2_); }));
    System::api.add_api_variable("oraw3h", new const APICallback([](){ return u32_to_hex(config::output_encoder.raw_value3_); }));
    System::api.add_api_variable("oraw4h", new const APICallback([](){ return u32_to_hex(config::output_encoder.raw_value4_); }));
   System::api.add_api_variable("olen", new APIUint8(&config::output_encoder.length_));
    System::api.add_api_variable("ocrc_latch", new const APIUint32(&config::output_encoder.crc_error_raw_latch_));
    System::api.add_api_variable("oind", new const APIUint8(&config::output_encoder.byte_ind));
    System::api.add_api_variable("ocrc_calc", new const APIUint8(&config::output_encoder.crc_calc_));
    System::api.add_api_variable("ozeros", new const APIUint32(&config::output_encoder.leading_zeros));
    System::api.add_api_variable("odiag", new const APIUint8(&config::output_encoder.diag_raw_.word));

}

void config_maintenance() {}