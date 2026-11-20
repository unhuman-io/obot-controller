#include <cstdint>
// struct SendData {
//     uint32_t encoder[6];
// };

#include "../param/param_obot_g474.h"
#include "st_device.h"
#include "../../motorlib/encoder.h"
#include "../../motorlib/torque_sensor.h"
#include "../../motorlib/gpio.h"
#include <algorithm>
#include "../../motorlib/peripheral/stm32g4/pin_config.h"
#include "../../motorlib/peripheral/stm32g4/spi_dma.h"
#include "../../motorlib/peripheral/stm32g4/spi.h"
#include "../../motorlib/parameter_api.h"

import ma782_encoder;

#define COMMS   COMMS_USB

using TorqueSensor = TorqueSensorBase;
using MotorEncoder = EncoderBase;
using OutputEncoder = EncoderBase;



struct InitCode {
    InitCode() {
        // trace pins enable
        RCC->AHB2ENR |= RCC_AHB2ENR_GPIOEEN;
        GPIO_SETL(E, 2, GPIO_MODE::ALT_FUN, GPIO_SPEED::VERY_HIGH, 0);
        GPIO_SETL(E, 3, GPIO_MODE::ALT_FUN, GPIO_SPEED::VERY_HIGH, 0);
        GPIO_SETL(E, 4, GPIO_MODE::ALT_FUN, GPIO_SPEED::VERY_HIGH, 0);
        GPIO_SETL(E, 5, GPIO_MODE::ALT_FUN, GPIO_SPEED::VERY_HIGH, 0);
        GPIO_SETL(E, 6, GPIO_MODE::ALT_FUN, GPIO_SPEED::VERY_HIGH, 0);
        //ETM->CR |= ETM_CR_ETMEN;
        uint32_t *etmcr = (uint32_t *)0xE0041000;
        *etmcr |= 1 << 11;
        uint32_t *etmteevr = (uint32_t *)0xE0041020;
        *etmteevr = 0x000037ef; // ON

        SPI1->CR2 = (15 << SPI_CR2_DS_Pos);   // 16 bit
        // ORDER DEPENDANCE SPE set last
        SPI1->CR1 = SPI_CR1_MSTR | (3 << SPI_CR1_BR_Pos) | SPI_CR1_SSI | SPI_CR1_SSM | SPI_CR1_SPE;    // baud = clock/16
        GPIO_SETL(A, 0, GPIO_MODE::OUTPUT, GPIO_SPEED::LOW, 0);
        GPIO_SETL(A, 1, GPIO_MODE::OUTPUT, GPIO_SPEED::LOW, 0);
        GPIO_SETL(A, 2, GPIO_MODE::OUTPUT, GPIO_SPEED::LOW, 0);
        GPIO_SETL(A, 3, GPIO_MODE::OUTPUT, GPIO_SPEED::LOW, 0);
        GPIO_SETL(A, 4, GPIO_MODE::OUTPUT, GPIO_SPEED::LOW, 0);
        GPIO_SETH(A, 8, GPIO_MODE::OUTPUT, GPIO_SPEED::LOW, 0);
        GPIOA->BSRR = GPIO_BSRR_BS0 | GPIO_BSRR_BS1 | GPIO_BSRR_BS2 | GPIO_BSRR_BS3 | GPIO_BSRR_BS4 | GPIO_BSRR_BS8;
    }
};

namespace config {
    const uint32_t main_loop_frequency = 1000;    
    const uint32_t pwm_frequency = 12000;
    InitCode init_code;

    MotorEncoder motor_encoder;
    TorqueSensor torque_sensor;
    OutputEncoder output_encoder;

    GPIO gpio_cs1(*GPIOA, 0, GPIO::OUTPUT);
    SPI spi1 {*SPI1};
    MA782Encoder ma782_1(spi1, gpio_cs1, SPIDMA::spi_pause[SPIDMA::SP1], MA782Encoder<SPI>::_4096);
    GPIO gpio_cs2(*GPIOA, 1, GPIO::OUTPUT);
    MA782Encoder ma782_2(spi1, gpio_cs2, SPIDMA::spi_pause[SPIDMA::SP1], MA782Encoder<SPI>::_4096);
    GPIO gpio_cs3(*GPIOA, 2, GPIO::OUTPUT);
    MA782Encoder ma782_3(spi1, gpio_cs3, SPIDMA::spi_pause[SPIDMA::SP1], MA782Encoder<SPI>::_4096);
    GPIO gpio_cs4(*GPIOA, 3, GPIO::OUTPUT);
    MA782Encoder ma782_4(spi1, gpio_cs4, SPIDMA::spi_pause[SPIDMA::SP1], MA782Encoder<SPI>::_4096);
    GPIO gpio_cs5(*GPIOA, 4, GPIO::OUTPUT);
    MA782Encoder ma782_5(spi1, gpio_cs5, SPIDMA::spi_pause[SPIDMA::SP1], MA782Encoder<SPI>::_4096);
    GPIO gpio_cs6(*GPIOA, 8, GPIO::OUTPUT);
    MA782Encoder ma782_6(spi1, gpio_cs6, SPIDMA::spi_pause[SPIDMA::SP1], MA782Encoder<SPI>::_4096);
};

#include "../../motorlib/boards/config_obot_g474_trace.cpp"

void config_init() {
    config::ma782_1.add_debug_variables<"m1">(System::api, System::communication_);
    config::ma782_2.add_debug_variables<"m2">(System::api, System::communication_);
    config::ma782_3.add_debug_variables<"m3">(System::api, System::communication_);
    config::ma782_4.add_debug_variables<"m4">(System::api, System::communication_);
    config::ma782_5.add_debug_variables<"m5">(System::api, System::communication_);
    config::ma782_6.add_debug_variables<"m6">(System::api, System::communication_);

    config::ma782_1.init();
    IWDG->KR = 0xAAAA;
    config::ma782_2.init();
    IWDG->KR = 0xAAAA;
    config::ma782_3.init();
    IWDG->KR = 0xAAAA;
    config::ma782_4.init();
    IWDG->KR = 0xAAAA;
    config::ma782_5.init();
    IWDG->KR = 0xAAAA;
    config::ma782_6.init();
}

void config_maintenance() {}

// void load_send_data(const MainLoop &main_loop, SendData * const data) {
//     config::ma782_1.trigger();
//     data->encoder[0] = config::ma782_1.read();
//     config::ma782_2.trigger();
//     data->encoder[1] = config::ma782_2.read();
//     config::ma782_3.trigger();
//     data->encoder[2] = config::ma782_3.read();
//     config::ma782_4.trigger();
//     data->encoder[3] = config::ma782_4.read();
//     config::ma782_5.trigger();
//     data->encoder[4] = config::ma782_5.read();
//     config::ma782_6.trigger();
//     data->encoder[5] = config::ma782_6.read();
// }