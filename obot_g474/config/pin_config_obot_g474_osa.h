#pragma once

#include "stm32g474xx.h"
#include "../../motorlib/peripheral/stm32g4/pin_config.h"

#define I_A_DR  ADC5->JDR1
#define I_B_DR  ADC4->JDR1
#define I_C_DR  ADC3->JDR1
#define V_BUS_DR ADC1->DR
#define V_REF_DR ADC1->JDR2
#define V_TEMP_DR ADC1->JDR1
#define V_A_DR ADC2->JDR1
#define V_B_DR ADC2->JDR2
#define V_C_DR ADC2->JDR3
#define I_BUS_DR ADC1->JDR3

#define TIM_R TIM4->CCR1
#define TIM_G TIM4->CCR3
#define TIM_B TIM4->CCR2

// two drivers, 0x1 and 0x2, 0x3 to enable both
void mps_driver_enable(uint8_t drivers) {
    GPIOC->ODR = ((drivers & 0x01) ? GPIO_ODR_OD13 : 0) |
                  ((drivers & 0x02) ? GPIO_ODR_OD15 : 0);
}

uint8_t mps_driver_enable_status() {
    return (GPIOC->ODR & GPIO_ODR_OD13 ? 1 : 0) |
            (GPIOC->ODR & GPIO_ODR_OD15 ? 2 : 0);
}

bool is_mps_driver_faulted() {
    return !(GPIOC->IDR & GPIO_IDR_ID14);
}

void pin_config_obot_g474_osa() {
     // Peripheral clock enable
        RCC->APB1ENR1 = RCC_APB1ENR1_SPI3EN | RCC_APB1ENR1_TIM2EN |  RCC_APB1ENR1_TIM4EN | RCC_APB1ENR1_TIM5EN | RCC_APB1ENR1_USBEN | RCC_APB1ENR1_RTCAPBEN | RCC_APB1ENR1_PWREN |  RCC_APB1ENR1_I2C1EN | RCC_APB1ENR1_I2C2EN;
        RCC->APB2ENR |= RCC_APB2ENR_SPI1EN | RCC_APB2ENR_TIM1EN | RCC_APB2ENR_HRTIM1EN | RCC_APB2ENR_SYSCFGEN;
        RCC->AHB1ENR |= RCC_AHB1ENR_DMA1EN | RCC_AHB1ENR_DMAMUX1EN;
        RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN | RCC_AHB2ENR_GPIOBEN | RCC_AHB2ENR_GPIOCEN | RCC_AHB2ENR_GPIODEN | RCC_AHB2ENR_ADC12EN | RCC_AHB2ENR_ADC345EN;
        PWR->CR1 |= PWR_CR1_DBP;
        RCC->BDCR |= 2 << RCC_BDCR_RTCSEL_Pos | RCC_BDCR_RTCEN;

    // Sleep mode enable
        RCC->APB1SMENR1 = RCC_APB1SMENR1_TIM2SMEN |  RCC_APB1SMENR1_TIM4SMEN | RCC_APB1SMENR1_TIM5SMEN | RCC_APB1SMENR1_USBSMEN |  RCC_APB1SMENR1_RTCAPBSMEN;
        RCC->APB2SMENR = RCC_APB2SMENR_SYSCFGSMEN;
        RCC->AHB1SMENR = 0;
        RCC->AHB2SMENR = RCC_AHB2SMENR_GPIOASMEN | RCC_AHB2SMENR_GPIOBSMEN | RCC_AHB2SMENR_GPIOCSMEN | RCC_AHB2SMENR_GPIODSMEN;

        CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
        DWT->CYCCNT = 0;
        DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;

        FLASH->ACR |= FLASH_ACR_PRFTEN;
        MASK_SET(FLASH->ACR, FLASH_ACR_LATENCY, 4);

        // GPIO configure
        // general purpose/scope/qep
        // GPIO_SETL(A, 0, GPIO_MODE::OUTPUT, GPIO_SPEED::VERY_HIGH, 0);   // gpio2
        // GPIO_SETL(A, 1, GPIO_MODE::OUTPUT, GPIO_SPEED::VERY_HIGH, 0);   // gpio3
        // GPIO_SETL(A, 2, GPIO_MODE::OUTPUT, GPIO_SPEED::VERY_HIGH, 0);   // gpio4
        // as qep
        // GPIO_SETL(A, 0, GPIO_MODE::ALT_FUN, GPIO_SPEED::VERY_HIGH, 1);   // QEPA TIM2
        // GPIO_SETL(A, 1, GPIO_MODE::ALT_FUN, GPIO_SPEED::VERY_HIGH, 1);   // QEPB TIM2
        // GPIO_SETL(A, 2, GPIO_MODE::ALT_FUN, GPIO_SPEED::VERY_HIGH, 1);   // QEPI TIM2

        // i2c/scope
        GPIO_SETH(A, 8, GPIO_MODE::OUTPUT, GPIO_SPEED::VERY_HIGH, 0);   // i2c2_sda as gpio
        GPIO_SETH(A, 9, GPIO_MODE::OUTPUT, GPIO_SPEED::VERY_HIGH, 0);   // i2c2_scl as gpio
        GPIO_SETL(C, 5, GPIO_MODE::OUTPUT, GPIO_SPEED::VERY_HIGH, 0);   // scope_3 as gpio

        // spi 1
        GPIO_SETL(A, 4, GPIO_MODE::OUTPUT, GPIO_SPEED::VERY_HIGH, 0);   // SPI1 CS
        GPIO_SETL(A, 5, GPIO_MODE::ALT_FUN, GPIO_SPEED::VERY_HIGH, 5);   // SPI1 CLK
        GPIO_SETL(A, 6, GPIO_MODE::ALT_FUN, GPIO_SPEED::VERY_HIGH, 5);   // SPI1 HIDO (host in device out)
        GPIO_SETL(A, 7, GPIO_MODE::ALT_FUN, GPIO_SPEED::VERY_HIGH, 5);   // SPI1 HODI

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
        GPIO_SETH(C, 10, GPIO_MODE::ALT_FUN, GPIO_SPEED::LOW, 6);   // SPI3 CLK
        GPIO_SETH(C, 11, GPIO_MODE::ALT_FUN, GPIO_SPEED::LOW, 6);   // SPI3 HIDO
        GPIO_SETH(C, 12, GPIO_MODE::ALT_FUN, GPIO_SPEED::LOW, 6);   // SPI3 HODI 
        GPIO_SETL(D, 2, GPIO_MODE::OUTPUT, GPIO_SPEED::LOW, 0);   // spi3 cs1
        GPIO_SETL(B, 4, GPIO_MODE::OUTPUT, GPIO_SPEED::LOW, 0);   // spi3 cs2
        


        // TIM1 main loop interrupt        
        static_assert(CPU_FREQUENCY_HZ / config::main_loop_frequency/4 < 65536, "Main loop frequency too low");
        TIM1->ARR = CPU_FREQUENCY_HZ / config::main_loop_frequency - 1;
        TIM1->DIER = TIM_DIER_UIE;
        NVIC_SetPriority(TIM1_UP_TIM16_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 1, 0));
        NVIC_EnableIRQ(TIM1_UP_TIM16_IRQn);

        // RTC
        RTC->WPR = 0xCA;
        RTC->WPR = 0x53;
        RTC->ICSR = RTC_ICSR_INIT;
        // while(!(RTC->ICSR & RTC_ICSR_INITF));
        // RTC->PRER = 128 << RTC_PRER_PREDIV_A_Pos | 256 << RTC_PRER_PREDIV_S_Pos;
        while(!(RTC->ICSR & RTC_ICSR_WUTWF));
        RTC->WUTR = 205; // 0.1 seconds wakeup
        RTC->CR = RTC_CR_WUTE | RTC_CR_WUTIE;
        RTC->ICSR &= ~RTC_ICSR_INIT;
        EXTI->IMR1 |= EXTI_IMR1_IM20;
        EXTI->RTSR1 |= EXTI_RTSR1_RT20;
        NVIC_SetPriority(RTC_WKUP_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 0, 0));
        //NVIC_EnableIRQ(RTC_WKUP_IRQn);
        RTC->SCR = RTC_SCR_CWUTF;

        // ADC
        // ADC1
        GPIO_SETL(C, 3, GPIO_MODE::ANALOG, GPIO_SPEED::LOW, 0); // ibus
        ADC12_COMMON->CCR = ADC_CCR_VSENSESEL | ADC_CCR_VREFEN | 3 << ADC_CCR_CKMODE_Pos; // hclk/4 (42.5 MHz)
        ADC1->SQR1 = 13 << ADC_SQR1_SQ1_Pos;    // vbus on opamp
        ADC1->JSQR = 2 << ADC_JSQR_JL_Pos | 16 << ADC_JSQR_JSQ1_Pos | 18 << ADC_JSQR_JSQ2_Pos | 9 << ADC_JSQR_JSQ3_Pos; // internal temperature, vrefint, ibus
        
        ADC1->CFGR = ADC_CFGR_JQDIS | ADC_CFGR_OVRMOD |1 << ADC_CFGR_EXTEN_Pos | 21 << ADC_CFGR_EXTSEL_Pos; // trigger 21 -> hrtim trig1
        ADC1->CFGR2 =  ADC_CFGR2_JOVSE | ADC_CFGR2_ROVSE | (8 << ADC_CFGR2_OVSS_Pos) | (7 << ADC_CFGR2_OVSR_Pos); // 256x oversample
        ADC1->SMPR1 = 6 << ADC_SMPR1_SMP9_Pos;  // 247.5 cycles ibus, 5.8us
        ADC1->SMPR2 = 2 << ADC_SMPR2_SMP13_Pos | // 12.5 cycles vbus, 294 ns, 200 ns min for opamp1
                      6 << ADC_SMPR2_SMP16_Pos | // 247.5 cycles interal temperature, 5.8us, 5us min
                      6 << ADC_SMPR2_SMP18_Pos;  // 247.5 cycles vrefint (~1.21V), 5.8us, 4us min

        // ADC2 VA,VB,VC -> JDR1,2,3
        GPIO_SETL(C, 0, GPIO_MODE::ANALOG, GPIO_SPEED::LOW, 0); // VA
        GPIO_SETL(C, 1, GPIO_MODE::ANALOG, GPIO_SPEED::LOW, 0); // VB
        GPIO_SETL(C, 2, GPIO_MODE::ANALOG, GPIO_SPEED::LOW, 0); // VC
        ADC2->JSQR = 2 << ADC_JSQR_JL_Pos | 6 << ADC_JSQR_JSQ1_Pos | 7 << ADC_JSQR_JSQ2_Pos | 8 << ADC_JSQR_JSQ3_Pos | 1 << ADC_JSQR_JEXTEN_Pos | 19 << ADC_JSQR_JEXTSEL_Pos; // trig 19 hrtim_adc_trg2 (injected)
        ADC2->CFGR = ADC_CFGR_JQDIS | ADC_CFGR_OVRMOD |1 << ADC_CFGR_EXTEN_Pos | 21 << ADC_CFGR_EXTSEL_Pos; // trigger 21 -> hrtim trig1
        ADC2->SMPR1 = 2 << ADC_SMPR1_SMP6_Pos | // 12.5 cycles VA, 5.8us
                      2 << ADC_SMPR1_SMP7_Pos | // 12.5 cycles VB, 5.8us
                      2 << ADC_SMPR1_SMP8_Pos;  // 12.5 cycles VC, 5.8us
        
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

        // I2C1
        GPIO_SETH(A, 15, GPIO_MODE::ALT_FUN, GPIO_SPEED::LOW, 4);   // i2c1 scl
        GPIO_SETH(B, 9, GPIO_MODE::ALT_FUN, GPIO_SPEED::LOW, 4);   // i2c1 sda
        MASK_SET(GPIOA->PUPDR, GPIO_PUPDR_PUPD15, GPIO_PULL::UP);
        MASK_SET(GPIOB->PUPDR, GPIO_PUPDR_PUPD9, GPIO_PULL::UP);
        MASK_SET(GPIOA->OTYPER, GPIO_OTYPER_OT15, 1);       // open drain
        MASK_SET(GPIOB->OTYPER, GPIO_OTYPER_OT9, 1);
        SYSCFG->CFGR1 |= SYSCFG_CFGR1_I2C1_FMP | SYSCFG_CFGR1_I2C2_FMP | SYSCFG_CFGR1_I2C_PB9_FMP;  // fast mode plus (1 MHz)

        // mps driver gpio    
        // fault pin input with pull up
        GPIO_SETH(C, 14, GPIO_MODE::INPUT, GPIO_SPEED::LOW, 0);
        MASK_SET(GPIOC->PUPDR, GPIO_PUPDR_PUPD14, 1);
        // nsleep pins are enable, set as outputs
        GPIO_SETH(C, 13, GPIO_MODE::OUTPUT, GPIO_SPEED::LOW, 0);
        GPIO_SETH(C, 15, GPIO_MODE::OUTPUT, GPIO_SPEED::LOW, 0);
        mps_driver_enable(3);
}

extern "C" void RTC_WKUP_IRQHandler() {
    IWDG->KR = 0xAAAA;
    static int count = 0;
    count++;
    EXTI->PR1 = EXTI_PR1_PIF20;
    RTC->SCR = RTC_SCR_CWUTF;
}

void setup_sleep() {
    NVIC_DisableIRQ(TIM1_UP_TIM16_IRQn);
    NVIC_DisableIRQ(ADC5_IRQn);
    mps_driver_enable(0);
    NVIC_SetPriority(USB_LP_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 0, 1));
    NVIC_EnableIRQ(RTC_WKUP_IRQn);
    MASK_SET(RCC->CFGR, RCC_CFGR_SW, 2); // HSE is system clock source
    RTC->SCR = RTC_SCR_CWUTF;
}

void finish_sleep() {
    MASK_SET(RCC->CFGR, RCC_CFGR_SW, 3); // PLL is system clock source
    mps_driver_enable(3);
    NVIC_DisableIRQ(RTC_WKUP_IRQn);
    NVIC_SetPriority(USB_LP_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 2, 0));
    NVIC_EnableIRQ(TIM1_UP_TIM16_IRQn);
    NVIC_EnableIRQ(ADC5_IRQn);
}