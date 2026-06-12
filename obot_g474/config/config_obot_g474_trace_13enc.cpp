#include <cstdint>
#include <array>
// struct SendData {
//     std::array<uint32_t, 16> encoder;
// };

#include "../param/param_obot_g474_can.h"
#include "st_device.h"
#include "../../motorlib/encoder.h"
#include "../../motorlib/torque_sensor.h"
#include "../../motorlib/gpio.h"
#include <algorithm>
#include "../../motorlib/peripheral/stm32g4/pin_config.h"
#include "../../motorlib/peripheral/stm32g4/spi_dma.h"
#include "../../motorlib/peripheral/stm32g4/spi_dma_usart.h"
#include "../../motorlib/peripheral/stm32g4/spi.h"
#include "../../motorlib/peripheral/stm32g4/usart_spi.h"
#include "../../motorlib/parameter_api.h"

import ma782_encoder;

#define COMMS   COMMS_CAN

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

        RCC->APB1ENR1 |= RCC_APB1ENR1_SPI2EN | RCC_APB1ENR1_SPI3EN | RCC_APB1ENR1_USART2EN;
        RCC->APB2ENR |= RCC_APB2ENR_SPI4EN;

        SPI1->CR2 = (15 << SPI_CR2_DS_Pos);   // 16 bit
        // ORDER DEPENDANCE SPE set last
        SPI1->CR1 = SPI_CR1_MSTR | (3 << SPI_CR1_BR_Pos) | SPI_CR1_SSI | SPI_CR1_SSM | SPI_CR1_SPE;    // baud = clock/16
        GPIO_SETL(A, 1, GPIO_MODE::OUTPUT, GPIO_SPEED::LOW, 0);
        GPIO_SETL(A, 2, GPIO_MODE::OUTPUT, GPIO_SPEED::LOW, 0);
        GPIO_SETL(A, 3, GPIO_MODE::OUTPUT, GPIO_SPEED::LOW, 0);
        GPIO_SETL(A, 4, GPIO_MODE::OUTPUT, GPIO_SPEED::LOW, 0);
        GPIOA->BSRR = GPIO_BSRR_BS1 | GPIO_BSRR_BS2 | GPIO_BSRR_BS3 | GPIO_BSRR_BS4;

        SPI2->CR2 = (15 << SPI_CR2_DS_Pos);   // 16 bit
        // ORDER DEPENDANCE SPE set last
        SPI2->CR1 = SPI_CR1_MSTR | (3 << SPI_CR1_BR_Pos) | SPI_CR1_SSI | SPI_CR1_SSM | SPI_CR1_SPE;    // baud = clock/16
        GPIO_SETH(B, 10, GPIO_MODE::OUTPUT, GPIO_SPEED::LOW, 0);
        GPIO_SETH(B, 11, GPIO_MODE::OUTPUT, GPIO_SPEED::LOW, 0);
        GPIO_SETH(B, 12, GPIO_MODE::OUTPUT, GPIO_SPEED::LOW, 0);
        GPIO_SETH(B, 13, GPIO_MODE::ALT_FUN, GPIO_SPEED::LOW, 5);   // clk
        GPIO_SETH(B, 14, GPIO_MODE::ALT_FUN, GPIO_SPEED::LOW, 5);   // miso
        GPIO_SETH(B, 15, GPIO_MODE::ALT_FUN, GPIO_SPEED::LOW, 5);   // mosi
        GPIOB->BSRR = GPIO_BSRR_BS10 | GPIO_BSRR_BS11 | GPIO_BSRR_BS12;

        SPI3->CR2 = (15 << SPI_CR2_DS_Pos);   // 16 bit
        // ORDER DEPENDANCE SPE set last
        SPI3->CR1 = SPI_CR1_MSTR | (3 << SPI_CR1_BR_Pos) | SPI_CR1_SSI | SPI_CR1_SSM | SPI_CR1_SPE;    // baud = clock/16
        GPIO_SETL(C, 7, GPIO_MODE::OUTPUT, GPIO_SPEED::LOW, 0);
        GPIO_SETH(C, 8, GPIO_MODE::OUTPUT, GPIO_SPEED::LOW, 0);
        GPIO_SETH(C, 9, GPIO_MODE::OUTPUT, GPIO_SPEED::LOW, 0);
        GPIO_SETH(C, 10, GPIO_MODE::ALT_FUN, GPIO_SPEED::LOW, 6);   // clk
        GPIO_SETH(C, 11, GPIO_MODE::ALT_FUN, GPIO_SPEED::LOW, 6);   // miso
        GPIO_SETH(C, 12, GPIO_MODE::ALT_FUN, GPIO_SPEED::LOW, 6);   // mosi
        GPIOC->BSRR = GPIO_BSRR_BS7 | GPIO_BSRR_BS8 | GPIO_BSRR_BS9;

        SPI4->CR2 = (15 << SPI_CR2_DS_Pos);   // 16 bit
        // ORDER DEPENDANCE SPE set last
        SPI4->CR1 = SPI_CR1_MSTR | (3 << SPI_CR1_BR_Pos) | SPI_CR1_SSI | SPI_CR1_SSM | SPI_CR1_SPE;    // baud = clock/16
        GPIO_SETH(E, 9, GPIO_MODE::OUTPUT, GPIO_SPEED::LOW, 0);
        GPIO_SETH(E, 10, GPIO_MODE::OUTPUT, GPIO_SPEED::LOW, 0);
        GPIO_SETH(E, 11, GPIO_MODE::OUTPUT, GPIO_SPEED::LOW, 0);
        GPIO_SETH(E, 12, GPIO_MODE::ALT_FUN, GPIO_SPEED::LOW, 5);   // clk
        GPIO_SETH(E, 13, GPIO_MODE::ALT_FUN, GPIO_SPEED::LOW, 5);   // miso
        GPIO_SETH(E, 14, GPIO_MODE::ALT_FUN, GPIO_SPEED::LOW, 5);   // mosi
        GPIOE->BSRR = GPIO_BSRR_BS9 | GPIO_BSRR_BS10 | GPIO_BSRR_BS11;

        USART2->CR2 = 0 << USART_CR2_STOP_Pos | USART_CR2_CLKEN | USART_CR2_LBCL | USART_CR2_MSBFIRST;
        // note stop = 1 seems to have issues with receive
        USART2->CR3 = 1 << USART_CR3_RXFTCFG_Pos;
        USART2->BRR = CPU_FREQUENCY_HZ/10'000'000;
        USART2->CR1 = USART_CR1_FIFOEN | USART_CR1_TE | USART_CR1_RE | USART_CR1_UE;
        GPIO_SETL(D, 2, GPIO_MODE::OUTPUT, GPIO_SPEED::LOW, 0);
        GPIO_SETL(D, 3, GPIO_MODE::OUTPUT, GPIO_SPEED::LOW, 0);
        GPIO_SETL(D, 4, GPIO_MODE::OUTPUT, GPIO_SPEED::LOW, 0);
        GPIO_SETL(D, 5, GPIO_MODE::ALT_FUN, GPIO_SPEED::MEDIUM, 7);   // mosi
        GPIO_SETL(D, 6, GPIO_MODE::ALT_FUN, GPIO_SPEED::MEDIUM, 7);   // miso
        GPIO_SETL(D, 7, GPIO_MODE::ALT_FUN, GPIO_SPEED::MEDIUM, 7);   // clk
        GPIOD->BSRR = GPIO_BSRR_BS2 | GPIO_BSRR_BS3 | GPIO_BSRR_BS4;
    }
};

namespace config {
    const uint32_t main_loop_frequency = 1000;    
    const uint32_t pwm_frequency = 20000;
    InitCode init_code;

    MotorEncoder motor_encoder;
    TorqueSensor torque_sensor;
    OutputEncoder output_encoder;

    SPI spi1 {*SPI1};
    SPI spi2 {*SPI2};
    SPI spi3 {*SPI3};
    SPI spi4 {*SPI4};
    USART_SPI usart_spi2 {*USART2};

    GPIO gpio_cs_a1(*GPIOA, 1, GPIO::OUTPUT);
    MA782Encoder ma782_a1(spi1, gpio_cs_a1, SPIDMA::spi_pause[SPIDMA::SP1], MA782Encoder<SPI>::_4096);
    GPIO gpio_cs_a2(*GPIOA, 2, GPIO::OUTPUT);
    MA782Encoder ma782_a2(spi1, gpio_cs_a2, SPIDMA::spi_pause[SPIDMA::SP1], MA782Encoder<SPI>::_4096);
    GPIO gpio_cs_a3(*GPIOA, 3, GPIO::OUTPUT);
    MA782Encoder ma782_a3(spi1, gpio_cs_a3, SPIDMA::spi_pause[SPIDMA::SP1], MA782Encoder<SPI>::_4096);
    GPIO gpio_cs_a4(*GPIOA, 4, GPIO::OUTPUT);
    MA782Encoder ma782_a4(spi1, gpio_cs_a4, SPIDMA::spi_pause[SPIDMA::SP1], MA782Encoder<SPI>::_4096);

    GPIO gpio_cs_b1(*GPIOE, 9, GPIO::OUTPUT);
    MA782Encoder ma782_b1(spi4, gpio_cs_b1, SPIDMA::spi_pause[SPIDMA::SP4], MA782Encoder<SPI>::_4096);
    GPIO gpio_cs_b2(*GPIOE, 10, GPIO::OUTPUT);
    MA782Encoder ma782_b2(spi4, gpio_cs_b2, SPIDMA::spi_pause[SPIDMA::SP4], MA782Encoder<SPI>::_4096);
    GPIO gpio_cs_b3(*GPIOE, 11, GPIO::OUTPUT);
    MA782Encoder ma782_b3(spi4, gpio_cs_b3, SPIDMA::spi_pause[SPIDMA::SP4], MA782Encoder<SPI>::_4096);

    GPIO gpio_cs_c1(*GPIOB, 10, GPIO::OUTPUT);
    MA782Encoder ma782_c1(spi2, gpio_cs_c1, SPIDMA::spi_pause[SPIDMA::SP2], MA782Encoder<SPI>::_4096);
    GPIO gpio_cs_c2(*GPIOB, 11, GPIO::OUTPUT);
    MA782Encoder ma782_c2(spi2, gpio_cs_c2, SPIDMA::spi_pause[SPIDMA::SP2], MA782Encoder<SPI>::_4096);
    GPIO gpio_cs_c3(*GPIOB, 12, GPIO::OUTPUT);
    MA782Encoder ma782_c3(spi2, gpio_cs_c3, SPIDMA::spi_pause[SPIDMA::SP2], MA782Encoder<SPI>::_4096);

    GPIO gpio_cs_d1(*GPIOC, 7, GPIO::OUTPUT);
    MA782Encoder ma782_d1(spi3, gpio_cs_d1, SPIDMA::spi_pause[SPIDMA::SP3], MA782Encoder<SPI>::_4096);
    GPIO gpio_cs_d2(*GPIOC, 8, GPIO::OUTPUT);
    MA782Encoder ma782_d2(spi3, gpio_cs_d2, SPIDMA::spi_pause[SPIDMA::SP3], MA782Encoder<SPI>::_4096);
    GPIO gpio_cs_d3(*GPIOC, 9, GPIO::OUTPUT);
    MA782Encoder ma782_d3(spi3, gpio_cs_d3, SPIDMA::spi_pause[SPIDMA::SP3], MA782Encoder<SPI>::_4096);

    GPIO gpio_cs_e1(*GPIOD, 2, GPIO::OUTPUT);
    MA782Encoder ma782_e1(usart_spi2, gpio_cs_e1, SPIDMA::spi_pause[SPIDMA_USART::SPI_USART2], MA782Encoder<USART_SPI>::_4096);
    GPIO gpio_cs_e2(*GPIOD, 3, GPIO::OUTPUT);
    MA782Encoder ma782_e2(usart_spi2, gpio_cs_e2, SPIDMA::spi_pause[SPIDMA_USART::SPI_USART2], MA782Encoder<USART_SPI>::_4096);
    GPIO gpio_cs_e3(*GPIOD, 4, GPIO::OUTPUT);
    MA782Encoder ma782_e3(usart_spi2, gpio_cs_e3, SPIDMA::spi_pause[SPIDMA_USART::SPI_USART2], MA782Encoder<USART_SPI>::_4096);
};

#include "../../motorlib/boards/config_obot_g474_trace.cpp"

void config_init() {
    config::ma782_a1.add_debug_variables<"ma1">(System::api, System::communication_);
    config::ma782_a2.add_debug_variables<"ma2">(System::api, System::communication_);
    config::ma782_a3.add_debug_variables<"ma3">(System::api, System::communication_);
    config::ma782_a4.add_debug_variables<"ma4">(System::api, System::communication_);

    config::ma782_b1.add_debug_variables<"mb1">(System::api, System::communication_);
    config::ma782_b2.add_debug_variables<"mb2">(System::api, System::communication_);
    config::ma782_b3.add_debug_variables<"mb3">(System::api, System::communication_);

    config::ma782_c1.add_debug_variables<"mc1">(System::api, System::communication_);
    config::ma782_c2.add_debug_variables<"mc2">(System::api, System::communication_);
    config::ma782_c3.add_debug_variables<"mc3">(System::api, System::communication_);

    config::ma782_d1.add_debug_variables<"md1">(System::api, System::communication_);
    config::ma782_d2.add_debug_variables<"md2">(System::api, System::communication_);
    config::ma782_d3.add_debug_variables<"md3">(System::api, System::communication_);

    config::ma782_e1.add_debug_variables<"me1">(System::api, System::communication_);
    config::ma782_e2.add_debug_variables<"me2">(System::api, System::communication_);
    config::ma782_e3.add_debug_variables<"me3">(System::api, System::communication_);

    config::ma782_a1.init();
    IWDG->KR = 0xAAAA;
    config::ma782_a2.init();
    IWDG->KR = 0xAAAA;
    config::ma782_a3.init();
    IWDG->KR = 0xAAAA;
    config::ma782_a4.init();
    IWDG->KR = 0xAAAA;

    config::ma782_b1.init();
    IWDG->KR = 0xAAAA;
    config::ma782_b2.init();
    IWDG->KR = 0xAAAA;
    config::ma782_b3.init();
    IWDG->KR = 0xAAAA;

    config::ma782_c1.init();
    IWDG->KR = 0xAAAA;
    config::ma782_c2.init();
    IWDG->KR = 0xAAAA;
    config::ma782_c3.init();
    IWDG->KR = 0xAAAA;

    config::ma782_d1.init();
    IWDG->KR = 0xAAAA;
    config::ma782_d2.init();
    IWDG->KR = 0xAAAA;
    config::ma782_d3.init();
    IWDG->KR = 0xAAAA;

    config::ma782_e1.init();
    IWDG->KR = 0xAAAA;
    config::ma782_e2.init();
    IWDG->KR = 0xAAAA;
    config::ma782_e3.init();
    IWDG->KR = 0xAAAA;

    System::communication_.set_send_decimation(2);
}

void config_maintenance() {}

// void load_send_data(const MainLoop &main_loop, SendData * const data) {
//     config::ma782_a1.trigger();
//     config::ma782_b1.trigger();
//     config::ma782_c1.trigger();
//     config::ma782_d1.trigger();
//     config::ma782_e1.trigger();

//     data->encoder[3] = config::ma782_a1.read();
//     data->encoder[6] = config::ma782_b1.read();
//     data->encoder[9] = config::ma782_c1.read();
//     data->encoder[12] = config::ma782_d1.read();
//     data->encoder[15] = config::ma782_e1.read();

//     config::ma782_a2.trigger();
//     config::ma782_b2.trigger();
//     config::ma782_c2.trigger();
//     config::ma782_d2.trigger();
//     config::ma782_e2.trigger();

//     data->encoder[2] = config::ma782_a2.read();
//     data->encoder[5] = config::ma782_b2.read();
//     data->encoder[8] = config::ma782_c2.read();
//     data->encoder[11] = config::ma782_d2.read();
//     data->encoder[14] = config::ma782_e2.read();

//     config::ma782_a3.trigger();
//     config::ma782_b3.trigger();
//     config::ma782_c3.trigger();
//     config::ma782_d3.trigger();
//     config::ma782_e3.trigger();

//     data->encoder[1] = config::ma782_a3.read();
//     data->encoder[4] = config::ma782_b3.read();
//     data->encoder[7] = config::ma782_c3.read();
//     data->encoder[10] = config::ma782_d3.read();
//     data->encoder[13] = config::ma782_e3.read();

//     config::ma782_a4.trigger();
//     data->encoder[0] = config::ma782_a4.read();

// }
