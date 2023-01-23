#include "../param/param_obot_g474_motor40.h"
#include "../st_device.h"
#include "../../motorlib/qep_encoder.h"
#include "../../motorlib/torque_sensor.h"
#include "../../motorlib/gpio.h"
#include "../../motorlib/peripheral/stm32g4/spi_dma.h"
#include "../../motorlib/icpz.h"
#include "../../motorlib/peripheral/stm32g4/spi_debug.h"
#include "../../motorlib/ads1235.h"

using TorqueSensor = ADS1235;
using MotorEncoder = ICPZ;
using OutputEncoder = EncoderBase;

extern "C" void SystemClock_Config();
void pin_config_obot_g474_motor_40();

struct InitCode {
    InitCode() {
      SystemClock_Config();
      pin_config_obot_g474_motor_40();


        // SPI1 ADS1235_2
        DMAMUX1_Channel2->CCR =  DMA_REQUEST_SPI1_TX;
        DMAMUX1_Channel3->CCR =  DMA_REQUEST_SPI1_RX;
        SPI1->CR1 = SPI_CR1_CPHA | SPI_CR1_MSTR | (2 << SPI_CR1_BR_Pos) | SPI_CR1_SSI | SPI_CR1_SSM | SPI_CR1_SPE;    // baud = clock/8 (ads1235 can do up to 52.6 MHz)
        SPI1->CR2 = (7 << SPI_CR2_DS_Pos) | SPI_CR2_FRXTH;    // 8 bit  
    }
};

namespace config {
    const uint32_t main_loop_frequency = 10000;    
    const uint32_t pwm_frequency = 50000;
    InitCode init_code;

    GPIO motor_encoder_cs = {*GPIOD, 2, GPIO::OUTPUT};
    SPIDMA spi_dma = {*SPI3, motor_encoder_cs, *DMA1_Channel1, *DMA1_Channel2};
    ICPZ motor_encoder(spi_dma, ICPZ::PZ03S);
    SPIDebug spi_debug(spi_dma);

    GPIO torque_sensor_cs(*GPIOA, 4, GPIO::OUTPUT);
    SPIDMA spi_dma2(*SPI1, torque_sensor_cs, *DMA1_Channel3, *DMA1_Channel4, 50, 50);
    ADS1235 torque_sensor(spi_dma2);
    
    EncoderBase output_encoder;
};

#include "../../motorlib/boards/config_obot_g474_motor_40.cpp"

void config_init() {
    System::api.add_api_variable("spi", new APICallback([](){ return config::spi_debug.read(); }, 
        [](std::string s) { config::spi_debug.write(s); }));
    
    // System::api.add_api_variable("torque1", new const APIFloat(&config::torque_sensor.torque1_));
    // System::api.add_api_variable("torque2", new const APIFloat(&config::torque_sensor.torque2_));
    // System::api.add_api_variable("torque_decimation", new APIUint16(&config::torque_sensor.decimation_));
}

void config_maintenance() {}