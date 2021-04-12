#pragma once

#include "stm32g474xx.h"
#include "../../motorlib/peripheral/stm32g4/pin_config.h"

#define I_A_DR  ADC3->JDR1
#define I_B_DR  ADC4->JDR1
#define I_C_DR  ADC5->JDR1
#define V_BUS_DR ADC1->DR
#define V_REF_DR ADC1->JDR2
#define V_TEMP_DR ADC1->JDR1
#define A1_DR ADC1->JDR3
#define A2_DR ADC1->JDR4

#define TIM_R TIM4->CCR1
#define TIM_G TIM4->CCR2
#define TIM_B TIM4->CCR3
#ifdef R0
    #undef TIM_R
    #undef TIM_G
    #undef TIM_B
    #define TIM_R TIM4->CCR1
    #define TIM_G TIM4->CCR3
    #define TIM_B TIM4->CCR2
#endif


void pin_config_freebot_g474_motor_r0() {
     // Peripheral clock enable
        RCC->APB1ENR1 = RCC_APB1ENR1_SPI3EN | RCC_APB1ENR1_TIM2EN |  RCC_APB1ENR1_TIM4EN | RCC_APB1ENR1_TIM5EN | RCC_APB1ENR1_USBEN;
        RCC->APB2ENR |= RCC_APB2ENR_SPI1EN | RCC_APB2ENR_TIM1EN | RCC_APB2ENR_HRTIM1EN | RCC_APB2ENR_SYSCFGEN;
        RCC->AHB1ENR |= RCC_AHB1ENR_DMA1EN | RCC_AHB1ENR_DMAMUX1EN;
        RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN | RCC_AHB2ENR_GPIOBEN | RCC_AHB2ENR_GPIOCEN | RCC_AHB2ENR_GPIODEN | RCC_AHB2ENR_ADC12EN | RCC_AHB2ENR_ADC345EN;

        CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
        DWT->CYCCNT = 0;
        DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;

        FLASH->ACR |= FLASH_ACR_PRFTEN;
        MASK_SET(FLASH->ACR, FLASH_ACR_LATENCY, 4);

        // GPIO configure
        GPIO_SETL(A, 0, GPIO_MODE::ALT_FUN, GPIO_SPEED::VERY_HIGH, 1);   // QEPA TIM2
        GPIO_SETL(A, 1, GPIO_MODE::ALT_FUN, GPIO_SPEED::VERY_HIGH, 1);   // QEPB TIM2
        GPIO_SETL(A, 2, GPIO_MODE::ALT_FUN, GPIO_SPEED::VERY_HIGH, 1);   // QEPI TIM2
        //GPIO_SETL(A, 0, 1, 3, 0);   // SPI1 CS on QEPA pin
        GPIO_SETL(A, 4, GPIO_MODE::ALT_FUN, GPIO_SPEED::VERY_HIGH, 5);   // SPI1 CS drv8323s
        GPIO_SETL(A, 5, GPIO_MODE::ALT_FUN, GPIO_SPEED::VERY_HIGH, 5);   // SPI1 CLK drv8323s
        GPIO_SETL(A, 6, GPIO_MODE::ALT_FUN, GPIO_SPEED::VERY_HIGH, 5);   // SPI1 HIDO (host in device out) drv8323s
        GPIO_SETL(A, 7, GPIO_MODE::ALT_FUN, GPIO_SPEED::VERY_HIGH, 5);   // SPI1 HODI drv8323s
        MASK_SET(GPIOA->PUPDR, GPIO_PUPDR_PUPD6, 1); // HIDO pull up

        GPIO_SETH(B, 14, GPIO_MODE::ALT_FUN, GPIO_SPEED::VERY_HIGH, 13); // hrtim1 chd1
        GPIO_SETH(B, 15, GPIO_MODE::ALT_FUN, GPIO_SPEED::VERY_HIGH, 13); // hrtim1 chd2 
        GPIO_SETL(C, 6, GPIO_MODE::ALT_FUN, GPIO_SPEED::VERY_HIGH, 13);  // hrtim1 chf1
        GPIO_SETL(C, 7, GPIO_MODE::ALT_FUN, GPIO_SPEED::VERY_HIGH, 13);  // hrtim1 chf2
        GPIO_SETH(C, 8, GPIO_MODE::ALT_FUN, GPIO_SPEED::VERY_HIGH, 3);   // hrtim1 che1
        GPIO_SETH(C, 9, GPIO_MODE::ALT_FUN, GPIO_SPEED::VERY_HIGH, 3);   // hrtim1 che2

        GPIO_SETH(A, 11, GPIO_MODE::ANALOG, GPIO_SPEED::LOW, 0);         // usb dm
        GPIO_SETH(A, 12, GPIO_MODE::ANALOG, GPIO_SPEED::LOW, 0);         // usb dp
        GPIO_SETH(A, 10, GPIO_MODE::INPUT, GPIO_SPEED::LOW, 0);          // EXTI trigger for usb 
        MASK_SET(SYSCFG->EXTICR[2], SYSCFG_EXTICR3_EXTI10, 0);           // EXTI PA10
        EXTI->RTSR1 = EXTI_RTSR1_RT10;                                   // rising and falling triggers
        EXTI->FTSR1 = EXTI_FTSR1_FT10;
        EXTI->IMR1 = EXTI_IMR1_IM10;                                     // interrupt unmasked, but not enabled in NVIQ

        // LED
        GPIO_SETL(B, 6, GPIO_MODE::ALT_FUN, GPIO_SPEED::VERY_HIGH, 2);      // TIM4 CH1
        GPIO_SETL(B, 7, GPIO_MODE::ALT_FUN, GPIO_SPEED::VERY_HIGH, 2);      // TIM4 CH2
        GPIO_SETH(B, 8, GPIO_MODE::ALT_FUN, GPIO_SPEED::VERY_HIGH, 2);      // TIM4 CH3
        TIM4->CCMR1 = TIM_CCMR1_OC1PE | 6 << TIM_CCMR1_OC1M_Pos |   // preload and pwm mode 1
                        TIM_CCMR1_OC2PE | 6 << TIM_CCMR1_OC2M_Pos;
        TIM4->CCMR2 = TIM_CCMR2_OC3PE | 6 << TIM_CCMR2_OC3M_Pos;
        TIM4->CCER = TIM_CCER_CC1E | TIM_CCER_CC2E | TIM_CCER_CC3E; // enable
        TIM4->CR1 = TIM_CR1_CEN;
     
        // spi 3
        GPIO_SETH(C, 10, GPIO_MODE::ALT_FUN, GPIO_SPEED::VERY_HIGH, 6);   // SPI3 CLK
        GPIO_SETH(C, 11, GPIO_MODE::ALT_FUN, GPIO_SPEED::VERY_HIGH, 6);   // SPI3 HIDO
        GPIO_SETH(C, 12, GPIO_MODE::ALT_FUN, GPIO_SPEED::VERY_HIGH, 6);   // SPI3 HODI 

        GPIO_SETL(D, 2, GPIO_MODE::OUTPUT, GPIO_SPEED::VERY_HIGH, 0);   // spi3 cs

        GPIO_SETH(C, 13, 1, 0, 0);  // Boostxl enable

        // TIM1 main loop interrupt        
        static_assert(CPU_FREQUENCY_HZ / config::main_loop_frequency < 65536, "Main loop frequency too low");
        TIM1->ARR = CPU_FREQUENCY_HZ / config::main_loop_frequency - 1;
        TIM1->DIER = TIM_DIER_UIE;
        NVIC_SetPriority(TIM1_UP_TIM16_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 1, 0));
        NVIC_EnableIRQ(TIM1_UP_TIM16_IRQn);

        // ADC
        // ADC1
        GPIO_SETL(C, 0, GPIO_MODE::ANALOG, GPIO_SPEED::LOW, 0); // A1
        GPIO_SETL(C, 1, GPIO_MODE::ANALOG, GPIO_SPEED::LOW, 0); // A2
        GPIO_SETL(C, 2, GPIO_MODE::ANALOG, GPIO_SPEED::LOW, 0); // A3
        ADC12_COMMON->CCR = ADC_CCR_VSENSESEL | ADC_CCR_VREFEN | 3 << ADC_CCR_CKMODE_Pos; // hclk/4 (42.5 MHz)
        ADC1->SQR1 = 13 << ADC_SQR1_SQ1_Pos;    // vbus on opamp
        ADC1->JSQR = 3 << ADC_JSQR_JL_Pos | 16 << ADC_JSQR_JSQ1_Pos | 18 << ADC_JSQR_JSQ2_Pos | 6 << ADC_JSQR_JSQ3_Pos 
            | 7 << ADC_JSQR_JSQ4_Pos; // internal temperature, vrefint, A1, A2
    
        
        ADC1->CFGR = ADC_CFGR_JQDIS | ADC_CFGR_OVRMOD |1 << ADC_CFGR_EXTEN_Pos | 21 << ADC_CFGR_EXTSEL_Pos; // trigger 21 -> hrtim trig1
        ADC1->CFGR2 =  ADC_CFGR2_JOVSE | ADC_CFGR2_ROVSE | (8 << ADC_CFGR2_OVSS_Pos) | (7 << ADC_CFGR2_OVSR_Pos); // 256x oversample
        ADC1->SMPR1 = 6 << ADC_SMPR1_SMP6_Pos | // 247.5 cycles A1, 5.8us
                      6 << ADC_SMPR1_SMP7_Pos;  // 247.5 cycles A2, 5.8us
        ADC1->SMPR2 = 2 << ADC_SMPR2_SMP13_Pos | // 12.5 cycles vbus, 294 ns, 200 ns min for opamp1
                      6 << ADC_SMPR2_SMP16_Pos | // 247.5 cycles interal temperature, 5.8us, 5us min
                      6 << ADC_SMPR2_SMP18_Pos;  // 247.5 cycles vrefint (~1.21V), 5.8us, 4us min
        
        //ADC3,4,5
        ADC345_COMMON->CCR = ADC_CCR_VREFEN | 3 << ADC_CCR_CKMODE_Pos; // hclk/4 (42.5 MHz)

        ADC3->SMPR2 = 2 << ADC_SMPR2_SMP13_Pos; // 12.5 cycles current_sense, 294 ns, 200 ns min for opamp3
        ADC4->SMPR2 = 2 << ADC_SMPR2_SMP17_Pos; // 12.5 cycles current_sense, 294 ns, 200 ns min for opamp6
        ADC5->SMPR1 = 2 << ADC_SMPR1_SMP5_Pos; // 12.5 cycles current_sense, 294 ns, 200 ns min for opamp4
        ADC3->JSQR = 1 << ADC_JSQR_JEXTEN_Pos | 27 << ADC_JSQR_JEXTSEL_Pos | 13 << ADC_JSQR_JSQ1_Pos; // trig 27 hrtim adc_trig1 (injected)
        ADC4->JSQR = 1 << ADC_JSQR_JEXTEN_Pos | 27 << ADC_JSQR_JEXTSEL_Pos | 17 << ADC_JSQR_JSQ1_Pos; // trig 27 hrtim adc_trig1 (injected)
        ADC5->JSQR = 1 << ADC_JSQR_JEXTEN_Pos | 27 << ADC_JSQR_JEXTSEL_Pos | 5 << ADC_JSQR_JSQ1_Pos;  // trig 27 hrtim adc_trig1 (injected)
        NVIC_SetPriority(ADC5_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 0, 0));
        NVIC_EnableIRQ(ADC5_IRQn);

        // OPAMP
        GPIO_SETL(A, 3, GPIO_MODE::ANALOG, GPIO_SPEED::LOW, 0);
        OPAMP1->CSR = 1 << OPAMP_CSR_VPSEL_Pos | 3 << OPAMP_CSR_VMSEL_Pos | OPAMP_CSR_HIGHSPEEDEN | OPAMP_CSR_OPAMPINTEN | OPAMP_CSR_OPAMPxEN; // internal follower on pa3/vinp1
        GPIO_SETH(B, 11, GPIO_MODE::ANALOG, GPIO_SPEED::LOW, 0);
        GPIO_SETH(B, 12, GPIO_MODE::ANALOG, GPIO_SPEED::LOW, 0);
        GPIO_SETH(B, 13, GPIO_MODE::ANALOG, GPIO_SPEED::LOW, 0);
        OPAMP4->CSR = 2 << OPAMP_CSR_VPSEL_Pos | 3 << OPAMP_CSR_VMSEL_Pos | OPAMP_CSR_HIGHSPEEDEN | OPAMP_CSR_OPAMPINTEN | OPAMP_CSR_OPAMPxEN; // internal follower on pb11/vinp2
        OPAMP6->CSR = 0 << OPAMP_CSR_VPSEL_Pos | 3 << OPAMP_CSR_VMSEL_Pos | OPAMP_CSR_HIGHSPEEDEN | OPAMP_CSR_OPAMPINTEN | OPAMP_CSR_OPAMPxEN; // internal follower on pb12/vinp0
        OPAMP3->CSR = 1 << OPAMP_CSR_VPSEL_Pos | 3 << OPAMP_CSR_VMSEL_Pos | OPAMP_CSR_HIGHSPEEDEN | OPAMP_CSR_OPAMPINTEN | OPAMP_CSR_OPAMPxEN; // internal follower on pb13/vinp1

        // USB
        NVIC_SetPriority(USB_LP_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 2, 0));
        NVIC_EnableIRQ(USB_LP_IRQn);

        // SPI1 DRV8323RS
        //GPIOA->BSRR = GPIO_ODR_OD0;  // disable other spi cs
        GPIOC->BSRR = GPIO_ODR_OD13; // drv enable
        ms_delay(10);

        SPI1->CR2 = (15 << SPI_CR2_DS_Pos) | SPI_CR2_FRF;   // 16 bit TI mode
        // ORDER DEPENDANCE SPE set last
        SPI1->CR1 = SPI_CR1_MSTR | (5 << SPI_CR1_BR_Pos) | SPI_CR1_SPE;    // baud = clock/64
        for (uint8_t i=0; i<sizeof(param->drv_regs)/sizeof(uint16_t); i++) {
            uint16_t reg_out = param->drv_regs[i];
            uint16_t reg_in = 0;
            SPI1->DR = reg_out;
            while(!(SPI1->SR & SPI_SR_RXNE));
            reg_in = SPI1->DR;

            reg_out |= (1<<15); // switch to read mode
            SPI1->DR = reg_out;
            while(!(SPI1->SR & SPI_SR_RXNE));
            reg_in = SPI1->DR;
            if ((reg_in & 0x7FF) != (reg_out & 0x7FF)) {
            drv_regs_error |= 1 << i;
            }
        }
        SPI1->CR1 = 0; // clear SPE
        // SPI1 CS-> gpio
        GPIO_SETL(A, 4, 1, 0, 0);
        GPIOA->BSRR = GPIO_ODR_OD4;

}

// return (fault status register 2 << 16) | (fault status register 1) 
uint32_t get_drv_status() {
        // pause main loop (due to overlap with torque sensor)
        TIM1->CR1 &= ~TIM_CR1_CEN;
        GPIO_SETL(A, 4, 2, 3, 5); 
        SPI1->CR1 = 0; // clear SPE
        SPI1->CR2 = (15 << SPI_CR2_DS_Pos) | SPI_CR2_FRF;   // 16 bit TI mode
        // ORDER DEPENDANCE SPE set last
        SPI1->CR1 = SPI_CR1_MSTR | (5 << SPI_CR1_BR_Pos) | SPI_CR1_SPE;    // baud = clock/64

        SPI1->DR = 1<<15; // fault status 1
        while(!(SPI1->SR & SPI_SR_RXNE));
        uint32_t reg_in = SPI1->DR;

        SPI1->DR = (1<<15) | (1<<11); // vgs status2
        while(!(SPI1->SR & SPI_SR_RXNE));
        reg_in |= SPI1->DR << 16;

        SPI1->CR1 = 0; // clear SPE
        // SPI1 CS-> gpio
        GPIO_SETL(A, 4, 1, 0, 0);
        GPIOA->BSRR = GPIO_ODR_OD4;

        // SPI1 ADS1235
        SPI1->CR1 = SPI_CR1_CPHA | SPI_CR1_MSTR | (4 << SPI_CR1_BR_Pos) | SPI_CR1_SSI | SPI_CR1_SSM | SPI_CR1_SPE;    // baud = clock/32
        SPI1->CR2 = (7 << SPI_CR2_DS_Pos) | SPI_CR2_FRXTH | SPI_CR2_RXDMAEN | SPI_CR2_TXDMAEN;    // 8 bit   

        // reenable main loop
        TIM1->CR1 = TIM_CR1_CEN;
        return reg_in;
}

void drv_reset(uint32_t blah) {
    GPIOC->BSRR = GPIO_BSRR_BR13; // drv enable
    ms_delay(10);
    GPIOC->BSRR = GPIO_BSRR_BS13; // drv enable
    ms_delay(10);
}
