#pragma once

#include "stm32g474xx.h"
#include "../../motorlib/peripheral/stm32g4/pin_config.h"

#define I_A_DR  ADC3->JDR1
#define I_B_DR  ADC4->JDR1
#define I_C_DR  ADC5->JDR1
#define V_BUS_DR ADC1->JDR3
#define V_3V3_BUS ADC1->DR
#define V_REF_DR ADC1->JDR2
#define V_TEMP_DR ADC1->JDR1

#define TIM_G TIM4->CCR1
#define TIM_R TIM4->CCR2
#define TIM_B TIM4->CCR4

inline void set_adc_seq(ADC_TypeDef* adc_base, uint8_t ch, uint8_t seq_n){
        const uint32_t pos_table[] = {ADC_JSQR_JSQ1_Pos, ADC_JSQR_JSQ2_Pos, ADC_JSQR_JSQ3_Pos, ADC_JSQR_JSQ4_Pos};
        const uint32_t mask_table[] = {ADC_JSQR_JSQ1_Msk, ADC_JSQR_JSQ2_Msk, ADC_JSQR_JSQ3_Msk, ADC_JSQR_JSQ4_Msk};
        adc_base->JSQR &= ~(mask_table[seq_n]);
        adc_base->JSQR |= ch << pos_table[seq_n];

}

void pin_config_obot_g474_h2() {


        // Peripheral clock enable
        RCC->APB1ENR1 = RCC_APB1ENR1_SPI3EN | RCC_APB1ENR1_TIM2EN |  RCC_APB1ENR1_TIM4EN | RCC_APB1ENR1_TIM5EN | RCC_APB1ENR1_USBEN | RCC_APB1ENR1_I2C1EN | RCC_APB1ENR1_RTCAPBEN | RCC_APB1ENR1_PWREN;
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
 

        /***************************************************
          RTC Configuration
        ***************************************************/

        /* // Enable Peripheral Clock */
        /* RCC->APB1SMENR1 |= RCC_APB1ENR1_RTCAPBEN; */
        /* // RTC and Backup Register Enable */
        /* PWR->CR1 |= PWR_CR1_DBP; */
        /* // LSI Clock Source and enable RTC clock */
        /* RCC->BDCR |= 2 << RCC_BDCR_RTCSEL_Pos | RCC_BDCR_RTCEN; */

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

        
 
        /***************************************************
          SPI1 Configuration

          SPI1 - Magnetic Encoder MA782:
          ~CS  - PA4
          SCLK - PA5
          MISO - PA6
          MOSI - PA7
          
        ***************************************************/

        //Enable Peripheral Clock
        /* RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN; */
        /* RCC->APB2ENR |= RCC_APB2ENR_SPI1EN; */

        GPIO_SETL(A, 4, GPIO_MODE::OUTPUT,  GPIO_SPEED::HIGH, 0);    // SPI1 CS1
        GPIO_SETL(A, 5, GPIO_MODE::ALT_FUN, GPIO_SPEED::HIGH, 5);   // SPI1 CLK
        GPIO_SETL(A, 6, GPIO_MODE::ALT_FUN, GPIO_SPEED::HIGH, 5);   // SPI1 MISO
        GPIO_SETL(A, 7, GPIO_MODE::ALT_FUN, GPIO_SPEED::HIGH, 5);   // SPI1 MOSI
        MASK_SET(GPIOA->PUPDR, GPIO_PUPDR_PUPD6, 1); // MISO pull up


        /***************************************************
          LED Configuration

          PWM Timer - TIM4
          Red - PB7 - TIM4.CH2
          Green - PB6 - TIM4.CH1
          Blue - PB9 - TIM4.CH4

        ***************************************************/
        /* //Enable Peripheral Clock */
        /* RCC->AHB2RSTR |= RCC_AHB2ENR_GPIOBEN; */
        /* RCC->APB1ENR2 |= RCC_APB1ENR1_TIM4EN; */

        // GPIO Configuration
        GPIO_SETL(B, 7, GPIO_MODE::ALT_FUN, GPIO_SPEED::VERY_HIGH, 2);      // TIM4 CH2 red
        GPIO_SETL(B, 6, GPIO_MODE::ALT_FUN, GPIO_SPEED::VERY_HIGH, 2);      // TIM4 CH1 green
        GPIO_SETH(B, 9, GPIO_MODE::ALT_FUN, GPIO_SPEED::VERY_HIGH, 2);      // TIM4 CH4 blue

        // Timer Configuration
        TIM4->CCMR1 = TIM_CCMR1_OC1PE | 7 << TIM_CCMR1_OC1M_Pos |   // preload and pwm mode 2
                        TIM_CCMR1_OC2PE | 7 << TIM_CCMR1_OC2M_Pos;
        TIM4->CCMR2 = TIM_CCMR2_OC4PE | 7 << TIM_CCMR2_OC4M_Pos;
        TIM4->CCER = TIM_CCER_CC1E | TIM_CCER_CC2E | TIM_CCER_CC4E; // enable
        TIM4->CR1 = TIM_CR1_CEN;
     

        /***************************************************
          Main Loop Timer Configuration

          TIM1

        ***************************************************/

        /* // Enable Peripheral Clock */
        /* RCC->APB2ENR |= RCC_APB2ENR_TIM1EN; */
        
        // TIM1 main loop interrupt        
        static_assert(CPU_FREQUENCY_HZ / config::main_loop_frequency < 65536, "Main loop frequency too low");
        TIM1->ARR = CPU_FREQUENCY_HZ / config::main_loop_frequency - 1;
        TIM1->DIER = TIM_DIER_UIE;
        NVIC_SetPriority(TIM1_UP_TIM16_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 1, 0));
        NVIC_EnableIRQ(TIM1_UP_TIM16_IRQn);


        /***************************************************
          HRPWM Configuration

          Motor Phases:
          PWMA - PB14
          PWMB - PB15
          PWMC - PA8


        ***************************************************/

        /* // Enable Peripheral Clocks */
        /* RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN; */
        /* RCC->AHB2RSTR |= RCC_AHB2ENR_GPIOBEN; */
        /* RCC->APB2SMENR |= RCC_APB2ENR_HRTIM1EN; */

        GPIO_SETH(B, 14, GPIO_MODE::ALT_FUN, GPIO_SPEED::VERY_HIGH, 13); // hrtim1 chd1
        GPIO_SETH(B, 15, GPIO_MODE::ALT_FUN, GPIO_SPEED::VERY_HIGH, 13); // hrtim1 chd2
        GPIO_SETH(A, 8, GPIO_MODE::ALT_FUN, GPIO_SPEED::VERY_HIGH, 13);  // hrtim1 cha1
        GPIO_SETH(A, 9, GPIO_MODE::ALT_FUN, GPIO_SPEED::VERY_HIGH, 13);  // hrtim1 cha2, pwm enable is managed through hrtim


        // driver fault and enable
        GPIO_SETH(C, 13, GPIO_MODE::OUTPUT, GPIO_SPEED::LOW, 0);  // drv enable
        GPIO_SETH(C, 14, GPIO_MODE::INPUT, GPIO_SPEED::LOW, 0);  // drv fault
        MASK_SET(GPIOC->PUPDR, GPIO_PUPDR_PUPD14, GPIO_PULL::UP);

        /***************************************************
          ++++++++++++++++++++++++++++++++++++++++++++++++++
          Analog Configuration Summary
          ++++++++++++++++++++++++++++++++++++++++++++++++++

          Motor Currents:
          IA - PB13 -> OPAMP3.VINP -> ADC3.13
          IB - PB12 -> OPAMP6.VINP -> ADC4.17
          IC - PB11 -> OPAMP4.VINP -> ADC5.5

          Motor Bus Voltage:
          VSNS_BUS - PB0 - ADC3.12 or ADC1.15 - COMP4.INP - OPAMP2.VINP [or OPAMP3.VINP]
           + OPAMP2.VOUT Internal [VSNS_BUS] - ADC2.16


          Logic Bus Voltage:
          VSNS_3V3 - PB1 - ADC1.12 [or ADC3.1]

          Force Sensors:
          FS_SG1 - PA0 - ADC12.1
          FS_SG2 - PA3 - ADC1.4


        ***************************************************/

        enum Adc1Channels{
                strainGauge1 = 1,
                strainGauge2 = 4,
                V3v3 = 12,
                /* Vbus = 15, */
                Vt = 16,
                VrefInt = 18,
        };

        #define ADC1_CH_STRGAUGE1 1
        #define ADC1_CH_STRGAUGE2 4
        #define ADC1_CH_V3V3    12
        #define ADC1_CH_VT      16
        #define ADC1_CH_VREFINT 18
        #define ADC1_CH_VBUS 15
        /* ADD_CH2SEQ(ADC1_CH_STRGAUGE1, 1); */

        enum Adc2Channels{
                Vbus = 16
        };

        enum Adc3Channels{
                Ia = 13
        };

        enum Adc4Channels{
                Ib = 17
        };

        enum Adc5Channels{
                Ic = 5
        };

        /* // Enable Peripheral Clocks */
        /* RCC->AHB2RSTR |= RCC_AHB2ENR_GPIOBEN; */
        /* RCC->AHB2ENR |=  RCC_AHB2ENR_ADC12EN | RCC_AHB2ENR_ADC345EN; */

        //VREFBUF->CSR = VREFBUF_CSR_ENVR | 2 << VREFBUF_CSR_VRS_Pos;
        // ADC1
        /* GPIO_SETL(C, 0, GPIO_MODE::ANALOG, GPIO_SPEED::LOW, 0); // A1 */
        /* GPIO_SETL(C, 1, GPIO_MODE::ANALOG, GPIO_SPEED::LOW, 0); // A2 */
        /* GPIO_SETL(C, 2, GPIO_MODE::ANALOG, GPIO_SPEED::LOW, 0); // A3 */



        /***************************************************
          VREFBUF

          *Currently Tied to VDDA (3V3)
        ***************************************************/


        /***************************************************
          ADC1
        ***************************************************/


        // Enable Internal Vtemp, Enable VREFINT, ADCclk = hclk/4 (42.5Mhz)
        ADC12_COMMON->CCR = ADC_CCR_VSENSESEL | ADC_CCR_VREFEN | 3 << ADC_CCR_CKMODE_Pos;
        // Flush Injected Queue, Enable Overrun, Rising Edge Ext Trigger & Regular Sample Trigger Source = trigger 21 -> hrtim_adc_trg1
        ADC1->CFGR = ADC_CFGR_JQDIS | ADC_CFGR_OVRMOD |1 << ADC_CFGR_EXTEN_Pos | 21 << ADC_CFGR_EXTSEL_Pos;
        // Injected Oversample Enable, Regular Oversample enable, Oversampling shift = 8-bits, Oversampling Ratio = 256x
        ADC1->CFGR2 =  ADC_CFGR2_JOVSE | ADC_CFGR2_ROVSE | (8 << ADC_CFGR2_OVSS_Pos) | (7 << ADC_CFGR2_OVSR_Pos);

        // Regular Samples
        ADC1->SQR1 = 12 << ADC_SQR1_SQ1_Pos;
        /* ADC1->SQR1 = 12 <<  ADC_SQR1_SQ1_Pos; */

        // Injected Samples
        /* ADC1->JSQR = Adc1Channels::Vt << ADC_JSQR_JSQ1_Pos | */

        /*              Adc1Channels::VrefInt << ADC_JSQR_JSQ2_Pos | */

        /*              /1* ADC1_STRGAUGE1 << ADC_JSQR_JSQ3_Pos | *1/ */
        /*              /1* ADC1_STRGAUGE2 << ADC_JSQR_JSQ4_Pos | *1/ */
        /*              3 << ADC_JSQR_JL_Pos; // 4 Conversions in Queue */
        set_adc_seq(ADC1, ADC1_CH_VT, 0);
        set_adc_seq(ADC1, ADC1_CH_VREFINT, 1);
        set_adc_seq(ADC1, ADC1_CH_VBUS, 2);
        ADC1->JSQR |= 2 << ADC_JSQR_JL_Pos;


        // Sample Time Configuration
        ADC1->SMPR1 = 6 << ADC_SMPR1_SMP1_Pos; // 247.5 cycles A1, 5.8us
        // ADC1->SMPR2 = 2 << ADC_SMPR2_SMP13_Pos | // 12.5 cycles vbus, 294 ns, 200 ns min for opamp1
        ADC1->SMPR2 = 6 << ADC_SMPR2_SMP16_Pos | // 247.5 cycles interal temperature, 5.8us, 5us min
                      6 << ADC_SMPR2_SMP18_Pos |  // 247.5 cycles vrefint (~1.21V), 5.8us, 4us min
                      6 << ADC_SMPR2_SMP15_Pos;  // 247.5 cycles vrefint (~1.21V), 5.8us, 4us min

        /***************************************************
          ADC2

        ***************************************************/

        // ADC2 A3->JDR1
        // GPIO_SETL(C, 2, GPIO_MODE::ANALOG, GPIO_SPEED::LOW, 0); // VC
        // Flush Injected Queue, Enable Overrun, Rising Edge Ext Trigger & Trigger Source = trigger 21 -> hrtim_adc_trg1
        ADC2->CFGR = ADC_CFGR_JQDIS | ADC_CFGR_OVRMOD |1 << ADC_CFGR_EXTEN_Pos | 21 << ADC_CFGR_EXTSEL_Pos;
        // Injected Oversample Enable, Regular Oversample enable, Oversampling shift = 8-bits, Oversampling Ratio = 256x
        ADC2->CFGR2 =  ADC_CFGR2_JOVSE | ADC_CFGR2_ROVSE | (8 << ADC_CFGR2_OVSS_Pos) | (7 << ADC_CFGR2_OVSR_Pos); // 256x oversample

        // Injected Sample Queue
        ADC2->JSQR = 8 << ADC_JSQR_JSQ1_Pos |
                     0 << ADC_JSQR_JL_Pos |
                     1 << ADC_JSQR_JEXTEN_Pos | 
                     19 << ADC_JSQR_JEXTSEL_Pos; // trig 19 hrtim_adc_trg2 (injected)
        // Sample Time Configuration
        ADC2->SMPR1 = 6 << ADC_SMPR1_SMP8_Pos;  // 247.5 cycles A3, 5.8us

        
        /***************************************************
          ADC3,4,5 Common
        ***************************************************/
        // Enable VREFINT, ADCclk = hclk/4 (42.5Mhz)
        ADC345_COMMON->CCR = ADC_CCR_VREFEN | 3 << ADC_CCR_CKMODE_Pos; // hclk/4 (42.5 MHz)

        /***************************************************
          ADC3
        ***************************************************/
        // Regular Sample Queue
        // Injected Sample Queue
        ADC3->JSQR = 1 << ADC_JSQR_JEXTEN_Pos |
                     27 << ADC_JSQR_JEXTSEL_Pos | // trig 27 hrtim adc_trig1 (injected)
                     13 << ADC_JSQR_JSQ1_Pos; 

        ADC3->SMPR2 = 2 << ADC_SMPR2_SMP13_Pos; // 12.5 cycles current_sense, 294 ns, 200 ns min for opamp3
        /***************************************************
          ADC4
        ***************************************************/
        // Injected Sample Queue
        ADC4->JSQR = 1 << ADC_JSQR_JEXTEN_Pos |
                     27 << ADC_JSQR_JEXTSEL_Pos | 
                     17 << ADC_JSQR_JSQ1_Pos; // trig 27 hrtim adc_trig1 (injected)
        ADC4->SMPR2 = 2 << ADC_SMPR2_SMP17_Pos; // 12.5 cycles current_sense, 294 ns, 200 ns min for opamp6
        /***************************************************
          ADC5
        ***************************************************/

        // Injected Sample Queue
        ADC5->JSQR = 1 << ADC_JSQR_JEXTEN_Pos |
                     27 << ADC_JSQR_JEXTSEL_Pos | 
                     13 << ADC_JSQR_JSQ1_Pos;  // trig 27 hrtim adc_trig1 (injected)
        ADC5->SMPR1 = 2 << ADC_SMPR1_SMP5_Pos; // 12.5 cycles current_sense, 294 ns, 200 ns min for opamp4


        NVIC_SetPriority(ADC5_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 0, 0));
        NVIC_EnableIRQ(ADC5_IRQn);

        /***************************************************
          OPAMPs
        ***************************************************/
        // OPAMP
        // GPIO_SETL(A, 3, GPIO_MODE::ANALOG, GPIO_SPEED::LOW, 0);
        // OPAMP1->CSR = 1 << OPAMP_CSR_VPSEL_Pos | 3 << OPAMP_CSR_VMSEL_Pos | OPAMP_CSR_HIGHSPEEDEN | OPAMP_CSR_OPAMPINTEN | OPAMP_CSR_OPAMPxEN; // internal follower on pa3/vinp1
        GPIO_SETH(B, 11, GPIO_MODE::ANALOG, GPIO_SPEED::LOW, 0);
        GPIO_SETH(B, 12, GPIO_MODE::ANALOG, GPIO_SPEED::LOW, 0);
        GPIO_SETH(B, 13, GPIO_MODE::ANALOG, GPIO_SPEED::LOW, 0);
        OPAMP4->CSR = 2 << OPAMP_CSR_VPSEL_Pos | 3 << OPAMP_CSR_VMSEL_Pos | OPAMP_CSR_HIGHSPEEDEN | OPAMP_CSR_OPAMPINTEN | OPAMP_CSR_OPAMPxEN; // internal follower on pb11/vinp2
        OPAMP6->CSR = 0 << OPAMP_CSR_VPSEL_Pos | 3 << OPAMP_CSR_VMSEL_Pos | OPAMP_CSR_HIGHSPEEDEN | OPAMP_CSR_OPAMPINTEN | OPAMP_CSR_OPAMPxEN; // internal follower on pb12/vinp0
        OPAMP3->CSR = 1 << OPAMP_CSR_VPSEL_Pos | 3 << OPAMP_CSR_VMSEL_Pos | OPAMP_CSR_HIGHSPEEDEN | OPAMP_CSR_OPAMPINTEN | OPAMP_CSR_OPAMPxEN; // internal follower on pb13/vinp1

        // VREF/2 on opamp1, input PA1, output PA2
        //GPIO_SETL(A, 1, GPIO_MODE::ANALOG, GPIO_SPEED::LOW, 0);
        //GPIO_SETL(A, 2, GPIO_MODE::ANALOG, GPIO_SPEED::LOW, 0);
        OPAMP1->CSR = 0 << OPAMP_CSR_VPSEL_Pos | 3 << OPAMP_CSR_VMSEL_Pos | OPAMP_CSR_HIGHSPEEDEN | OPAMP_CSR_OPAMPxEN; // external follower on pa1/vinp0 pa2/vout

        /***************************************************
          USB Configuration

          DP - PA12
          DM - PA11
          USB_EXTI - PA10 (Used to clear the buffer if there is no traffic)
        ***************************************************/

        /* // Enable Peripheral Clocks */
        /* RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN; */
        /* RCC->APB1ENR1 |= RCC_APB1ENR1_USBEN; */

        // USB
        GPIO_SETH(A, 11, GPIO_MODE::ANALOG, GPIO_SPEED::LOW, 0);         // usb dm
        GPIO_SETH(A, 12, GPIO_MODE::ANALOG, GPIO_SPEED::LOW, 0);         // usb dp
        GPIO_SETH(A, 10, GPIO_MODE::INPUT, GPIO_SPEED::LOW, 0);          // EXTI trigger for usb 
        MASK_SET(SYSCFG->EXTICR[2], SYSCFG_EXTICR3_EXTI10, 0);           // EXTI PA10
        EXTI->RTSR1 = EXTI_RTSR1_RT10;                                   // rising and falling triggers
        EXTI->FTSR1 = EXTI_FTSR1_FT10;
        EXTI->IMR1 = EXTI_IMR1_IM10;                                     // interrupt unmasked, but not enabled in NVIQ

        // USB Interrupts
        NVIC_SetPriority(USB_LP_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 2, 0));
        NVIC_EnableIRQ(USB_LP_IRQn);


}

extern "C" void RTC_WKUP_IRQHandler() {
    IWDG->KR = 0xAAAA;
    static int count = 0;
    count++;
    EXTI->PR1 = EXTI_PR1_PIF20;
    RTC->SCR = RTC_SCR_CWUTF;
}
