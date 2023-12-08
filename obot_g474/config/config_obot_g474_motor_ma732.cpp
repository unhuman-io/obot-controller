#include "../param/param_obot_g474.h"
#include "st_device.h"
#include "../../motorlib/peripheral/stm32g4/spi_dma.h"
#include "../../motorlib/ma732_encoder.h"
#include "../../motorlib/torque_sensor.h"
#include "../../motorlib/gpio.h"

using TorqueSensor = TorqueSensorBase;
using MotorEncoder = MA732Encoder;
using OutputEncoder = EncoderBase;

struct InitCode {
    InitCode() {    
      SPI3->CR2 = (15 << SPI_CR2_DS_Pos);   // 16 bit
      // ORDER DEPENDANCE SPE set last
      SPI3->CR1 = SPI_CR1_MSTR | (3 << SPI_CR1_BR_Pos) | SPI_CR1_SSI | SPI_CR1_SSM | SPI_CR1_SPE;    // baud = clock/16
    }
};

namespace config {
    const uint32_t main_loop_frequency = 10000;    
    const uint32_t pwm_frequency = 50000;
    InitCode init_code;

    GPIO motor_encoder_cs(*GPIOD, 2, GPIO::OUTPUT);
    MA732Encoder motor_encoder(*SPI3, motor_encoder_cs);
    TorqueSensor torque_sensor;
    GPIO output_encoder_cs(*GPIOD, 2, GPIO::OUTPUT);
    OutputEncoder output_encoder;
};

#include "../../motorlib/boards/config_obot_g474_motor.cpp"

void config_init() {
    System::api.add_api_variable("mbct", new APICallbackUint32([](){ return config::motor_encoder.get_bct(); },
                    [](uint32_t u){ config::motor_encoder.set_bct(u); }));
    System::api.add_api_variable("met", new APICallbackUint32([](){ return config::motor_encoder.get_et(); },
                    [](uint32_t u){ config::motor_encoder.set_et(u); }));
    System::api.add_api_variable("mmgt", new APICallbackUint32([](){ return config::motor_encoder.get_magnetic_field_strength(); },
                    [](uint32_t u){ config::motor_encoder.set_mgt(u); }));
}

void config_maintenance() {}