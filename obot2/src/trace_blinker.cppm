module;

import trace_board;
import stm32g474;

export module trace_blinker;

export class TraceBlinker : public TraceBoard {
  public:
    TraceBlinker() : TraceBoard() {
        RCC->RCC_AHB2ENR_b.GPIOBEN = 1;
        GPIOB->MODER_b.MODER6 = 1;
        GPIOB->MODER_b.MODER7 = 1;
        GPIOB->MODER_b.MODER8 = 1;
    }

    void blink() {
        GPIOB->BSRR_b.BS6 = 1;
        cpu::wait_ms(500);
        GPIOB->BSRR_b.BR6 = 1;
        cpu::wait_ms(500);
        GPIOB->BSRR_b.BS7 = 1;
        cpu::wait_ms(500);
        GPIOB->BSRR_b.BR7 = 1;
        cpu::wait_ms(500);
        GPIOB->BSRR_b.BS8 = 1;
        cpu::wait_ms(500);
        GPIOB->BSRR_b.BR8 = 1;
        cpu::wait_ms(500);
    }
};