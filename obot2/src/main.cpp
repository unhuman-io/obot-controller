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

__attribute__((used, section(".debug_fun"))) float square(float x) {
    return x * x;
}

__attribute__((used, section(".debug_fun"))) int squarei(int x) {
    return x * x;
}

class blah {
     __attribute__((used, section(".debug_fun.funny"))) int set_red() {
        GPIOB->ODR_b.ODR2 ^= 1;
        GPIOB->ODR_b.ODR6 ^= 1;
        i++;
        return i;
    }
    int i = 0;
};

__attribute__((used, section(".debug_bss"))) blah blah_instance;

int main() {
    RCC->RCC_APB2ENR_b.TIM1EN = 1;
    RCC->RCC_APB1ENR1_b.TIM2EN = 1;
    RCC->RCC_APB1ENR1_b.TIM3EN = 1;

    TIM1->TIM1_DIER_b.UIE = 1;
    TIM1->TIM1_PSC = 4000;//25939/2;
    
    volatile float f = 2;
    f = square(f);

    TIM2->TIM2_DIER_b.UIE = 1;
    TIM1->TIM1_SMCR_b.SMS = 0b110; // trigger mode
    TIM1->TIM1_SMCR_b.TS = 0b1; // trigger on tim2

    TIM3->TIM3_DIER_b.UIE = 1;
    TIM3->TIM3_PSC = 1000;
    //TIM1->TIM1_CR1_b.CEN = 1;
    NVIC_EnableIRQ(TIM1_UP_TIM16_IRQn);
    set_nvic_priority(TIM1_UP_TIM16_IRQn, 2);
    NVIC_EnableIRQ(TIM2_IRQn);
    set_nvic_priority(TIM2_IRQn, 3);
    NVIC_EnableIRQ(TIM3_IRQn);
    set_nvic_priority(TIM3_IRQn, 0);
    NVIC_EnableIRQ(USB_LP_IRQn);
    CoreDebug->DEMCR |= 1 << 16;
    set_nvic_priority(DebugMonitor_IRQn, 1);
    NVIC_EnableIRQ(DebugMonitor_IRQn);
    trace_blinker.run();
    cpu::wait_ms(2000);
    TIM2->TIM2_CR2_b.MMS = 1; // enable is a trigger out
    TIM2->TIM2_CR1_b.CEN = 1;
    TIM3->TIM3_CR1_b.CEN = 1;
    uint8_t i = 0;
    asm("vldr.f32 s16, =0x12345678");
    asm("bkpt #0");
    while(1) {
        static uint32_t counter = 0;
        counter++;
        cpu::wait_ms(1000);
        asm("vmov s0, %[val] \n" :: [val] "r" (counter++) : "s0");
        //std::string s;
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
    //trace_blinker.toggle_red();
    TIM1->TIM1_SR_b.UIF = 0;
     asm("dsb":::"memory");

}

uint32_t stuff[4];

extern "C" void TIM2_IRQHandler() {
    //trace_blinker.blink();
    asm("":::"memory");
    asm("str sp, %[addr] \n" :: [addr] "m" (stuff[0]));
    //asm("bkpt #0");
    TIM2->TIM2_SR_b.UIF = 0;
    asm("dsb":::"memory"); // dsb for interrupt near end to not retrigger
}

extern "C" void TIM3_IRQHandler() {
    trace_blinker.toggle_blue();
    TIM3->TIM3_SR_b.UIF = 0;
     asm("dsb":::"memory");
}

std::string_view parse(const std::string_view data);
volatile bool mon_continue = false;
volatile bool continued = false;
void set_monitor_continue() {
    mon_continue = true;
    continued = true;
}
bool monitor_continue() {
    if (mon_continue) {
        mon_continue = false;
        return true;
    }
    return false;
}

volatile bool mon_step = false;
void set_monitor_step() {
    mon_step = true;
}
bool monitor_step() {
    if (mon_step) {
        mon_step = false;
        return true;
    }
    return false;
}


__attribute__((used)) std::string_view last;

int to_hex(int length_buffer, void *address, int length_bytes, uint8_t *buffer) {
    if (length_bytes > length_buffer/2) {
        length_bytes = length_buffer/2;
    }
    auto *src = static_cast<const uint8_t*>(address);
    static constexpr char hex[] = "0123456789abcdef";
    for (int i = 0; i < length_bytes; ++i) {
        uint8_t v = src[i];
        buffer[2*i]     = hex[v >> 4];
        buffer[2*i + 1] = hex[v & 0x0F];
    }
    int out_len = length_bytes * 2;
    return out_len;
}

struct GDBRegs {
    uint32_t r0, r1, r2, r3, r4, r5, r6, r7,
             r8, r9, r10, r11, r12, sp, lr, pc;
    uint32_t psr, msp, psp, primask, basepri, faultmask, control;
    float s[32];
    uint32_t fpscr;
} gregs;

bool set_breakpoint(uint32_t address, int num) {
    FPB->CTRL |= 0x3;
    if ((FPB->COMP[num] & 1) == 0) {
        // not enabled
        uint32_t replace = address & 2 ? 2 : 1;
        FPB->COMP[num] = replace << 30 | (address & ~3) | 1;
        return true;
    }
    return false;
}

void set_breakpoint(uint32_t address) {
    constexpr size_t max_breakpoints = 6;
    for (size_t i = 1; i < max_breakpoints; i++) {
        if (set_breakpoint(address, i)) {
            break;
        }
    }
}
volatile bool trap_continue = false;
volatile bool trap_breakpoint = true;
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
                int length_bytes = 0;
                {
                    auto length_str = str.substr(comma_pos + 1);
                    auto [_, ec] = std::from_chars(length_str.data(), length_str.data() + length_str.size(), length_bytes, 16);
                    if (ec != std::errc()) {
                        return std::string_view("bad len");
                        break;
                    }
                }
                length = to_hex(sizeof(buffer), reinterpret_cast<void *>(addr), length_bytes, buffer);

                break;
            }
            case 'M':
            {
                auto comma_pos = str.find(',');
                if (comma_pos == std::string_view::npos) {
                    return std::string_view("no comma");
                    break;
                }
                auto colon_pos = str.find(':', comma_pos + 1);
                if (colon_pos == std::string_view::npos) {
                    return std::string_view("no colon pos");
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
                int length_bytes = 0;
                {
                    auto length_str = str.substr(comma_pos + 1, colon_pos - comma_pos - 1);
                    auto [_, ec] = std::from_chars(length_str.data(), length_str.data() + length_str.size(), length_bytes, 16);
                    if (ec != std::errc()) {
                        return std::string_view("bad len");
                        break;
                    }
                }
                auto data_str = str.substr(colon_pos + 1);
                if (data_str.size() < static_cast<size_t>(length_bytes * 2)) {
                    return std::string_view("data too short");
                    break;
                }
                for (int i = 0; i < length_bytes; ++i) {
                    char c1 = data_str[2*i];
                    char c2 = data_str[2*i + 1];
                    uint8_t v1 = (c1 >= 'a') ? (c1 - 'a' + 10) : (c1 >= 'A' ? (c1 - 'A' + 10) : (c1 - '0'));
                    uint8_t v2 = (c2 >= 'a') ? (c2 - 'a' + 10) : (c2 >= 'A' ? (c2 - 'A' + 10) : (c2 - '0'));
                    uint8_t v = (v1 << 4) | v2;
                    reinterpret_cast<uint8_t *>(addr)[i] = v;
                }
                return std::string_view("OK");
                break;
            }
            case 'b':
                return std::string_view("break");
            case 'P':
            {
                int reg_num = 0;
                {
                    auto reg_num_str = str.substr(2);
                    auto [_, ec] = std::from_chars(reg_num_str.data(), reg_num_str.data() + reg_num_str.size(), reg_num, 16);
                    if (ec != std::errc()) {
                        return std::string_view("bad reg num");
                        break;
                    }
                }
                unsigned int value = 0;
                {
                    auto equal_pos = str.find('=');
                    if (equal_pos == std::string_view::npos) {
                        return std::string_view("no equal");
                        break;
                    }
                    auto value_str = str.substr(equal_pos + 1);
                    auto [_, ec] = std::from_chars(value_str.data(), value_str.data() + value_str.size(), value, 16);
                    if (ec != std::errc()) {
                        return std::string_view("bad value");
                        break;
                    }
                }
                (&gregs.r0)[reg_num] = __builtin_bswap32(value);
                return std::string_view("OK");
            } 
            case 'c':
                set_monitor_continue();
                return std::string_view("continue");
            case 's':
                monitor_step();
                return std::string_view("step");
            case 'g':
                length = to_hex(sizeof(buffer), &gregs, 6*4, buffer);
                break;
            case 'p':
            {
                int reg_num = 0;
                {
                    auto reg_num_str = str.substr(2);
                    auto [_, ec] = std::from_chars(reg_num_str.data(), reg_num_str.data() + reg_num_str.size(), reg_num, 16);
                    if (ec != std::errc()) {
                        return std::string_view("bad reg num");
                        break;
                    }
                }
                length = to_hex(sizeof(buffer), (&gregs.r0) + reg_num, 4, buffer);
                break;
            }
            case 'z':
                return std::string_view("remove breakpoint not implemented");
            case 'Z':
            {
                auto comma_pos = str.find(',');
                if (comma_pos == std::string_view::npos) {
                    return std::string_view("no comma");
                    break;
                }
                auto addr_str = str.substr(comma_pos + 1);
                uintptr_t addr = 0;
                auto [_, ec] = std::from_chars(addr_str.data(), addr_str.data() + addr_str.size(), addr, 16);
                if (ec != std::errc()) {
                    return std::string_view("bad addr");
                    break;
                }
                
                set_breakpoint(addr);
                asm("mov r4, r3" :::"r4");
                volatile uint32_t reg = FPB->COMP[1];
                //while(trap_breakpoint);
                trap_continue = true;
                return std::string_view("OK");
            }

            case 'y':
                if (continued) {
                    return std::string_view("continued");
                } else {
                    return std::string_view("not continued");
                }
                break;

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
    uint32_t msp; // 0x11
    uint32_t psp; // 0x12
    uint32_t primask; // 0x13
    uint32_t basepri; // 0x14
    uint32_t faultmask; // 0x15
    uint32_t control;   // 0x16
    float s2[16];   // 0x109 - 0x10f as doubles
};

struct alignas(8) FPUContext {
    float s[16];
    uint32_t fpscr;
};

extern "C" __attribute__((used)) void debug_monitor(ContextState* state,
        ContextStateExt* ext, FPUContext* fpu) {
    trace_blinker.set_green();
    //SCB->DFSR = 2; // clear flags
    //state->return_address += 2; // skip the faulting instruction
    [[maybe_unused]] struct {
        std::uintptr_t addr[3];
        uint32_t sp;
    } args = {reinterpret_cast<std::uintptr_t>(state), reinterpret_cast<std::uintptr_t>(ext),
            reinterpret_cast<std::uintptr_t>(fpu), ext->r13};
    gregs = {.r0 = state->r0,
             .r1 = state->r1,
             .r2 = state->r2,
             .r3 = state->r3,
             .r4 = ext->r4,
             .r5 = ext->r5,
             .r6 = ext->r6,
             .r7 = ext->r7,
             .r8 = ext->r8,
             .r9 = ext->r9,
             .r10 = ext->r10,
             .r11 = ext->r11,
             .r12 = state->r12,
             .sp = ext->r13,
             .lr = state->lr,
             .pc = state->return_address,
             .psr = state->psr,
             .msp = ext->msp,
             .psp = ext->psp,
             .primask = ext->primask,
             .basepri = ext->basepri,
             .faultmask = ext->faultmask,
             .control = ext->control,
             .s = {fpu->s[0], fpu->s[1], fpu->s[2], fpu->s[3],
                    fpu->s[4], fpu->s[5], fpu->s[6], fpu->s[7],
                    fpu->s[8], fpu->s[9], fpu->s[10], fpu->s[11],
                    fpu->s[12], fpu->s[13], fpu->s[14], fpu->s[15],
                    ext->s2[0], ext->s2[1], ext->s2[2], ext->s2[3],
                    ext->s2[4], ext->s2[5], ext->s2[6], ext->s2[7],
                    ext->s2[8], ext->s2[9], ext->s2[10], ext->s2[11],
                    ext->s2[12], ext->s2[13], ext->s2[14], ext->s2[15]},
             .fpscr = fpu->fpscr
            };
    asm("":::"memory");
    //trace_blinker.usb.send_data(2, reinterpret_cast<const uint8_t*>(&args), sizeof(args), false);
    //trace_blinker.usb.send_data(2, reinterpret_cast<const uint8_t*>(fpu), 64, false);
    continued = false;
    while (1) {
        
        if (monitor_continue()) {
            //asm("bkpt #3");
            volatile uint32_t reg = FPB->COMP[1];
            //while(trap_continue);
            CoreDebug->DEMCR &= ~(1 << 18);
            //FPB->CTRL = 2; // disable all breakpoints
            //FPB->COMP[0] = 0; // clear first breakpoint
            if (reinterpret_cast<uint8_t *>(gregs.pc)[1] == 0xbe) {
                // breakpoint instruction
                gregs.pc += 2;
            }
            //asm("":::"memory");
            
            break;
        } else if ( monitor_step() ) {
            //FPB->CTRL = 2; // disable all breakpoints
            //FPB->COMP[0] = 0; // clear first breakpoint

            CoreDebug->DEMCR |= 1 << 18;
            // // set a temporary breakpoint at the next instruction
            // uint8_t instruction_byte = reinterpret_cast<uint8_t *>(state->return_address)[1];
            // if (instruction_byte == 0xbe) {
            //     // breakpoint instruction
            //     state->return_address += 2;
            //     instruction_byte = reinterpret_cast<uint8_t *>(state->return_address)[1];
            // }
            // uint8_t high_5bit = instruction_byte >> 3;
            // if ( high_5bit == 0b11110 || high_5bit == 0b11111 || high_5bit == 0b11101 ) {
            //     // 32 bit instruction
            //     set_breakpoint(state->return_address + 4);
            // } else {
            //     // 16 bit instruction
            //     set_breakpoint(state->return_address + 2);
            // }
            break;
        }
    }
    FPB->COMP[0] = 0; // clear first breakpoint
    state->r0 = gregs.r0;
    state->r1 = gregs.r1;
    state->r2 = gregs.r2;
    state->r3 = gregs.r3;
    state->r12 = gregs.r12;
    state->lr = gregs.lr;
    state->return_address = gregs.pc;
    // todo use all these regs and add the rest
    ext->r4 = gregs.r4;
    ext->r5 = gregs.r5;
    ext->r6 = gregs.r6;
    ext->r7 = gregs.r7;
    ext->r8 = gregs.r8;
    ext->r9 = gregs.r9;
    ext->r10 = gregs.r10;
    ext->r11 = gregs.r11;
    ext->r13 = gregs.sp;
    for (int i = 0; i < 16; ++i) {
        fpu->s[i] = gregs.s[i];
    }
    for (int i = 0; i < 16; ++i) {
        ext->s2[i] = gregs.s[16 + i];
    }
    fpu->fpscr = gregs.fpscr;
    asm("":::"memory");
    SCB->DFSR = 3; // clear flags
    trace_blinker.clear_green();
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
static_assert(sizeof(ContextStateExt) == 128);
//static_assert(&FPU->FPCAR == (std::uintptr_t) 0xE000EF38);

extern "C" __attribute__((naked)) void DebugMon_Handler() {
    asm("mov r0, sp \n" // ContextState pointer in r0
        "sub sp, sp, #128 \n" // make space for ContextStateExt
        "mov r1, sp \n" // ContextStateExt pointer in r1
        "vmov s0, s0 \n" // ensure floating point state is saved
        "mov r2, sp \n"
        "stm r2!, {r4-r11} \n" // store r4-r11
        //"str lr, [r2] \n"  // store lr
        // "mrs r4, msp \n"
        // "mrs r5, psp \n"
        "mov r4, r0 \n" // overwritten
        "mov r5, lr \n"
        "mov r6, r0 \n" // overwritten
        "mov r7, #0 \n"
        "mrs r8, primask \n"
        "mrs r9, basepri \n"
        "mrs r10, faultmask \n"
        "mrs r11, control \n"
        "stm r2!, {r4-r11} \n"
        "vstm r2!, {s16-s31} \n" // store s16-s31
        "mov r2, r0 \n"
        "tst lr, #0x10 \n"     // check EXC_RETURN bit 4 for extended frame
        "ite eq \n"
        "addeq r2, #0x68 \n"   // extended frame
        "addne r2, #0x20 \n"   // basic frame
        "str r2, [sp, #32] \n" // store sp at offset sp and msp
        "str r2, [sp, #40] \n"
        "ldr r2, =0xE000EF38 \n" 
        "ldr r2, [r2] \n"      // FPCAR address in r2
        "push {lr} \n"
        "bl debug_monitor \n"
       // "bkpt #2 \n"
        "pop {lr} \n"
        "mov r0, sp \n" // ContextStateExt pointer in r0
        "ldm r0!, {r4-r11} \n" // load r4-r11
        "add r0, r0, #8 \n" // skip sp
        ////"ldm r0, {lr} \n" // load lr
        "add r0, r0, #24 \n" // skip to fp regs
        "vldm r0!, {s16-s31} \n" // load s16-s31
        "add sp, sp, #128 \n"
        "bx lr \n");
}



// void monitor_step() {
//     // mask ints with priority lower than whatever called the breakpoint
//     // the interrupt that called the breakpoint will have priority in bits 7-4 of the IPSR
//     CoreDebug->DEMCR |= 1 << 18;
// }

extern "C" __attribute__((used)) void usb_interrupt(ContextState *state) {
    asm("":::"memory");
    trace_blinker.usb.interrupt();
    if (trace_blinker.usb.new_rx_data(1)) {
        uint8_t buffer[64];
        int len = trace_blinker.usb.receive_data(1, buffer, sizeof(buffer));
        std::string_view s_in = std::string_view(reinterpret_cast<const char *>(buffer), len);
        // echo back

        std::string_view s_out = parse(s_in);
        if (s_out == "break") {
            s_out = "break at ";// + std::to_string(state->return_address);
            //std::memcpy(buffer, s_out.data(), std::min(s_out.size(), sizeof(buffer)));
            //s_out = std::string_view(reinterpret_cast<const char *>(buffer), std::min(s_out.size(), sizeof(buffer)));
            set_breakpoint(state->return_address, 0);
        } else if (s_out == "step") {
            s_out = "step at ";// + std::to_string(gregs.pc);
            //std::memcpy(buffer, s_out.data(), std::min(s_out.size(), sizeof(buffer)));
            //s_out = std::string_view(reinterpret_cast<const char *>(buffer), std::min(s_out.size(), sizeof(buffer)));
            set_monitor_step();
        }
        last = s_out;
        trace_blinker.usb.send_data(1, reinterpret_cast<const uint8_t *>(s_out.data()), s_out.size(), false);
    }
    asm("":::"memory");
}

extern "C" __attribute__((naked)) void USB_LP_IRQHandler() {
    asm("mov r0, sp \n" // ContextState pointer in r0
        "push {lr} \n"
        "bl usb_interrupt \n"
        "pop {pc} \n");
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