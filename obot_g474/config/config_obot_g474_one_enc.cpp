//dual flex tony 20240704
#include "../param/param_obot_g474_osa.h"
#include "st_device.h"
#include "../../motorlib/peripheral/stm32g4/spi_dma.h"
#include "../../motorlib/ma732_encoder.h"
#include "../../motorlib/ma782_encoder.h"
#include "../../motorlib/peripheral/stm32g4/spi_torque.h"
#include "../../motorlib/gpio.h"
#include "../../motorlib/sensor_multiplex.h"
#include "../../motorlib/bmi270.h"
#include "../../motorlib/peripheral/stm32g4/pin_config.h"

#define DISABLE_CALIBRATION

using TorqueSensor = SPITorque;
using MotorEncoder = SensorMultiplex<MA782Encoder,MA782Encoder>;
using OutputEncoder = SensorMultiplex<MA782Encoder, MA782Encoder>::SecondarySensor;

struct InitCode {
    InitCode() {
      // dma setup for spi torque
      DMAMUX1_Channel0->CCR =  DMA_REQUEST_SPI3_TX;
      DMAMUX1_Channel1->CCR =  DMA_REQUEST_SPI3_RX;

      // configure gpio CS pins as outputs
      GPIO_SETL(A, 4, GPIO_MODE::OUTPUT, GPIO_SPEED::MEDIUM, 0);   // SPI1 CS1
      GPIO_SETL(B, 0, GPIO_MODE::OUTPUT, GPIO_SPEED::MEDIUM, 0);   // SPI1 CS2
      GPIO_SETL(D, 2, GPIO_MODE::OUTPUT, GPIO_SPEED::MEDIUM, 0);   // SPI3 CS1
      GPIO_SETL(B, 4, GPIO_MODE::OUTPUT, GPIO_SPEED::MEDIUM, 0);   // SPI3 CS2
      GPIOA->BSRR = GPIO_BSRR_BS4;
      GPIOB->BSRR = GPIO_BSRR_BS0 | GPIO_BSRR_BS4;
      GPIOD->BSRR = GPIO_BSRR_BS2;
    }
};

namespace config {
    const uint32_t main_loop_frequency = 10000;    
    const uint32_t pwm_frequency = 50000;
    InitCode init_code;

    GPIO motor_encoder_cs(*GPIOB, 0, GPIO::OUTPUT);//(*GPIOA, 4, GPIO::OUTPUT) = SPI1_CS1,      (*GPIOB, 0, GPIO::OUTPUT) = SPI1_CS2      TWO ENCODERS
    //GPIO motor_encoder_cs(*GPIOA, 4, GPIO::OUTPUT);//(*GPIOA, 4, GPIO::OUTPUT) = SPI1_CS1,      (*GPIOB, 0, GPIO::OUTPUT) = SPI1_CS2        ONE ENCODER
    MA782Encoder motor_encoder_direct(*SPI1, motor_encoder_cs, SPIDMA::spi_pause[SPIDMA::SP1]);
    GPIO output_encoder_cs(*GPIOA, 4, GPIO::OUTPUT);//(*GPIOA, 4, GPIO::OUTPUT) = SPI1_CS1,      (*GPIOB, 0, GPIO::OUTPUT) = SPI1_CS2     TWO ENCODERS
    //GPIO output_encoder_cs(*GPIOB, 0, GPIO::OUTPUT);//(*GPIOA, 4, GPIO::OUTPUT) = SPI1_CS1,      (*GPIOB, 0, GPIO::OUTPUT) = SPI1_CS2       ONE ENCODER
    MA782Encoder output_encoder_direct(*SPI1, output_encoder_cs, SPIDMA::spi_pause[SPIDMA::SP1], 119);
    MotorEncoder motor_encoder(motor_encoder_direct, output_encoder_direct, 0);
    OutputEncoder &output_encoder = motor_encoder.secondary();

    GPIO torque_sensor_cs(*GPIOD, 2, GPIO::OUTPUT);
    SPITorque torque_sensor(*SPI3, torque_sensor_cs, *DMA1_Channel1, *DMA1_Channel2,  
        SPIDMA::spi_pause[SPIDMA::SP3], 20);

    GPIO imu_cs(*GPIOB, 4, GPIO::OUTPUT);
    SPIDMA spi1_dma_bmi270(SPIDMA::SP3, imu_cs, DMA1_CH1, DMA1_CH2, 1000, 40, 40,
        SPI_CR1_MSTR | (4 << SPI_CR1_BR_Pos) | SPI_CR1_SSI | SPI_CR1_SSM);    // baud = clock/32
    BMI270 imu(spi1_dma_bmi270);
};

#include "../../motorlib/boards/config_obot_g474_osa.cpp"

void config_init() {
    config::imu.init();
    
    
    System::api.add_api_variable("mbct", new APICallbackUint32([](){ return config::motor_encoder_direct.get_bct(); },
                    [](uint32_t u){ config::motor_encoder_direct.set_bct(u); }));
    System::api.add_api_variable("met", new APICallbackUint32([](){ return config::motor_encoder_direct.get_et(); },
                    [](uint32_t u){ config::motor_encoder_direct.set_et(u); }));
    System::api.add_api_variable("mmgt", new APICallbackUint32([](){ return config::motor_encoder_direct.get_magnetic_field_strength(); },
                    [](uint32_t u){ config::motor_encoder_direct.set_mgt(u); }));
    System::api.add_api_variable("jbct", new APICallbackUint32([](){ return config::output_encoder_direct.get_bct(); },
                    [](uint32_t u){ config::output_encoder_direct.set_bct(u); }));
    System::api.add_api_variable("jet", new APICallbackUint32([](){ return config::output_encoder_direct.get_et(); },
                    [](uint32_t u){ config::output_encoder_direct.set_et(u); }));
    System::api.add_api_variable("jmgt", new APICallbackUint32([](){ return config::output_encoder_direct.get_magnetic_field_strength(); },
                    [](uint32_t u){ config::output_encoder_direct.set_mgt(u); }));

    
/*     System::api.add_api_variable("CR", new APICallbackUint32([](){ return config::torque_sensor.result0_; },
                    [](uint32_t u){ config::torque_sensor.reset(u); }));   */

    System::api.add_api_variable("CR", new APICallbackUint32([](){ config::torque_sensor.reset(1); return 1; },
                    [](uint32_t u){ config::torque_sensor.reset(u); }));                                            //TORQUE SENSOR RESET COMAND */

    System::api.add_api_variable("C0", new const APIUint32(&config::torque_sensor.result0_));
    System::api.add_api_variable("C1", new const APIUint32(&config::torque_sensor.result1_)); 
    System::api.add_api_variable("C2", new const APIUint32(&config::torque_sensor.result2_));
    System::api.add_api_variable("C3", new const APIUint32(&config::torque_sensor.result3_));
    System::api.add_api_variable("C4", new const APIUint32(&config::torque_sensor.result4_));
    System::api.add_api_variable("C5", new const APIUint32(&config::torque_sensor.result5_));

    System::api.add_api_variable("RATIO_A", new const APIFloat(&config::torque_sensor.ratio_A_));
    System::api.add_api_variable("RATIO_B", new const APIFloat(&config::torque_sensor.ratio_B_));
    System::api.add_api_variable("RATIO_C", new const APIFloat(&config::torque_sensor.ratio_C_));

    System::api.add_api_variable("TORQUE_A", new const APIFloat(&config::torque_sensor.torque_A_));
    System::api.add_api_variable("TORQUE_B", new const APIFloat(&config::torque_sensor.torque_B_));
    System::api.add_api_variable("TORQUE_C", new const APIFloat(&config::torque_sensor.torque_C_));

    System::api.add_api_variable("offset_A", new APIFloat(&config::torque_sensor.offset_A));
    System::api.add_api_variable("offset_B", new APIFloat(&config::torque_sensor.offset_B));
    System::api.add_api_variable("offset_C", new APIFloat(&config::torque_sensor.offset_C));
    System::api.add_api_variable("gain_A", new APIFloat(&config::torque_sensor.gain_A));
    System::api.add_api_variable("gain_B", new APIFloat(&config::torque_sensor.gain_B));
    System::api.add_api_variable("gain_C", new APIFloat(&config::torque_sensor.gain_C));
    System::api.add_api_variable("gain_T", new APIFloat(&config::torque_sensor.gain_T));

    //System::api.add_api_variable("imu_read", new const APICallback([](){ return config::imu.get_string(); }));
    System::api.add_api_variable("imu_read", new const APICallback([](){ config::imu.read(); return "ok"; }));
    System::api.add_api_variable("ax", new const APICallbackFloat([](){ return config::imu.data_.acc_x*8./pow(2,15); }));
    System::api.add_api_variable("ay", new const APICallbackFloat([](){ return config::imu.data_.acc_y*8./pow(2,15); }));
    System::api.add_api_variable("az", new const APICallbackFloat([](){ return config::imu.data_.acc_z*8./pow(2,15); }));
    System::api.add_api_variable("gx", new const APICallbackFloat([](){ return config::imu.data_.gyr_x*2000.*M_PI/180/pow(2,15); }));
    System::api.add_api_variable("gy", new const APICallbackFloat([](){ return config::imu.data_.gyr_y*2000.*M_PI/180/pow(2,15); }));
    System::api.add_api_variable("gz", new const APICallbackFloat([](){ return config::imu.data_.gyr_z*2000.*M_PI/180/pow(2,15); }));   
    
    config::torque_sensor.reset(1); 
}

void config_maintenance() {}
