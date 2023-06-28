#include "../param/param_obot_g474_aksim.h"
#include "../st_device.h"
#include "../../motorlib/peripheral/stm32g4/spi_dma.h"
#include "../../motorlib/aksim2_encoder.h"
#include "../../motorlib/torque_sensor.h"
#include "../../motorlib/gpio.h"
#include "../../motorlib/temperature_sensor.h"
//#include "../../motorlib/qia128.h"
#include "../../motorlib/peripheral/stm32g4/qia128_uart.h"
#include "../../motorlib/peripheral/stm32g4/pin_config.h"
//#include "../../motorlib/sensor_multiplex.h"
#include "../../motorlib/peripheral/stm32g4/max31889.h"
#include "../../motorlib/sensor_multiplex.h"

#ifndef MOTOR_ENCODER_BITS
#define MOTOR_ENCODER_BITS 18
#endif

#ifndef OUTPUT_ENCODER_BITS
#define OUTPUT_ENCODER_BITS 18
#endif

//#pragma message XSTR(MOTOR_ENCODER_BITS)
//#pragma message XSTR(OUTPUT_ENCODER_BITS)

#define END_TRIGGER_MOTOR_ENCODER
using TorqueSensor = QIA128_UART; 
//using TorqueSensor = TorqueSensorBase;
using MotorEncoder = Aksim2Encoder<MOTOR_ENCODER_BITS>;
//using MotorEncoder = EncoderBase;
#ifdef JOINT_ENCODER_BITS
#define CUSTOM_SENDDATA
using OutputEncoder = SensorMultiplex<Aksim2Encoder<OUTPUT_ENCODER_BITS>, Aksim2Encoder<JOINT_ENCODER_BITS>>;
using JointEncoder = OutputEncoder::SecondarySensor;
#else
using OutputEncoder = Aksim2Encoder<OUTPUT_ENCODER_BITS>;
#endif
//using OutputEncoder = EncoderBase;

//using TorqueSensor = TorqueSensorMultiplex<QIA128, Aksim2Encoder<18>>;
//using OutputEncoder = TorqueSensor::SecondarySensor;

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

      // uart
    //   RCC->APB1ENR2 |= RCC_APB1ENR2_LPUART1EN;
    //   MASK_SET(RCC->CCIPR, RCC_CCIPR_LPUART1SEL, 1); // sysclk: 
    //   LPUART1->BRR = 256*CPU_FREQUENCY_HZ/320000;
    //   LPUART1->CR3 = 2 << USART_CR3_RXFTCFG_Pos; // 4 bytes fifo threshold
    //   LPUART1->CR1 = USART_CR1_FIFOEN | USART_CR1_TE | USART_CR1_RE | USART_CR1_UE;
    //   GPIO_SETL(C, 0, GPIO_MODE::ALT_FUN, GPIO_SPEED::LOW, 8);
    //   GPIO_SETL(C, 1, GPIO_MODE::ALT_FUN, GPIO_SPEED::LOW, 8);

        //uart5
        RCC->APB1ENR1 |= RCC_APB1ENR1_UART5EN;
        MASK_SET(RCC->CCIPR, RCC_CCIPR_UART5SEL, 1); // sysclk: 
        UART5->BRR = CPU_FREQUENCY_HZ/320000;
        UART5->CR3 = 2 << USART_CR3_RXFTCFG_Pos; // 4 bytes fifo threshold
        UART5->CR1 = USART_CR1_FIFOEN | USART_CR1_TE | USART_CR1_RE | USART_CR1_UE;
        GPIO_SETH(C, 12, GPIO_MODE::ALT_FUN, GPIO_SPEED::LOW, 5);
        GPIO_SETL(D, 2, GPIO_MODE::ALT_FUN, GPIO_SPEED::LOW, 5);

      GPIO_SETL(A, 0, GPIO_MODE::OUTPUT, GPIO_SPEED::VERY_HIGH, 0);   // PA0-> motor encoder cs
      // gpio out
      GPIO_SETL(A, 1, GPIO::OUTPUT, GPIO_SPEED::VERY_HIGH, 0);
      // gpio in
      GPIO_SETL(A, 2, GPIO::INPUT, GPIO_SPEED::VERY_HIGH, 0);

#ifdef JOINT_ENCODER_BITS
      GPIO_SETL(C, 2, GPIO_MODE::OUTPUT, GPIO_SPEED::MEDIUM, 0); // A3 used as joint encoder cs
      GPIOC->BSRR = GPIO_BSRR_BS2;
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
    //EncoderBase motor_encoder;
    
    GPIO output_encoder_cs(*GPIOC, 3, GPIO::OUTPUT);
    SPIDMA spi1_dma(*SPI1, output_encoder_cs, *DMA1_Channel3, *DMA1_Channel4);
#ifdef JOINT_ENCODER_BITS
    Aksim2Encoder<OUTPUT_ENCODER_BITS> output_encoder_direct(spi1_dma);
    GPIO joint_encoder_cs(*GPIOC, 2, GPIO::OUTPUT);
    SPIDMA spi1_dma2(*SPI1, joint_encoder_cs, *DMA1_Channel3, *DMA1_Channel4);
    Aksim2Encoder<JOINT_ENCODER_BITS> joint_encoder_direct(spi1_dma2);
    OutputEncoder output_encoder(output_encoder_direct, joint_encoder_direct);
    JointEncoder &joint_encoder = output_encoder.secondary();
#else
    OutputEncoder output_encoder(spi1_dma);
#endif
    //EncoderBase output_encoder;
    //QIA128_UART torque_sensor(*LPUART1);
    QIA128_UART torque_sensor(*UART5);
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
#define PWM_MULT 3
#endif
    config::motor_pwm.set_frequency_multiplier(PWM_MULT);
    System::api.add_api_variable("mdiag", new const APIUint8(&config::motor_encoder.diag_.word));
    System::api.add_api_variable("mdiag_raw", new const APIUint8(&config::motor_encoder.diag_raw_.word));
    System::api.add_api_variable("mcrc", new const APIUint8(&config::motor_encoder.crc_calc_));
    System::api.add_api_variable("merr", new APIUint32(&config::motor_encoder.diag_err_count_));
    System::api.add_api_variable("mwarn", new APIUint32(&config::motor_encoder.diag_warn_count_));
    System::api.add_api_variable("mcrc_cnt", new APIUint32(&config::motor_encoder.crc_err_count_));
    System::api.add_api_variable("mraw", new APIUint32(&config::motor_encoder.raw_value_));
    System::api.add_api_variable("mrawh", new const APICallback([](){ return u32_to_hex(config::motor_encoder.raw_value_); }));
    System::api.add_api_variable("mcrc_latch", new const APIUint32(&config::motor_encoder.crc_error_raw_latch_));
    System::api.add_api_variable("Tmotor", new const APICallbackFloat([](){ return config::motor_temperature.read(); }));
    System::api.add_api_variable("Tambient", new const APICallbackFloat([](){ return config::ambient_temperature.get_temperature(); }));
    System::api.add_api_variable("Tambient2", new const APICallbackFloat([](){ return config::ambient_temperature_if.get_temperature(); }));
    System::api.add_api_variable("Tambient3", new const APICallbackFloat([](){ return config::ambient_temperature_3.get_temperature(); }));
    System::api.add_api_variable("Tambient4", new const APICallbackFloat([](){ return config::ambient_temperature_4.get_temperature(); }));
#ifdef JOINT_ENCODER_BITS
    config::output_encoder_direct.spi_dma_.register_operation_ = config::drv.register_operation_;
    config::joint_encoder_direct.spi_dma_.register_operation_ = config::drv.register_operation_;
    System::api.add_api_variable("oerr", new APIUint32(&config::output_encoder_direct.diag_err_count_));
    System::api.add_api_variable("owarn", new APIUint32(&config::output_encoder_direct.diag_warn_count_));
    System::api.add_api_variable("ocrc_cnt", new APIUint32(&config::output_encoder_direct.crc_err_count_));
    System::api.add_api_variable("oraw", new APIUint32(&config::output_encoder_direct.raw_value_));
    System::api.add_api_variable("orawh", new const APICallback([](){ return u32_to_hex(config::output_encoder_direct.raw_value_); }));
    System::api.add_api_variable("ocrc_latch", new const APIUint32(&config::output_encoder_direct.crc_error_raw_latch_));
    System::api.add_api_variable("jerr", new APIUint32(&config::joint_encoder_direct.diag_err_count_));
    System::api.add_api_variable("jwarn", new APIUint32(&config::joint_encoder_direct.diag_warn_count_));
    System::api.add_api_variable("jcrc_cnt", new APIUint32(&config::joint_encoder_direct.crc_err_count_));
    System::api.add_api_variable("jraw", new APIUint32(&config::joint_encoder_direct.raw_value_));
    System::api.add_api_variable("jrawh", new const APICallback([](){ return u32_to_hex(config::joint_encoder_direct.raw_value_); }));
    System::api.add_api_variable("jcrc_latch", new const APIUint32(&config::joint_encoder_direct.crc_error_raw_latch_));
    System::api.add_api_variable("jbias", new APIFloat(&joint_encoder_bias));
#else
    config::output_encoder.spi_dma_.register_operation_ = config::drv.register_operation_;
    System::api.add_api_variable("oerr", new APIUint32(&config::output_encoder.diag_err_count_));
    System::api.add_api_variable("owarn", new APIUint32(&config::output_encoder.diag_warn_count_));
    System::api.add_api_variable("ocrc_cnt", new APIUint32(&config::output_encoder.crc_err_count_));
    System::api.add_api_variable("oraw", new APIUint32(&config::output_encoder.raw_value_));
    System::api.add_api_variable("orawh", new const APICallback([](){ return u32_to_hex(config::output_encoder.raw_value_); }));
    System::api.add_api_variable("ocrc_latch", new const APIUint32(&config::output_encoder.crc_error_raw_latch_));
#endif
    System::api.add_api_variable("brr", new APIUint32(&LPUART1->BRR));
    System::api.add_api_variable("cr1", new APIUint32(&LPUART1->CR1));
    System::api.add_api_variable("isr", new APIUint32(&LPUART1->ISR));
    System::api.add_api_variable("traw", new const APIUint32(&config::torque_sensor.raw_));
    System::api.add_api_variable("twait_error", new const APIUint32(&config::torque_sensor.wait_error_));
    System::api.add_api_variable("tread_error", new const APIUint32(&config::torque_sensor.read_error_));
    System::api.add_api_variable("tread_len", new const APIUint32(&config::torque_sensor.read_len_));
    System::api.add_api_variable("tcrc_error", new const APIUint32(&config::torque_sensor.crc_error_));
    System::api.add_api_variable("tcrc_calc", new const APIUint8(&config::torque_sensor.crc_calc_));
    System::api.add_api_variable("tcrc_read", new const APIUint8(&config::torque_sensor.crc_read_));
    System::api.add_api_variable("twait_error", new const APIUint32(&config::torque_sensor.wait_error_));
    System::api.add_api_variable("ttimeout_error", new const APIUint32(&config::torque_sensor.timeout_error_));
    System::api.add_api_variable("tfull_raw", new const APIUint32(&config::torque_sensor.full_raw_));
    System::api.add_api_variable("5V", new const APIFloat(&v5v));
    System::api.add_api_variable("V5V", new const APIUint32(&V5V));
    System::api.add_api_variable("I5V", new const APIUint32(&I5V));
    System::api.add_api_variable("i5V", new const APIFloat(&i5v));
    System::api.add_api_variable("i48V", new const APIFloat(&i48v));
    System::api.add_api_variable("IBUS", new const APIUint32(&I_BUS_DR));
    System::api.add_api_variable("TSENSE", new const APIUint32(&TSENSE));
    System::api.add_api_variable("TSENSE2", new const APIUint32(&TSENSE2));

}

FrequencyLimiter temp_rate_motor = {10};
void config_maintenance() {
    if(temp_rate_motor.run()) {
        config::motor_temperature.read();
        round_robin_logger.log_data(MOTOR_TEMPERATURE_INDEX, config::motor_temperature.get_temperature());
        if (config::motor_temperature.get_temperature() > 120) {
            config::main_loop.status_.error.motor_temperature = true;
        }
        config::ambient_temperature.read();
        round_robin_logger.log_data(AMBIENT_TEMPERATURE_INDEX, config::ambient_temperature.get_temperature());
        config::ambient_temperature_if.read();
        round_robin_logger.log_data(AMBIENT_TEMPERATURE_2_INDEX, config::ambient_temperature_if.get_temperature());
        config::ambient_temperature_3.read();
        round_robin_logger.log_data(AMBIENT_TEMPERATURE_3_INDEX, config::ambient_temperature_3.get_temperature());
        config::ambient_temperature_4.read();
        round_robin_logger.log_data(AMBIENT_TEMPERATURE_4_INDEX, config::ambient_temperature_4.get_temperature());
    }
    if(config::motor_encoder.crc_err_count_ > 100 || config::motor_encoder.diag_err_count_ > 100 ||
        config::motor_encoder.diag_warn_count_ > pow(2,31)) {
            config::main_loop.status_.error.motor_encoder = true;
    }
    round_robin_logger.log_data(MOTOR_ENCODER_CRC_INDEX, config::motor_encoder.crc_err_count_);
    round_robin_logger.log_data(MOTOR_ENCODER_ERROR_INDEX, config::motor_encoder.diag_err_count_);
#ifdef JOINT_ENCODER_BITS
    if (!joint_bias_set) {
        if (config::joint_encoder_direct.get_value() != 0) {
            joint_bias_set = true;
            joint_encoder_bias = param->joint_encoder_bias;
            float joint_position = (float) config::joint_encoder_direct.get_value()*2*M_PI/pow(2,JOINT_ENCODER_BITS) + joint_encoder_bias;
            if (joint_position > param->joint_encoder_rollover) {
                joint_encoder_bias -= 2*M_PI;
            } else if (joint_position < -param->joint_encoder_rollover) {
                joint_encoder_bias += 2*M_PI;
            }
            logger.log_printf("joint encoder raw: %f, joint encoder bias: %f", (float) config::joint_encoder_direct.get_value()*2*M_PI/pow(2,JOINT_ENCODER_BITS), joint_encoder_bias);
        }
    }
    if(config::output_encoder_direct.crc_err_count_ > pow(2,31) || config::output_encoder_direct.diag_err_count_ > 100 ||
        config::output_encoder_direct.diag_warn_count_ > pow(2,31)) {
            config::main_loop.status_.error.output_encoder = true;
    }
    if(config::joint_encoder_direct.crc_err_count_ > pow(2,31) || config::joint_encoder_direct.diag_err_count_ > 100 ||
        config::joint_encoder_direct.diag_warn_count_ > pow(2,31)) {
            config::main_loop.status_.error.output_encoder = true;
    }
    round_robin_logger.log_data(OUTPUT_ENCODER_CRC_INDEX, config::output_encoder_direct.crc_err_count_);
    round_robin_logger.log_data(OUTPUT_ENCODER_ERROR_INDEX, config::output_encoder_direct.diag_err_count_);
    round_robin_logger.log_data(JOINT_ENCODER_CRC_INDEX, config::joint_encoder_direct.crc_err_count_);
    round_robin_logger.log_data(JOINT_ENCODER_ERROR_INDEX, config::joint_encoder_direct.diag_err_count_);
#else
    if(config::output_encoder.crc_err_count_ > pow(2,31) || config::output_encoder.diag_err_count_ > 100 ||
        config::output_encoder.diag_warn_count_ > pow(2,31)) {
            config::main_loop.status_.error.output_encoder = true;
    }
    round_robin_logger.log_data(OUTPUT_ENCODER_CRC_INDEX, config::output_encoder.crc_err_count_);
    round_robin_logger.log_data(OUTPUT_ENCODER_ERROR_INDEX, config::output_encoder.diag_err_count_);
#endif
    v5v = (float) V5V/4096*v3v3*2;
    i5v = (float) I5V/4096*v3v3;
    i48v = -((float) I_BUS_DR-2048)/4096*v3v3/20/.0005;
    round_robin_logger.log_data(VOLTAGE_5V_INDEX, v5v);
    round_robin_logger.log_data(CURRENT_5V_INDEX, i5v);
#if defined (HAS_BUS_CURRENT_SENSE)
    round_robin_logger.log_data(BUS_CURRENT_INDEX, i48v);
#endif
    round_robin_logger.log_data(TORQUE_SENSOR_CRC_INDEX, config::torque_sensor.crc_error_);
    round_robin_logger.log_data(TORQUE_SENSOR_ERROR_INDEX, config::torque_sensor.read_error_ + config::torque_sensor.wait_error_ + config::torque_sensor.timeout_error_);
    if (config::torque_sensor.timeout_error_ > 100) {
        config::main_loop.status_.error.torque_sensor = true;
    }
}

#ifdef JOINT_ENCODER_BITS
void load_send_data(const MainLoop &main_loop, SendData * const data) {
    static FirstOrderLowPassFilter joint_position_filter(1.0/config::main_loop_frequency, 1000);
    static FIRFilter<> joint_velocity_filter(1.0/config::main_loop_frequency, param->main_loop_param.output_filter_hz.output_velocity);
    data->iq = main_loop.status_.fast_loop.iq_filtered;
    data->host_timestamp_received = main_loop.host_timestamp_;
    data->mcu_timestamp = main_loop.status_.fast_loop.timestamp;
    data->motor_position = main_loop.status_.motor_position_filtered;
    data->reserved = main_loop.status_.output_position_filtered;
    data->motor_velocity = main_loop.status_.motor_velocity_filtered;
    
    data->torque = main_loop.status_.torque_filtered;
    float joint_position = (float) config::joint_encoder_direct.get_value()*2*M_PI/pow(2,JOINT_ENCODER_BITS) + joint_encoder_bias;
    data->joint_position = joint_position_filter.update(joint_position);
    data->joint_velocity = joint_velocity_filter.update(joint_position);
    data->torque = main_loop.status_.torque;
    data->rr_data = main_loop.status_.rr_data;
    data->reserved = main_loop.status_.output_position;
    data->flags.mode = main_loop.status_.mode;
    data->flags.error = main_loop.status_.error;
    data->flags.misc.byte = 0;
#ifdef GPIO_IN
    data->flags.misc.gpio = GPIO_IN;
#endif  // GPIO_IN
}
#endif