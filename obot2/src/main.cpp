import trace_blinker;
//#include <cstdint>
//import <cstdint>;
import stm32g474;
import trace_board;
//import <cstddef>;
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
    set_nvic_priority(TIM1_UP_TIM16_IRQn, 2);
    NVIC_EnableIRQ(TIM2_IRQn);
    set_nvic_priority(TIM2_IRQn, 3);
    NVIC_EnableIRQ(USB_LP_IRQn);
    CoreDebug->DEMCR |= 1 << 16;
    set_nvic_priority(DebugMonitor_IRQn, 1);
    NVIC_EnableIRQ(DebugMonitor_IRQn);
    trace_blinker.run();
    cpu::wait_ms(2000);
    TIM2->TIM2_CR2_b.MMS = 1; // enable is a trigger out
    TIM2->TIM2_CR1_b.CEN = 1;
    uint8_t i = 0;
    while(1) {
        static uint32_t counter = 0;
        counter++;
        cpu::wait_ms(1000);
        asm("vmov s0, %[val] \n" :: [val] "r" (counter++) : "s0");
        std::string s;
        asm("":::"memory");
        //s = "counter: ";// + std::to_string(counter) + "\n";
        //s += std::to_string(counter);
        //trace_blinker.usb.send_data(2, reinterpret_cast<const uint8_t *>(s.c_str()), s.size());
        //volatile char c = s[0];
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

uint32_t stuff[4];

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
    TIM3->TIM3_SR_b.UIF = 0;
}

std::string_view parse(const std::string_view data);

__attribute__((used)) std::string_view last;

extern "C" void USB_LP_IRQHandler() {
    asm("":::"memory");
    trace_blinker.usb.interrupt();
    if (trace_blinker.usb.new_rx_data(1)) {
        uint8_t buffer[64];
        int len = trace_blinker.usb.receive_data(1, buffer, sizeof(buffer));
        std::string_view s_in = std::string_view(reinterpret_cast<const char *>(buffer), len);
        // echo back

        std::string_view s_out = parse(s_in);
        last = s_out;
        trace_blinker.usb.send_data(1, reinterpret_cast<const uint8_t *>(s_out.data()), s_out.size(), false);
    }
    asm("":::"memory");
}
uint8_t buffer[63] = "default";
std::string_view parse(const std::string_view str) {
    int length = 20;
    if (str.size() > 1 && str[0] == '$') {
        switch (str[1]) {
            case 'm':
            {
                auto comma_pos = str.rfind(',');
                if (comma_pos == std::string_view::npos) {
                    return std::string_view("no comma");
                    break;
                }
                auto addr_str = str.substr(2, comma_pos - 2);
                uintptr_t addr = 0;
                {
                    auto [_, ec] = std::from_chars(addr_str.data(), addr_str.data() + addr_str.size(), addr, 16);
                    if (ec != std::errc()) {
                        return std::string_view("bad addr");
                        break;
                    }
                }
                int length_hex = 0;
                {
                    auto length_str = str.substr(comma_pos + 1);
                    auto [_, ec] = std::from_chars(length_str.data(), length_str.data() + length_str.size(), length_hex, 16);
                    if (ec != std::errc()) {
                        return std::string_view("bad len");
                        break;
                    }
                }
                if (length_hex > sizeof(buffer)/2) {
                    length_hex = sizeof(buffer)/2;
                }
                length = length_hex*2;
                for (size_t i = 0; i < length_hex; i++) {
                    uint8_t *ptr = reinterpret_cast<uint8_t *>(addr + i);
                    if (*ptr == 0) {
                        buffer[i*2] = '0';
                        buffer[i*2 + 1] = '0';
                    } else if (*ptr < 16) {
                        buffer[i*2] = '0';
                        std::to_chars(reinterpret_cast<char *>(buffer + i*2 + 1), reinterpret_cast<char *>(buffer + i*2 + 2), *ptr, 16);
                    } else {
                        std::to_chars(reinterpret_cast<char *>(buffer + i*2), reinterpret_cast<char *>(buffer + i*2) + 2, *ptr, 16);
                    }
                }
                break;
            }
            default:
                return std::string_view();
        }
    }

    return std::string_view(reinterpret_cast<const char *>(buffer), length);
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
    [[maybe_unused]] struct {
        std::uintptr_t addr[3];
        uint32_t sp;
    } args = {reinterpret_cast<std::uintptr_t>(state), reinterpret_cast<std::uintptr_t>(ext),
            reinterpret_cast<std::uintptr_t>(fpu), ext->r13};
    //trace_blinker.usb.send_data(2, reinterpret_cast<const uint8_t*>(&args), sizeof(args), false);
    trace_blinker.usb.send_data(2, reinterpret_cast<const uint8_t*>(fpu), 64, false);
   
}


// below assembly uses hardcoded offsets - static asserts to ensure they are correct
static_assert(alignof(ContextStateExt) == 8);
// static_assert(offsetof(ContextStateExt, r4) == 0);
// static_assert(offsetof(ContextStateExt, r5) == 4);
// static_assert(offsetof(ContextStateExt, r6) == 8);
// static_assert(offsetof(ContextStateExt, r7) == 12);
// static_assert(offsetof(ContextStateExt, r8) == 16);
// static_assert(offsetof(ContextStateExt, r9) == 20);
// static_assert(offsetof(ContextStateExt, r10) == 24);
// static_assert(offsetof(ContextStateExt, r11) == 28);
// static_assert(offsetof(ContextStateExt, r13) == 32);
// static_assert(offsetof(ContextStateExt, lr) == 36);
// static_assert(sizeof(ContextStateExt) == 40);
//static_assert(&FPU->FPCAR == (std::uintptr_t) 0xE000EF38);

extern "C" __attribute__((naked)) void DebugMon_Handler() {
    asm("mov r0, sp \n" // ContextState pointer in r0
        "sub sp, sp, #40 \n" // make space for ContextStateExt
        "mov r1, sp \n" // ContextStateExt pointer in r1
        "vmov s0, s0 \n" // ensure floating point state is saved
        "mov r2, sp \n"
        "stm r2!, {r4-r11} \n" // store r4-r11
        "str lr, [r2, #4] \n"  // store lr
        "mov r2, r0 \n"
        "tst lr, #0x10 \n"     // check EXC_RETURN bit 4 for extended frame
        "ite eq \n"
        "addeq r2, #0x68 \n"   // extended frame
        "addne r2, #0x20 \n"   // basic frame
        "str r2, [sp, 32] \n"  // store original sp position in r13 position of ContextStateExt
        "ldr r2, =0xE000EF38 \n" 
        "ldr r2, [r2] \n"      // FPCAR address in r2
        "push {lr} \n"
        "bl debug_monitor \n"
        "pop {lr} \n"
        "add sp, sp, #40 \n"
        "bx lr \n");
}

extern "C" {
void _close() {}
void _lseek() {}
void _read() {}
void _write() {}
void _kill() {}
void _getpid() {}
void _isatty() {}
void _fstat() {}
__attribute__((used)) void abort() { while(1);}
__attribute__((used)) void _exit() {}
//FILE *const stderr = 0;
}