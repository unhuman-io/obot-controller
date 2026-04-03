

asm(R"(.section .text.init.enter
     .global _start
     .word 0x20001000
     .word _start)");

int main() {
    for (volatile int i = 0; ; i += 1);
    return 0;
}

extern "C" void _exit() {}