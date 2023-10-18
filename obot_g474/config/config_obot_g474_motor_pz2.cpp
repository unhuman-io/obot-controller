#include "../../motorlib/boards/config_obot_g474_motor.h"
#include "../param/param_obot_g474_pz2.h"
#include "../st_device.h"
#include "../../motorlib/peripheral/stm32g4/spi_dma.h"
#include "../../motorlib/icpz.h"
#include "../../motorlib/torque_sensor.h"
#include "../../motorlib/gpio.h"
#include "../../motorlib/temperature_sensor.h"
#include "../../motorlib/peripheral/stm32g4/pin_config.h"
#include "../../motorlib/peripheral/stm32g4/max31889.h"
#include "../../motorlib/sensor_multiplex.h"
#include "../../motorlib/max11254.h"

#define GPIO_OUT int gpio_out_1234 // todo: necessary?

//#define END_TRIGGER_MOTOR_ENCODER


using TorqueSensor = TorqueSensorMultiplex<MAX11254<>, ICPZ>;
using OutputEncoder = TorqueSensor::SecondarySensor;
using MotorEncoder = ICPZ;


extern "C" void SystemClock_Config();
void pin_config_obot_g474_motor_r0();

#ifdef HD11
// MCS0 HDR24 PC1
// MCS1 HDR22 PC2
// OCS0 HDR17 PA4
// OCS1 HDR5  PD2
#define OCS_PIN D,2
#else
// MCS PD2
// OCS PC3
#define OCS_PIN C,3
#endif

// setup for R4

struct InitCode {
    InitCode() {
      SystemClock_Config();
      pin_config_obot_g474_motor_r0();
      SPI3->CR2 = (7 << SPI_CR2_DS_Pos) | SPI_CR2_FRXTH;   // 8 bit
      // ORDER DEPENDANCE SPE set last
      SPI3->CR1 = SPI_CR1_MSTR | (3 << SPI_CR1_BR_Pos) | SPI_CR1_SSI | SPI_CR1_SSM;    // baud = clock/16
           
      SPI1->CR2 = (7 << SPI_CR2_DS_Pos) | SPI_CR2_FRXTH;   // 8 bit
      // ORDER DEPENDANCE SPE set last
      SPI1->CR1 = SPI_CR1_MSTR | (3 << SPI_CR1_BR_Pos) | SPI_CR1_SSI | SPI_CR1_SSM;    // baud = clock/16
      DMAMUX1_Channel0->CCR =  DMA_REQUEST_SPI3_TX;
      DMAMUX1_Channel1->CCR =  DMA_REQUEST_SPI3_RX;
      DMAMUX1_Channel2->CCR =  DMA_REQUEST_SPI1_TX;
      DMAMUX1_Channel3->CCR =  DMA_REQUEST_SPI1_RX;


      GPIO_SETL(C, 2, GPIO_MODE::OUTPUT, GPIO_SPEED::VERY_HIGH, 0);   // PD2-> motor encoder cs
      GPIO_SETL(C, 1, GPIO_MODE::OUTPUT, GPIO_SPEED::VERY_HIGH, 0);   // PD2-> motor encoder cs
      GPIO_SETL(D, 2, GPIO_MODE::OUTPUT, GPIO_SPEED::VERY_HIGH, 0);   // PD2-> motor encoder cs
      GPIO_SETL(A, 4, GPIO_MODE::OUTPUT, GPIO_SPEED::VERY_HIGH, 0);   // PD2-> motor encoder cs
      
      // gpio out
      GPIO_SETL(A, 1, GPIO::OUTPUT, GPIO_SPEED::VERY_HIGH, 0);
      // gpio in
      GPIO_SETL(A, 2, GPIO::INPUT, GPIO_SPEED::VERY_HIGH, 0);
        // 17 PC3 output encoder cs
        // 4  PB4 LTC cs
        // 6  PB3 adc reset
        // 8  PA0 ADC CS
        // 10 PA1 temp CS
        // 12 PA2 DRDY
      GPIO_SETL(C, 3, GPIO_MODE::OUTPUT, GPIO_SPEED::VERY_HIGH, 0);   // PC3-> output encoder cs
      GPIO_SETL(B, 4, GPIO_MODE::OUTPUT, GPIO_SPEED::VERY_HIGH, 0);   // PB4 LTC cs
      GPIOB->BSRR = GPIO_BSRR_BS4;
      GPIO_SETL(B, 3, GPIO_MODE::OUTPUT, GPIO_SPEED::VERY_HIGH, 0);   // PB3 adc reset
      GPIOB->BSRR = GPIO_BSRR_BS3;
      GPIO_SETL(A, 0, GPIO_MODE::OUTPUT, GPIO_SPEED::VERY_HIGH, 0);   // PA2 adc cs
      GPIOA->BSRR = GPIO_BSRR_BS0;
      GPIO_SETL(A, 1, GPIO_MODE::OUTPUT, GPIO_SPEED::VERY_HIGH, 0);   // PA1 temp CS
      GPIOA->BSRR = GPIO_BSRR_BS1;
      GPIO_SETL(A, 2, GPIO_MODE::INPUT, GPIO_SPEED::VERY_HIGH, 0);   // DRDY

    //   GPIO_SETL(C, 4, GPIO_MODE::OUTPUT, GPIO_SPEED::MEDIUM, 0); // TODO: figure out why base config isn't setting this
    //   GPIOC->BSRR = GPIO_BSRR_BS4; // bmi270



      GPIO_SETH(A, 9, GPIO_MODE::OUTPUT, GPIO_SPEED::MEDIUM, 0);
#ifdef DISABLE_USBN_PULLDOWN
      GPIOA->BSRR = GPIO_BSRR_BR9;
#else
      GPIOA->BSRR = GPIO_BSRR_BS9;
#endif

      GPIOC->BSRR = GPIO_BSRR_BS1 | GPIO_BSRR_BS2 |GPIO_BSRR_BS3;
      GPIOA->BSRR = GPIO_BSRR_BS4;
      GPIOD->BSRR = GPIO_BSRR_BS2;
      for (int i=0;i<1;i++) {
        ms_delay(40); // for pz encoders to power on
        IWDG->KR = 0xAAAA;
      }
    }
};

namespace config {
    const uint32_t main_loop_frequency = 10000;
    const uint32_t pwm_frequency = 50000;
    InitCode init_code;

    GPIO motor_encoder_cs(*GPIOC, 2, GPIO::OUTPUT);
    SPIDMA spi3_dma(*SPI3, motor_encoder_cs, *DMA1_Channel1, *DMA1_Channel2);
    MotorEncoder motor_encoder(spi3_dma, ICPZ::PZ08S);

    
    GPIO output_encoder_cs(*GPIOC, 3, GPIO::OUTPUT);
    SPIDMA spi1_dma(*SPI1, output_encoder_cs, *DMA1_Channel3, *DMA1_Channel4, 100, 100, nullptr,
        SPI_CR1_MSTR | (3 << SPI_CR1_BR_Pos) | SPI_CR1_SSI | SPI_CR1_SSM);

    ICPZ output_encoder_direct(spi1_dma, ICPZ::PZ03S);    
    
    GPIO torque_sensor_cs(*GPIOA, 0, GPIO::OUTPUT);
    SPIDMA spi1_dma2(*SPI1, torque_sensor_cs, *DMA1_Channel3, *DMA1_Channel4, 100, 100, nullptr,
        SPI_CR1_MSTR | 6 << SPI_CR1_BR_Pos | SPI_CR1_SSI | SPI_CR1_SSM);
    MAX11254<> torque_sensor_direct(spi1_dma2, 0);
    TorqueSensor torque_sensor(torque_sensor_direct, output_encoder_direct);
    OutputEncoder &output_encoder = torque_sensor.secondary();
};

#define SPI1_REINIT_CALLBACK
void spi1_reinit_callback() {
    // necessary to recover from sleep
    config::spi1_dma.reinit();
    uint32_t tmp = DMA1_Channel4->CCR;
    DMA1_Channel4->CCR = 0; // flush spi1 rx
    DMA1_Channel4->CNDTR = 0;
    DMA1_Channel4->CCR = tmp;
}

#include "../../motorlib/boards/config_obot_g474_motor.cpp"

namespace config {
    PT1000 motor_temperature(A1_DR, v3v3);
    MAX31889 ambient_temperature(i2c1);
    MAX31889 ambient_temperature_if(i2c1,1);
    MAX31889 ambient_temperature_3(i2c1,2);
    MAX31889 ambient_temperature_4(i2c1,3);
};

void config_init() {
    config::motor_pwm.set_frequency_multiplier(param->pwm_multiplier);

    config::motor_encoder.set_debug_variables("m", System::api);

    // System::api.add_api_variable("mcrc_latch", new const APIUint32(&config::motor_encoder.crc_error_raw_latch_));
    System::api.add_api_variable("Tmotor", new const APICallbackFloat([](){ return config::motor_temperature.read(); }));
    System::api.add_api_variable("Tambient", new const APICallbackFloat([](){ return config::ambient_temperature.get_temperature(); }));
    System::api.add_api_variable("Tambient2", new const APICallbackFloat([](){ return config::ambient_temperature_if.get_temperature(); }));
    System::api.add_api_variable("Tambient3", new const APICallbackFloat([](){ return config::ambient_temperature_3.get_temperature(); }));
    System::api.add_api_variable("Tambient4", new const APICallbackFloat([](){ return config::ambient_temperature_4.get_temperature(); }));

    config::output_encoder_direct.spidma_.register_operation_ = config::drv.register_operation_;
    config::output_encoder_direct.register_operation_ = config::drv.register_operation_;
    config::output_encoder_direct.set_debug_variables("o", System::api);

    config::torque_sensor_direct.spi_dma_.register_operation_ = config::drv.register_operation_;
    System::api.add_api_variable("traw", new const APIUint32(&config::torque_sensor_direct.raw_value_));
    System::api.add_api_variable("tint", new const APIInt32(&config::torque_sensor_direct.signed_value_));
    System::api.add_api_variable("ttimeout_error", new const APIUint32(&config::torque_sensor_direct.timeout_error_));
    System::api.add_api_variable("tread_error", new const APIUint32(&config::torque_sensor_direct.read_error_));
   
    // System::api.add_api_variable("5V", new const APIFloat(&v5v));
    // System::api.add_api_variable("V5V", new const APIUint32(&V5V));
    // System::api.add_api_variable("I5V", new const APIUint32(&I5V));
    // System::api.add_api_variable("IBUS", new const APIUint32(&I_BUS_DR));
    // System::api.add_api_variable("TSENSE", new const APIUint32(&TSENSE));
    // System::api.add_api_variable("TSENSE2", new const APIUint32(&TSENSE2));

}

MedianFilter<> motor_temperature_filter;
MedianFilter<> ambient_temperature_filter;
MedianFilter<> ambient2_temperature_filter;
MedianFilter<> ambient3_temperature_filter;
MedianFilter<> ambient4_temperature_filter;

FrequencyLimiter temp_rate_motor = {10};
void config_maintenance() {
    if(temp_rate_motor.run()) {
        float Tmotor = motor_temperature_filter.update(config::motor_temperature.read());
        config::main_loop.motor_temperature_model_.set_ambient_temperature(Tmotor);
        round_robin_logger.log_data(MOTOR_TEMPERATURE_INDEX, Tmotor);
        if (Tmotor > 120) {
            config::main_loop.status_.error.motor_temperature = true;
        }
        float Tambient = ambient_temperature_filter.update(config::ambient_temperature.read());
        round_robin_logger.log_data(AMBIENT_TEMPERATURE_INDEX, Tambient);
        float Tambient2 = ambient2_temperature_filter.update(config::ambient_temperature_if.read());
        round_robin_logger.log_data(AMBIENT_TEMPERATURE_2_INDEX, Tambient2);
        float Tambient3 = ambient3_temperature_filter.update(config::ambient_temperature_3.read());
        round_robin_logger.log_data(AMBIENT_TEMPERATURE_3_INDEX, Tambient3);
        float Tambient4 = ambient4_temperature_filter.update(config::ambient_temperature_4.read());
        round_robin_logger.log_data(AMBIENT_TEMPERATURE_4_INDEX, Tambient4);
    }
    if(config::motor_encoder.crc_error_count_ > 100 || config::motor_encoder.error_count_ > 100 ||
        config::motor_encoder.warn_count_ > pow(2,31)) {
            config::main_loop.status_.error.motor_encoder = true;
    }
    round_robin_logger.log_data(MOTOR_ENCODER_CRC_INDEX, config::motor_encoder.crc_error_count_);
    round_robin_logger.log_data(MOTOR_ENCODER_ERROR_INDEX, config::motor_encoder.error_count_);

    if(config::output_encoder_direct.crc_error_count_ > 100 || config::output_encoder_direct.error_count_ > 100 ||
        config::output_encoder_direct.warn_count_ > pow(2,31)) {
            config::main_loop.status_.error.output_encoder = true;
    }
    round_robin_logger.log_data(OUTPUT_ENCODER_CRC_INDEX, config::output_encoder_direct.crc_error_count_);
    round_robin_logger.log_data(OUTPUT_ENCODER_ERROR_INDEX, config::output_encoder_direct.error_count_);

    round_robin_logger.log_data(TORQUE_SENSOR_CRC_INDEX, config::torque_sensor_direct.read_error_);
    round_robin_logger.log_data(TORQUE_SENSOR_ERROR_INDEX, config::torque_sensor_direct.timeout_error_);
    if (config::torque_sensor_direct.timeout_error_ > 100 ||
        config::torque_sensor_direct.read_error_ > 100) {
        config::main_loop.status_.error.torque_sensor = true;
    }
}
