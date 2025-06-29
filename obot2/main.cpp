import stm32g474;
import trace_board;

TraceBoard trace_board;

int main() {
    RCC->RCC_AHB2ENR_b.GPIOBEN = 1;
    GPIOB->MODER_b.MODER6 = 1;
    while(1) {
        GPIOB->BSRR_b.BS6 = 1;
        for (int i = 0; i < 1000000; i++) {
            asm("nop");
        }
        GPIOB->BSRR_b.BR6 = 1;
        for (int i = 0; i < 1000000; i++) {
            asm("nop");
        }
    }
}

extern "C" {
void _close() {}
void _lseek() {}
void _read() {}
void _write() {}
}
