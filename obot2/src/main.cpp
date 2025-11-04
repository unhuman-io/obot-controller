import trace_blinker;
//#include <cstdint>
//import <cstdint>;
import stm32g474;
import trace_board;
#include <cstddef>
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
        // char c[14] = "Hello, World!";
        // c[13] = i++;
        // trace_blinker.usb.send_data(2, (uint8_t *) c, 13, true, 10'000'000);
        // c[13] = i++;
        // trace_blinker.usb.send_data(2, (uint8_t *) c, 13, true, 10'000'000);
    }
}
    

    

extern "C" void TIM1_UP_TIM16_IRQHandler() {
    trace_blinker.blink();
    TIM1->TIM1_SR_b.UIF = 0;
}

uint32_t stuff[2];

extern "C" void TIM2_IRQHandler() {
    //trace_blinker.blink();
    asm("":::"memory");
    asm("str sp, %[addr] \n" :: [addr] "m" (stuff[0]));
    asm("bkpt #0");
    TIM2->TIM2_SR_b.UIF = 0;
    asm("dsb":::"memory"); // dsb for interrupt near end to not retrigger
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


struct ContextState {
    uint32_t r0;
    uint32_t r1;
    uint32_t r2;
    uint32_t r3;
    uint32_t r12;
    uint32_t lr; // r14
    uint32_t return_address; // pc
    uint32_t psr;

};

struct alignas(8) ContextStateExt {
    uint32_t r4;
    uint32_t r5;
    uint32_t r6;
    uint32_t r7;
    uint32_t r8;
    uint32_t r9;
    uint32_t r10;
    uint32_t r11;
    uint32_t r13; //sp
    uint32_t lr;
};

struct alignas(8) FPUContext {
    float s[16];
    uint32_t fpscr;
};

extern "C" __attribute__((used)) void debug_monitor(ContextState* state,
        const ContextStateExt* ext, const FPUContext* fpu) {
    trace_blinker.set_green();
    SCB->DFSR = 2; // clear flags
    state->return_address += 2; // skip the faulting instruction
    struct {
        std::uintptr_t addr[3];
        uint32_t sp;
    } args = {reinterpret_cast<std::uintptr_t>(state), reinterpret_cast<std::uintptr_t>(ext),
            reinterpret_cast<std::uintptr_t>(fpu), ext->r13};
    //trace_blinker.usb.send_data(2, reinterpret_cast<const uint8_t*>(&args), sizeof(args), false);
    trace_blinker.usb.send_data(2, reinterpret_cast<const uint8_t*>(fpu), 64, false);
    asm("add sp, sp, %[size] \n" :: [size] "i" (sizeof(ContextStateExt)));
}

extern "C" __attribute__((naked)) void DebugMon_Handler() {
    asm("str sp, %[addr] \n" :: [addr] "m" (stuff[1]));
    asm("mov r0, sp \n"
        "sub sp, sp, %[size] \n" // make space for ContextStateExt
        "mov r1, sp \n"
        "vmov s0, s0 \n" // ensure floating point state is saved
        "mov r2, sp \n"
        "stm r2!, {r4-r11} \n"
        "str lr, [r2, #4] \n" // store lr
        "mov r2, r0 \n"
        "tst lr, #0x10 \n"
        "ite eq \n"
        "addeq r2, #0x68 \n" // extended frame
        "addne r2, #0x20 \n" // basic frame
        // todo need to test bit to figure out if fpu context is saved to figure out sp offset
        "str r2, [sp, %[r13_offset]] \n" // store original sp position in r13 position of ContextStateExt
        "mov r2, %[fpcar] \n" // fpcar address
        "b debug_monitor" :: [size] "i" (sizeof(ContextStateExt)),
            [r13_offset] "i" (offsetof(ContextStateExt, r13)),
            [fpcar] "r" (FPU->FPCAR) :
            "r0", "r1", "r2", "r4", "r5", "r6", "r7", "r8", "r9", "r10", "r11", "lr");
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