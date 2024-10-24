#include "../../motorlib/boards/config_obot_g474_motor.h"
#include "../param/param_obot_g474_pz2.h"
#include "st_device.h"
#include "../../motorlib/peripheral/stm32g4/spi_dma.h"
#include "../../motorlib/sensors/encoders/stm32g4/icpz_dma.h"
#include "../../motorlib/torque_sensor.h"
#include "../../motorlib/gpio.h"
#include "../../motorlib/temperature_sensor.h"
#include "../../motorlib/peripheral/stm32g4/pin_config.h"
#include "../../motorlib/peripheral/stm32g4/max31889.h"
#include "../../motorlib/sensor_multiplex.h"
#include "../../motorlib/sensors/torque/stm32g4/max11254_dma.h"
#define COMMS   COMMS_USB

// #define COMMS   COMMS_UART
// #define COMMS_UART_BAUDRATE 4000000
// #define COMMS_UART_NUMBER 2

#define GPIO_OUT int gpio_out_1234 // todo: necessary?

//#define END_TRIGGER_MOTOR_ENCODER


using TorqueSensor = TorqueSensorBase;// TorqueSensorMultiplex<MAX11254<>, ICPZ>;
using OutputEncoder = ICPZDMA;//TorqueSensor::SecondarySensor;
using MotorEncoder = ICPZDMA;

uint32_t gpio_d2_bsrr[2] = {4 << 16, 4};
uint32_t gpio_c3_bsrr[2] = {8 << 16, 8};

inline void motor_start_cs_trigger() {
    HRTIM1->sTimerxRegs[0].TIMxDIER = HRTIM_TIMDIER_CMP1DE |  HRTIM_TIMDIER_CMP2DE ;
}
    
inline void motor_stop_cs_trigger() {
    HRTIM1->sTimerxRegs[0].TIMxDIER = 0;
    // wait for CS high
    while(!(GPIOD->IDR & 0x4));
}

inline void output_start_cs_trigger() {
    HRTIM1->sMasterRegs.MDIER |= HRTIM_MDIER_MCMP2DE;
}
    
inline void output_stop_cs_trigger() {
    HRTIM1->sMasterRegs.MDIER &= ~HRTIM_MDIER_MCMP2DE;
    // wait for CS high
    while(!(GPIOC->IDR & 0x8));
}

struct InitCode {
    InitCode() {
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


      GPIO_SETL(D, 2, GPIO_MODE::OUTPUT, GPIO_SPEED::VERY_HIGH, 0);   // PD2-> motor encoder cs
      GPIOD->BSRR = GPIO_BSRR_BS2;
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
      GPIOC->BSRR = GPIO_BSRR_BS3;
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

        // motor icpz dma trigger
        HRTIM1->sTimerxRegs[0].CMP1xR = 47000;
        HRTIM1->sTimerxRegs[0].CMP2xR = 7000;
        HRTIM1->sTimerxRegs[0].TIMxCR2 = 0;
        HRTIM1->sTimerxRegs[0].PERxR = 54400;
        HRTIM1->sTimerxRegs[0].TIMxCR |= HRTIM_TIMCR_PREEN | HRTIM_TIMCR_TRSTU | HRTIM_TIMCR_CONT | 1 << HRTIM_TIMCR_CK_PSC_Pos;
        //HRTIM1->sTimerxRegs[0].TIMxDIER = HRTIM_TIMDIER_CMP1DE; // | HRTIM_TIMDIER_CMP2DE;

        DMAMUX1_Channel4->CCR = 96; // hrtima
        DMA1_Channel5->CMAR = (uint32_t)&gpio_d2_bsrr;
        DMA1_Channel5->CPAR = (uint32_t)&GPIOD->BSRR;
        DMA1_Channel5->CNDTR = 1;
        GPIOD->BSRR = 4;
        DMA1_Channel5->CCR = DMA_CCR_CIRC | DMA_CCR_DIR | DMA_CCR_EN | DMA_CCR_MINC | DMA_CCR_MSIZE_1 | DMA_CCR_PSIZE_1;

        // DMAMUX1_Channel5->CCR = 16 << DMAMUX_CxCR_SYNC_ID_Pos | 1 << DMAMUX_CxCR_SPOL_Pos | DMAMUX_CxCR_SE | 1; // ch1 rx complete
        DMAMUX1_Channel5->CCR = 1; // reqgen0
        DMAMUX1_RequestGenerator0->RGCR = 1 << DMAMUX_RGxCR_GPOL_Pos | 17 << DMAMUX_RGxCR_SIG_ID_Pos | DMAMUX_RGxCR_GE;
        DMA1_Channel6->CMAR = (uint32_t)&gpio_d2_bsrr[1];
        DMA1_Channel6->CPAR = (uint32_t)&GPIOD->BSRR;
        DMA1_Channel6->CNDTR = 1;
        DMA1_Channel6->CCR = DMA_CCR_CIRC | DMA_CCR_DIR | DMA_CCR_EN | DMA_CCR_MINC | DMA_CCR_MSIZE_1 | DMA_CCR_PSIZE_1;

        MASK_SET(SYSCFG->EXTICR[0], SYSCFG_EXTICR1_EXTI2, 3); // EXTI PD2

        // output icpz dma
        HRTIM1->sMasterRegs.MCMP2R = 200;

        DMAMUX1_Channel10->CCR = 95; // hrtim master
        DMA2_Channel3->CMAR = (uint32_t)&gpio_c3_bsrr;
        DMA2_Channel3->CPAR = (uint32_t)&GPIOC->BSRR;
        DMA2_Channel3->CNDTR = 1;
        GPIOC->BSRR = 2;
        DMA2_Channel3->CCR = DMA_CCR_CIRC | DMA_CCR_DIR | DMA_CCR_EN | DMA_CCR_MINC | DMA_CCR_MSIZE_1 | DMA_CCR_PSIZE_1;

        // DMAMUX1_Channel5->CCR = 16 << DMAMUX_CxCR_SYNC_ID_Pos | 1 << DMAMUX_CxCR_SPOL_Pos | DMAMUX_CxCR_SE | 1; // ch1 rx complete
        DMAMUX1_Channel11->CCR = 2; // reqgen1
        DMAMUX1_RequestGenerator1->RGCR = 1 << DMAMUX_RGxCR_GPOL_Pos | 19 << DMAMUX_RGxCR_SIG_ID_Pos | DMAMUX_RGxCR_GE;
        DMA2_Channel4->CMAR = (uint32_t)&gpio_c3_bsrr[1];
        DMA2_Channel4->CPAR = (uint32_t)&GPIOC->BSRR;
        DMA2_Channel4->CNDTR = 1;
        DMA2_Channel4->CCR = DMA_CCR_CIRC | DMA_CCR_DIR | DMA_CCR_EN | DMA_CCR_MINC | DMA_CCR_MSIZE_1 | DMA_CCR_PSIZE_1;

        MASK_SET(SYSCFG->EXTICR[0], SYSCFG_EXTICR1_EXTI3, 2); // EXTI PC3

    //   DMAMUX1_Channel2->CCR = 3 << DMAMUX_CxCR_SYNC_ID_Pos | 4 << DMAMUX_CxCR_NBREQ_Pos | 2 << DMAMUX_CxCR_SPOL_Pos | DMAMUX_CxCR_SE | DMA_REQUEST_SPI1_TX;
    //   DMAMUX1_Channel3->CCR = 4 << DMAMUX_CxCR_NBREQ_Pos | DMAMUX_CxCR_EGE | DMA_REQUEST_SPI1_RX;
    //   DMA2_Channel3->CCR = DMA_CCR_CIRC | DMA_CCR_DIR | DMA_CCR_EN | DMA_CCR_MINC | DMA_CCR_MSIZE_1 | DMA_CCR_PSIZE_1;

      GPIO_SETH(A, 9, GPIO_MODE::OUTPUT, GPIO_SPEED::MEDIUM, 0);
#ifdef DISABLE_USBN_PULLDOWN
      GPIOA->BSRR = GPIO_BSRR_BR9;
#else
      GPIOA->BSRR = GPIO_BSRR_BS9;
#endif

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

    GPIO motor_encoder_cs(*GPIOD, 2, GPIO::OUTPUT);
    SPIDMA spi3_dma(SPIDMA::SP3, motor_encoder_cs, DMA1_CH1, DMA1_CH2, 1000);
    MotorEncoder motor_encoder(spi3_dma, *DMAMUX1_Channel0, *DMAMUX1_Channel1, 2,
        motor_start_cs_trigger, motor_stop_cs_trigger, ICPZDMA::PZ08S);

    
    GPIO output_encoder_cs(*GPIOC, 3, GPIO::OUTPUT);
    SPIDMA spi1_dma(SPIDMA::SP1, output_encoder_cs, DMA1_CH3, DMA1_CH4, 1000, 100, 100,
        SPI_CR1_MSTR | (3 << SPI_CR1_BR_Pos) | SPI_CR1_SSI | SPI_CR1_SSM);

    ICPZDMA output_encoder_direct(spi1_dma, *DMAMUX1_Channel2, *DMAMUX1_Channel3, 3,
        output_start_cs_trigger, output_stop_cs_trigger, ICPZDMA::PZ03S);    
    
    // GPIO torque_sensor_cs(*GPIOA, 0, GPIO::OUTPUT);
    // SPIDMA spi1_dma2(*SPI1, torque_sensor_cs, *DMA1_Channel3, *DMA1_Channel4, 100, 100, nullptr,
    //     SPI_CR1_MSTR | 6 << SPI_CR1_BR_Pos | SPI_CR1_SSI | SPI_CR1_SSM);
    // MAX11254<> torque_sensor_direct(spi1_dma2, 1);
    // TorqueSensor torque_sensor(torque_sensor_direct, output_encoder_direct)
    // OutputEncoder &output_encoder = torque_sensor.secondary();

    TorqueSensor torque_sensor;
    OutputEncoder &output_encoder = output_encoder_direct;
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

    ICPZ_SET_DEBUG_VARIABLES("m", System::api, config::motor_encoder);

    // System::api.add_api_variable("mcrc_latch", new const APIUint32(&config::motor_encoder.crc_error_raw_latch_));
    System::api.add_api_variable("Tmotor", new const APICallbackFloat([](){ return config::motor_temperature.read(); }));
    System::api.add_api_variable("Tambient", new const APICallbackFloat([](){ return config::ambient_temperature.get_temperature(); }));
    System::api.add_api_variable("Tambient2", new const APICallbackFloat([](){ return config::ambient_temperature_if.get_temperature(); }));
    System::api.add_api_variable("Tambient3", new const APICallbackFloat([](){ return config::ambient_temperature_3.get_temperature(); }));
    System::api.add_api_variable("Tambient4", new const APICallbackFloat([](){ return config::ambient_temperature_4.get_temperature(); }));

    ICPZ_SET_DEBUG_VARIABLES("o", System::api, config::output_encoder_direct);

    // System::api.add_api_variable("traw", new const APIUint32(&config::torque_sensor_direct.raw_value_));
    // System::api.add_api_variable("tint", new const APIInt32(&config::torque_sensor_direct.signed_value_));
    // System::api.add_api_variable("ttimeout_error", new const APIUint32(&config::torque_sensor_direct.timeout_error_));
    // System::api.add_api_variable("tread_error", new const APIUint32(&config::torque_sensor_direct.read_error_));
    // System::api.add_api_variable("tmux_delay", new APICallbackUint16([](){ return 0; }, [](uint16_t u){ config::torque_sensor_direct.write_reg16(5, u); }));
   
    System::api.add_api_variable("mstart", new const APICallback([]
                                                                 { config::motor_encoder.start_continuous_read(); return "ok"; }));
    System::api.add_api_variable("mstop", new const APICallback([]
                                                                { config::motor_encoder.stop_continuous_read(); return "ok"; }));
    System::api.add_api_variable("ostart", new const APICallback([]
                                                                 { config::output_encoder.start_continuous_read(); return "ok"; }));
    System::api.add_api_variable("ostop", new const APICallback([]
                                                                { config::output_encoder.stop_continuous_read(); return "ok"; }));
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

    // round_robin_logger.log_data(TORQUE_SENSOR_CRC_INDEX, config::torque_sensor_direct.read_error_);
    // round_robin_logger.log_data(TORQUE_SENSOR_ERROR_INDEX, config::torque_sensor_direct.timeout_error_);
    // if (config::torque_sensor_direct.timeout_error_ > 100 ||
    //     config::torque_sensor_direct.read_error_ > 100) {
    //     config::main_loop.status_.error.torque_sensor = true;
    // }
}
