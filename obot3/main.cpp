
// unsigned int go_to_bootloader;
// unsigned int rcc_csr_copy;
void pin_config();
extern unsigned int _estack;
extern "C" void _start(void);
__attribute__((section(".isr_vector"), used))
void (*const vector_table[])(void) = {
    (void (*)(void))(&_estack), // 0x0: Initial Stack Pointer
    _start                        // 0x4: Jump to standard crt0 start
};

int main() {
    pin_config();
    while(1) {
        
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
