#include "../param/param_obot_g474.h"
#include "st_device.h"
#include "../../motorlib/qep_encoder.h"
#include "../../motorlib/torque_sensor.h"
#include "../../motorlib/gpio.h"
#include "../../motorlib/ma732_encoder.h"
#include "../../motorlib/peripheral/stm32g4/pin_config.h"
#include "../../motorlib/peripheral/stm32g4/spi_dma.h"
#define COMMS   COMMS_USB

class OutputEncoder : public MA732Encoder {
 public:
    OutputEncoder(SPI_TypeDef& s, GPIO& g, SPIPause &sp) : MA732Encoder(s, g, sp) {}
    // bypass some issue with CI encoder filter setting
    bool init() { return true; }
};

using TorqueSensor = TorqueSensorBase;
using MotorEncoder = QEPEncoder;

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

    QEPEncoder motor_encoder(*TIM2);
    TorqueSensor torque_sensor;
    GPIO motor_encoder_cs(*GPIOD, 2, GPIO::OUTPUT);
    OutputEncoder output_encoder(*SPI3, motor_encoder_cs, SPIDMA::spi_pause[SPIDMA::SP3]);
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