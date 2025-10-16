import trace_blinker;
//#include <cstdint>
//import <cstdint>;
import stm32g474;

TraceBlinker trace_blinker;

int main() {
    RCC->RCC_APB2ENR_b.TIM1EN = 1;
    TIM1->DIER_b.UIE = 1;
    TIM1->PSC = 25939/2;
    TIM1->CR1_b.CEN = 1;
    NVIC_EnableIRQ(TIM1_UP_TIM16_IRQn);
    trace_blinker.run();
}

extern "C" void TIM1_UP_TIM16_IRQHandler() {
    trace_blinker.blink();
    TIM1->SR_b.UIF = 0;
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