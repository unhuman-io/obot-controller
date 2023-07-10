#include "../param/param_obot_g474.h"
#include "../st_device.h"
#include "../../motorlib/peripheral/stm32g4/spi_dma.h"
#include "../../motorlib/icpz.h"
#include "../../motorlib/torque_sensor.h"
#include "../../motorlib/gpio.h"
#include "../../motorlib/temperature_sensor.h"
#include "../../motorlib/peripheral/stm32g4/pin_config.h"
#include "../../motorlib/peripheral/stm32g4/max31889.h"



//#define END_TRIGGER_MOTOR_ENCODER

using TorqueSensor = TorqueSensorBase;
using MotorEncoder = ICPZ;
using OutputEncoder = ICPZ;


extern "C" void SystemClock_Config();
void pin_config_obot_g474_motor_r0();

// setup for R4

struct InitCode {
    InitCode() {
      SystemClock_Config();
      pin_config_obot_g474_motor_r0();
      SPI3->CR2 = (7 << SPI_CR2_DS_Pos) | SPI_CR2_FRXTH;   // 8 bit
      // ORDER DEPENDANCE SPE set last
      SPI3->CR1 = SPI_CR1_MSTR | (5 << SPI_CR1_BR_Pos) | SPI_CR1_SSI | SPI_CR1_SSM | SPI_CR1_CPOL;    // baud = clock/64
           
      SPI1->CR2 = (7 << SPI_CR2_DS_Pos) | SPI_CR2_FRXTH;   // 8 bit
      // ORDER DEPENDANCE SPE set last
      SPI1->CR1 = SPI_CR1_MSTR | (5 << SPI_CR1_BR_Pos) | SPI_CR1_SSI | SPI_CR1_SSM | SPI_CR1_CPOL;    // baud = clock/64
      DMAMUX1_Channel0->CCR =  DMA_REQUEST_SPI3_TX;
      DMAMUX1_Channel1->CCR =  DMA_REQUEST_SPI3_RX;
      DMAMUX1_Channel2->CCR =  DMA_REQUEST_SPI1_TX;
      DMAMUX1_Channel3->CCR =  DMA_REQUEST_SPI1_RX;


      GPIO_SETL(A, 0, GPIO_MODE::OUTPUT, GPIO_SPEED::VERY_HIGH, 0);   // PA0-> motor encoder cs
      // gpio out
      GPIO_SETL(A, 1, GPIO::OUTPUT, GPIO_SPEED::VERY_HIGH, 0);
      // gpio in
      GPIO_SETL(A, 2, GPIO::INPUT, GPIO_SPEED::VERY_HIGH, 0);



#ifndef ENABLE_USBN_PULLDOWN
      GPIO_SETH(A, 9, GPIO_MODE::OUTPUT, GPIO_SPEED::MEDIUM, 0); // A3 used as joint encoder cs
      GPIOA->BSRR = GPIO_BSRR_BR9;
#endif
    }
};

namespace config {
    const uint32_t main_loop_frequency = 10000;
    const uint32_t pwm_frequency = 25000;
    InitCode init_code;

    GPIO motor_encoder_cs(*GPIOA, 0, GPIO::OUTPUT);
    SPIDMA spi3_dma(*SPI3, motor_encoder_cs, *DMA1_Channel1, *DMA1_Channel2);
    MotorEncoder motor_encoder(spi3_dma);
    
    GPIO output_encoder_cs(*GPIOC, 3, GPIO::OUTPUT);
    SPIDMA spi1_dma(*SPI1, output_encoder_cs, *DMA1_Channel3, *DMA1_Channel4);

    OutputEncoder output_encoder(spi1_dma);
    TorqueSensorBase torque_sensor;
};

#define SPI1_REINIT_CALLBACK
void spi1_reinit_callback() {
    SPI1->CR1=0;
    SPI1->CR2 = (7 << SPI_CR2_DS_Pos) | SPI_CR2_FRXTH;   // 8 bit
    // ORDER DEPENDANCE SPE set last
    SPI1->CR1 = SPI_CR1_MSTR | (5 << SPI_CR1_BR_Pos) | SPI_CR1_SSI | SPI_CR1_SSM | SPI_CR1_CPOL;    // baud = clock/64
    config::spi1_dma.reinit();
}

#include "../../motorlib/boards/config_obot_g474_motor.cpp"

namespace config {
    PT1000 motor_temperature(A1_DR, v3v3);
    MAX31889 ambient_temperature(i2c1);
    MAX31889 ambient_temperature_if(i2c1,1);
    MAX31889 ambient_temperature_3(i2c1,2);
    MAX31889 ambient_temperature_4(i2c1,3);
};

float v5v, i5v, i48v;
#ifdef JOINT_ENCODER_BITS
float joint_encoder_bias = 0;
bool joint_bias_set = false;
#endif

void config_init() {
#ifndef PWM_MULT
#define PWM_MULT 2
#endif
    config::motor_pwm.set_frequency_multiplier(PWM_MULT);
    // System::api.add_api_variable("mdiag", new const APIUint8(&config::motor_encoder.diag_.word));
    // System::api.add_api_variable("mdiag_raw", new const APIUint8(&config::motor_encoder.diag_raw_.word));
    // System::api.add_api_variable("mcrc", new const APIUint8(&config::motor_encoder.crc_calc_));
    // System::api.add_api_variable("merr", new APIUint32(&config::motor_encoder.diag_err_count_));
    // System::api.add_api_variable("mwarn", new APIUint32(&config::motor_encoder.diag_warn_count_));
    // System::api.add_api_variable("mcrc_cnt", new APIUint32(&config::motor_encoder.crc_err_count_));
    // System::api.add_api_variable("mraw", new APIUint32(&config::motor_encoder.raw_value_));
    // System::api.add_api_variable("mrawh", new const APICallback([](){ return u32_to_hex(config::motor_encoder.raw_value_); }));
    // System::api.add_api_variable("mcrc_latch", new const APIUint32(&config::motor_encoder.crc_error_raw_latch_));
    System::api.add_api_variable("Tmotor", new const APICallbackFloat([](){ return config::motor_temperature.read(); }));
    System::api.add_api_variable("Tambient", new const APICallbackFloat([](){ return config::ambient_temperature.get_temperature(); }));
    System::api.add_api_variable("Tambient2", new const APICallbackFloat([](){ return config::ambient_temperature_if.get_temperature(); }));
    System::api.add_api_variable("Tambient3", new const APICallbackFloat([](){ return config::ambient_temperature_3.get_temperature(); }));
    System::api.add_api_variable("Tambient4", new const APICallbackFloat([](){ return config::ambient_temperature_4.get_temperature(); }));

    config::output_encoder.spi_dma_.register_operation_ = config::drv.register_operation_;
    // System::api.add_api_variable("oerr", new APIUint32(&config::output_encoder.diag_err_count_));
    // System::api.add_api_variable("owarn", new APIUint32(&config::output_encoder.diag_warn_count_));
    // System::api.add_api_variable("ocrc_cnt", new APIUint32(&config::output_encoder.crc_err_count_));
    // System::api.add_api_variable("oraw", new APIUint32(&config::output_encoder.raw_value_));
    // System::api.add_api_variable("orawh", new const APICallback([](){ return u32_to_hex(config::output_encoder.raw_value_); }));
    // System::api.add_api_variable("ocrc_latch", new const APIUint32(&config::output_encoder.crc_error_raw_latch_));
   
    System::api.add_api_variable("5V", new const APIFloat(&v5v));
    System::api.add_api_variable("V5V", new const APIUint32(&V5V));
    System::api.add_api_variable("I5V", new const APIUint32(&I5V));
    System::api.add_api_variable("i5V", new const APIFloat(&i5v));
    System::api.add_api_variable("i48V", new const APIFloat(&i48v));
    System::api.add_api_variable("IBUS", new const APIUint32(&I_BUS_DR));
    System::api.add_api_variable("TSENSE", new const APIUint32(&TSENSE));
    System::api.add_api_variable("TSENSE2", new const APIUint32(&TSENSE2));

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
    // if(config::motor_encoder.crc_err_count_ > 100 || config::motor_encoder.diag_err_count_ > 100 ||
    //     config::motor_encoder.diag_warn_count_ > pow(2,31)) {
    //         config::main_loop.status_.error.motor_encoder = true;
    // }
    // round_robin_logger.log_data(MOTOR_ENCODER_CRC_INDEX, config::motor_encoder.crc_err_count_);
    // round_robin_logger.log_data(MOTOR_ENCODER_ERROR_INDEX, config::motor_encoder.diag_err_count_);

    // if(config::output_encoder.crc_err_count_ > pow(2,31) || config::output_encoder.diag_err_count_ > 100 ||
    //     config::output_encoder.diag_warn_count_ > pow(2,31)) {
    //         config::main_loop.status_.error.output_encoder = true;
    // }
    // round_robin_logger.log_data(OUTPUT_ENCODER_CRC_INDEX, config::output_encoder.crc_err_count_);
    // round_robin_logger.log_data(OUTPUT_ENCODER_ERROR_INDEX, config::output_encoder.diag_err_count_);

    v5v = (float) V5V/4096*v3v3*2;
    i5v = (float) I5V/4096*v3v3;
    i48v = -((float) I_BUS_DR-2048)/4096*v3v3/20/.0005;
    round_robin_logger.log_data(VOLTAGE_5V_INDEX, v5v);
    round_robin_logger.log_data(CURRENT_5V_INDEX, i5v);
#if defined (HAS_BUS_CURRENT_SENSE)
    round_robin_logger.log_data(BUS_CURRENT_INDEX, i48v);
#endif
    // round_robin_logger.log_data(TORQUE_SENSOR_CRC_INDEX, config::torque_sensor.crc_error_);
    // round_robin_logger.log_data(TORQUE_SENSOR_ERROR_INDEX, config::torque_sensor.read_error_ + config::torque_sensor.wait_error_ + config::torque_sensor.timeout_error_);
    // if (config::torque_sensor.timeout_error_ > 100) {
    //     config::main_loop.status_.error.torque_sensor = true;
    // }
}
