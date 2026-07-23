
// unsigned int go_to_bootloader;
// unsigned int rcc_csr_copy;
#include "stm32g474xx.h"
void clock_config();
void pin_config();
extern unsigned int _estack;
extern unsigned int _sidata; // Flash load address
extern unsigned int _sdata;  // RAM start address
extern unsigned int _edata;  // RAM end address
extern unsigned int __bss_start__, __bss_end__;    // .bss symbols

extern "C" void (*__init_array_start[])(void);
extern "C" void (*__init_array_end[])(void);
int main();
extern "C" void Reset_Handler(void) {
    clock_config();
    // Copy .data from Flash to RAM
    unsigned int *src = &_sidata;
    unsigned int *dst = &_sdata;
    
    while (dst < &_edata) {
        *dst++ = *src++;
    }
    dst = &__bss_start__;
    while (dst < &__bss_end__) {
        *dst++ = 0;
    }

    // 3. Run C++ constructors
    void (**p)() = __init_array_start;
        while (p < __init_array_end) {
            (*p)();
            p++;
        }

    // 4. Jump straight to main
    main();
    
    while(1);
}


__attribute__((section(".isr_vector"), used))
void (*const vector_table[])(void) = {
    (void (*)(void))(&_estack), // 0x0: Initial Stack Pointer
    Reset_Handler                        // 0x4: Jump to standard crt0 start
};

void tim2_isr();
void tim1_isr();
void os_loop();

int main() {
    pin_config();
    while(1) {
        tim2_isr();
        tim1_isr();
        os_loop();
        //MySystem::MainLoop::update();
    }
    return 0;
}

extern "C" {
void _close_r() {}
void _getpid_r() {}
void _kill_r() {}
void _lseek_r() {}
void _read_r() {}
void _write_r() {}
}
