import stm32g474;
import trace_board;
#include <cstdint>

TraceBoard trace_board;

int main() {
    RCC->RCC_AHB2ENR_b.GPIOBEN = 1;
    GPIOB->MODER_b.MODER6 = 1;
    GPIOB->MODER_b.MODER7 = 1;
    GPIOB->MODER_b.MODER8 = 1;

    while(1) {
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