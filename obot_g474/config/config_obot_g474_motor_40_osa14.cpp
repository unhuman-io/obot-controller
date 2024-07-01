#include "../param/param_obot_g474_motor40.h"
#include "st_device.h"
#include "../../motorlib/qep_encoder.h"
#include "../../motorlib/torque_sensor.h"
#include "../../motorlib/gpio.h"
#include "../../motorlib/peripheral/stm32g4/spi_dma.h"
#include "../../motorlib/sensors/encoders/icpz.h"
#include "../../motorlib/peripheral/stm32g4/spi_debug.h"
#include "../../motorlib/ads1235.h"

using TorqueSensor = ADS1235;
using MotorEncoder = ICPZ;
using OutputEncoder = EncoderBase;

struct InitCode {
    InitCode() {
        //SPI3 PZ
        DMAMUX1_Channel0->CCR =  DMA_REQUEST_SPI3_TX;
        DMAMUX1_Channel1->CCR =  DMA_REQUEST_SPI3_RX;
        SPI3->CR1 = SPI_CR1_MSTR | (3 << SPI_CR1_BR_Pos) | SPI_CR1_SSI | SPI_CR1_SSM;    // baud = clock/16 spi mode 0
        SPI3->CR2 = (7 << SPI_CR2_DS_Pos) | SPI_CR2_FRXTH;    // 8 bit   
 
    }
};

namespace config {
    const uint32_t main_loop_frequency = 10000;    
    const uint32_t pwm_frequency = 50000;
    InitCode init_code;

    GPIO motor_encoder_cs = {*GPIOD, 2, GPIO::OUTPUT};
    SPIDMA spi3_dma = {SPIDMA::SP3, motor_encoder_cs, DMA1_CH1, DMA1_CH2, 0};
    ICPZ motor_encoder(spi3_dma, ICPZ::PZ03S);
    SPIDebug spi_debug(spi3_dma);

    GPIO torque_sensor_cs(*GPIOA, 4, GPIO::OUTPUT);
    SPIDMA spi1_dma(SPIDMA::SP1, torque_sensor_cs, DMA1_CH3, DMA1_CH4, 0, 50, 50);
    ADS1235 torque_sensor(spi1_dma);
    
    EncoderBase output_encoder;
};

#include "../../motorlib/boards/config_obot_g474_motor_40.cpp"

void config_init() {
    System::api.add_api_variable("spi", new APICallback([](){ return config::spi_debug.read(); }, 
        [](std::string s) { config::spi_debug.write(s); }));
    ICPZ_SET_DEBUG_VARIABLES("m", System::api, config::motor_encoder);
    
    // System::api.add_api_variable("torque1", new const APIFloat(&config::torque_sensor.torque1_));
    // System::api.add_api_variable("torque2", new const APIFloat(&config::torque_sensor.torque2_));
    // System::api.add_api_variable("torque_decimation", new APIUint16(&config::torque_sensor.decimation_));
}

void config_maintenance() {}