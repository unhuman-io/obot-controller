#pragma once

#include "stm32g474xx.h"


void pin_config_freebot_g474_motor_r0() {
     // Peripheral clock enable
        RCC->APB1ENR1 = RCC_APB1ENR1_SPI3EN | RCC_APB1ENR1_TIM2EN |  RCC_APB1ENR1_TIM4EN | RCC_APB1ENR1_TIM5EN;
        RCC->APB2ENR |= RCC_APB2ENR_SPI1EN | RCC_APB2ENR_TIM1EN | RCC_APB2ENR_HRTIM1EN;
        RCC->AHB1ENR |= RCC_AHB1ENR_DMA1EN | RCC_AHB1ENR_DMAMUX1EN;
        RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN | RCC_AHB2ENR_GPIOBEN | RCC_AHB2ENR_GPIOCEN | RCC_AHB2ENR_GPIODEN;

        CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
        DWT->CYCCNT = 0;
        DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;

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
        static_assert(CPU_FREQUENCY_HZ / main_loop_frequency < 65536, "Main loop frequency too low");
        TIM1->ARR = CPU_FREQUENCY_HZ / main_loop_frequency - 1;
        TIM1->DIER = TIM_DIER_UIE;
        NVIC_SetPriority(TIM1_UP_TIM16_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 1, 0));
        NVIC_EnableIRQ(TIM1_UP_TIM16_IRQn);
}