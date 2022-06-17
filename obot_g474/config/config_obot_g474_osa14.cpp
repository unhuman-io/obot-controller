#include "../param/param_obot_g474_osa.h"
#include "../st_device.h"
#include "../../motorlib/peripheral/stm32g4/spi_dma.h"
#include "../../motorlib/ma732_encoder.h"
#include "../../motorlib/ma782_encoder.h"
#include "../../motorlib/peripheral/stm32g4/spi_torque.h"
#include "../../motorlib/gpio.h"
#include "../../motorlib/sensor_multiplex.h"

using TorqueSensor = TorqueSensorMultiplex<SPITorque, MA782Encoder>;
using MotorEncoder = MA782Encoder;
using OutputEncoder = TorqueSensorMultiplex<SPITorque, MA782Encoder>::SecondarySensor;

extern "C" void SystemClock_Config();
void pin_config_obot_g474_osa();

struct InitCode {
    InitCode() {
      SystemClock_Config();
      pin_config_obot_g474_osa();

      // dma setup for spi torque
      DMAMUX1_Channel0->CCR =  DMA_REQUEST_SPI3_TX;
      DMAMUX1_Channel1->CCR =  DMA_REQUEST_SPI3_RX;
    }
};

namespace config {
    const uint32_t main_loop_frequency = 10000;    
    const uint32_t pwm_frequency = 50000;
    InitCode init_code;

    GPIO motor_encoder_cs(*GPIOA, 4, GPIO::OUTPUT);
    MA782Encoder motor_encoder(*SPI1, motor_encoder_cs);
    volatile int spi3_register_operation = 0;
    GPIO torque_sensor_cs(*GPIOD, 2, GPIO::OUTPUT); // spi3 cs1
    SPITorque torque_sensor_direct(*SPI3, torque_sensor_cs, *DMA1_Channel1, *DMA1_Channel2, 0, &spi3_register_operation);
    GPIO output_encoder_cs(*GPIOB, 4, GPIO::OUTPUT); // spi3 cs2
    MA782Encoder output_encoder_direct(*SPI3, output_encoder_cs, 119, &spi3_register_operation);
    TorqueSensorMultiplex<SPITorque, MA782Encoder> torque_sensor(torque_sensor_direct, output_encoder_direct, 10);
    OutputEncoder &output_encoder = torque_sensor.secondary();
};

#include "config_obot_g474_osa.cpp"

void config_init() {
    System::api.add_api_variable("mbct", new APICallbackUint32([](){ return config::motor_encoder.get_bct(); },
                    [](uint32_t u){ config::motor_encoder.set_bct(u); }));
    System::api.add_api_variable("met", new APICallbackUint32([](){ return config::motor_encoder.get_et(); },
                    [](uint32_t u){ config::motor_encoder.set_et(u); }));
    System::api.add_api_variable("mmgt", new APICallbackUint32([](){ return config::motor_encoder.get_magnetic_field_strength(); },
                    [](uint32_t u){ config::motor_encoder.set_mgt(u); }));
    System::api.add_api_variable("jbct", new APICallbackUint32([](){ return config::output_encoder_direct.get_bct(); },
                    [](uint32_t u){ config::motor_encoder.set_bct(u); }));
    System::api.add_api_variable("jet", new APICallbackUint32([](){ return config::output_encoder_direct.get_et(); },
                    [](uint32_t u){ config::motor_encoder.set_et(u); }));
    System::api.add_api_variable("jmgt", new APICallbackUint32([](){ return config::output_encoder_direct.get_magnetic_field_strength(); },
                    [](uint32_t u){ config::motor_encoder.set_mgt(u); }));

    System::api.add_api_variable("C1", new const APIUint32(&config::torque_sensor_direct.result0_));
    System::api.add_api_variable("C2", new const APIUint32(&config::torque_sensor_direct.result1_));     
}

void config_maintenance() {}