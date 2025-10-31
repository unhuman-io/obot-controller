import trace_blinker;
//#include <cstdint>
//import <cstdint>;
import stm32g474;
import trace_board;
import obot_std;

TraceBlinker trace_blinker;
// extern uint32_t go_to_bootloader;

namespace cpu = stm32g474;

int main() {
    RCC->RCC_APB2ENR_b.TIM1EN = 1;
    RCC->RCC_APB1ENR1_b.TIM2EN = 1;

    TIM1->TIM1_DIER_b.UIE = 1;
    TIM1->TIM1_PSC = 25939/2;
    

    TIM2->TIM2_DIER_b.UIE = 1;
    TIM1->TIM1_SMCR_b.SMS = 0b110; // trigger mode
    TIM1->TIM1_SMCR_b.TS = 0b1; // trigger on tim2
    //TIM1->TIM1_CR1_b.CEN = 1;
    NVIC_EnableIRQ(TIM1_UP_TIM16_IRQn);
    NVIC_EnableIRQ(TIM2_IRQn);
    set_nvic_priority(TIM2_IRQn, 1);
    NVIC_EnableIRQ(USB_LP_IRQn);
    CoreDebug->DEMCR |= 1 << 16;
    NVIC_EnableIRQ(DebugMonitor_IRQn);
    trace_blinker.run();
    cpu::wait_ms(2000);
    TIM2->TIM2_CR2_b.MMS = 1; // enable is a trigger out
    TIM2->TIM2_CR1_b.CEN = 1;
    uint8_t i = 0;
    while(1) {
        char c[14] = "Hello, World!";
        c[13] = i++;
        trace_blinker.usb.send_data(2, (uint8_t *) c, 13, true, 10'000'000);
        c[13] = i++;
        trace_blinker.usb.send_data(2, (uint8_t *) c, 13, true, 10'000'000);
    }
}
    

    

extern "C" void TIM1_UP_TIM16_IRQHandler() {
    trace_blinker.blink();
    TIM1->TIM1_SR_b.UIF = 0;
}

extern "C" void TIM2_IRQHandler() {
    //trace_blinker.blink();
    asm("bkpt #0");
    TIM2->TIM2_SR_b.UIF = 0;
}

extern "C" void TIM3_IRQHandler() {
    trace_blinker.blink();
    TIM3->TIM2_SR_b.UIF = 0;
}

extern "C" void USB_LP_IRQHandler() {
    asm("":::"memory");
    trace_blinker.usb.interrupt();
    asm("":::"memory");
}


struct ContextFrame {
    uint32_t r0;
    uint32_t r1;
    uint32_t r2;
    uint32_t r3;
    uint32_t r12;
    uint32_t lr;
    uint32_t return_address;
    uint32_t psr;
};

extern "C" __attribute__((used)) void debug_monitor(ContextFrame* frame) {
    trace_blinker.set_green();
    SCB->DFSR = 2; // clear flags
    frame->return_address += 2; // skip the faulting instruction
}

extern "C" __attribute__((naked)) void DebugMon_Handler() {
    asm("mrs r0, msp \n"
        "b debug_monitor");
    asm("":::"memory");
}

extern "C" {
void _close() {}
void _lseek() {}
void _read() {}
void _write() {}
void _kill() {}
void _getpid() {}
__attribute__((used)) void _exit() {}
}