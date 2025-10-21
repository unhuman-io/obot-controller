import trace_blinker;
//#include <cstdint>
//import <cstdint>;
import stm32g474;
import trace_board;

TraceBlinker trace_blinker;

namespace cpu = stm32g474;

int main() {
    RCC->RCC_APB2ENR_b.TIM1EN = 1;
    RCC->RCC_APB1ENR1_b.TIM2EN = 1;
    TIM1->DIER_b.UIE = 1;
    TIM1->PSC = 25939/2;
    

    TIM2->DIER_b.UIE = 1;
    TIM1->SMCR_b.SMS = 0b110;
    TIM1->SMCR_b.TS = 0b110;
    TIM1->CR1_b.CEN = 1;
    cpu::wait_ms(2000);
    TIM2->CR1_b.CEN = 1;
    NVIC_EnableIRQ(TIM1_UP_TIM16_IRQn);
    NVIC_EnableIRQ(TIM2_IRQn);
    trace_blinker.run();
}

extern "C" void TIM1_UP_TIM16_IRQHandler() {
    trace_blinker.blink();
    TIM1->SR_b.UIF = 0;
}

extern "C" void TIM2_IRQHandler() {
    trace_blinker.blink();
    TIM2->SR_b.UIF = 0;
}

extern "C" void TIM3_IRQHandler() {
    trace_blinker.blink();
    TIM3->SR_b.UIF = 0;
}

extern "C" {
void _close() {}
void _lseek() {}
void _read() {}
void _write() {}
void _kill() {}
void _getpid() {}
void _exit() {}
}