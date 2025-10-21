module;

import trace_board;
import stm32g474;
import obot_std;

export module trace_blinker;

namespace cpu = stm32g474;

export template<uint32_t rate = 5>
class TraceBlinker : public TraceBoard {
  public:
    TraceBlinker() : TraceBoard() {
        // r0
        RCC->RCC_AHB2ENR_b.GPIOBEN = 1;
        GPIOB->MODER_b.MODER6 = 1;
        GPIOB->MODER_b.MODER7 = 1;
        GPIOB->MODER_b.MODER8 = 1;

        // r1
        RCC->RCC_AHB2ENR_b.GPIOCEN = 1;
        RCC->RCC_AHB2ENR_b.GPIOFEN = 1;
        GPIOB->MODER_b.MODER2 = 1;
        GPIOC->MODER_b.MODER12 = 1;
        GPIOF->MODER_b.MODER9 = 1;
    }

    void blink() {
        GPIOB->BSRR_b.BS6 = 1;
        GPIOB->BSRR_b.BS2 = 1;
        cpu::wait_ms(1'000 / rate);
        GPIOB->BSRR_b.BR6 = 1;
        GPIOB->BSRR_b.BR2 = 1;
        cpu::wait_ms(1'000 / rate);
        GPIOB->BSRR_b.BS7 = 1;
        GPIOC->BSRR_b.BS12 = 1;
        cpu::wait_ms(1'000 / rate);
        GPIOB->BSRR_b.BR7 = 1;
        GPIOC->BSRR_b.BR12 = 1;
        cpu::wait_ms(1'000 / rate);
        GPIOB->BSRR_b.BS8 = 1;
        GPIOF->BSRR_b.BS9 = 1;
        cpu::wait_ms(1'000 / rate);
        GPIOB->BSRR_b.BR8 = 1;
        GPIOF->BSRR_b.BR9 = 1;
        cpu::wait_ms(1'000 / rate);
    }
    void run() {
        while(1) {
           // blink();
        }
    }
};