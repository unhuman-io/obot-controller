#include <cstddef>

// memcpy and memset for the clang builds. The C library that toolchain links
// has both as a loop over bytes, and clang calls them for every struct it
// copies past a few words: the main loop's copies of an 80 byte status were
// 2300 of its 3500 cycles. These move words while there are some, 16 bytes a
// pass, which the Cortex-M4 does at any alignment. gcc links a memcpy of this
// kind and keeps it.
#if defined(__clang__) && defined(__thumb2__)
extern "C" {
[[gnu::naked, gnu::used]] void* memcpy(void*, const void*, std::size_t) {
    __asm__(R"(
        push    {r0, r4}
    1:  cmp     r2, #16
        blo     2f
        ldr     r3, [r1], #4
        ldr     r4, [r1], #4
        str     r3, [r0], #4
        str     r4, [r0], #4
        ldr     r3, [r1], #4
        ldr     r4, [r1], #4
        str     r3, [r0], #4
        str     r4, [r0], #4
        subs    r2, #16
        b       1b
    2:  cmp     r2, #4
        blo     3f
        ldr     r3, [r1], #4
        str     r3, [r0], #4
        subs    r2, #4
        b       2b
    3:  cbz     r2, 4f
        ldrb    r3, [r1], #1
        strb    r3, [r0], #1
        subs    r2, #1
        b       3b
    4:  pop     {r0, r4}
        bx      lr
    )");
}
[[gnu::naked, gnu::used]] void* __aeabi_memcpy(void*, const void*, std::size_t) { __asm__("b memcpy"); }
[[gnu::naked, gnu::used]] void* __aeabi_memcpy4(void*, const void*, std::size_t) { __asm__("b memcpy"); }
[[gnu::naked, gnu::used]] void* __aeabi_memcpy8(void*, const void*, std::size_t) { __asm__("b memcpy"); }

[[gnu::naked, gnu::used]] void* memset(void*, int, std::size_t) {
    __asm__(R"(
        mov     r12, r0
        uxtb    r1, r1
        orr     r1, r1, r1, lsl #8
        orr     r1, r1, r1, lsl #16
    1:  cmp     r2, #4
        blo     2f
        str     r1, [r0], #4
        subs    r2, #4
        b       1b
    2:  cbz     r2, 3f
        strb    r1, [r0], #1
        subs    r2, #1
        b       2b
    3:  mov     r0, r12
        bx      lr
    )");
}
}
#endif
