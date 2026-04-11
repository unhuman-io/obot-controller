#include <stdint.h>
#include <stddef.h>

// Prevent LTO from deleting these backend-required functions
__attribute__((used)) 
void __aeabi_memcpy(void *dest, const void *src, size_t n) {
    uint8_t *d8 = (uint8_t *)dest;
    const uint8_t *s8 = (const uint8_t *)src;

    if ((((uintptr_t)d8 | (uintptr_t)s8) & 3) == 0) {
        uint32_t *d32 = (uint32_t *)d8;
        const uint32_t *s32 = (const uint32_t *)s8;
        while (n >= 4) {
            *d32++ = *s32++;
            n -= 4;
        }
        d8 = (uint8_t *)d32;
        s8 = (const uint8_t *)s32;
    }

    while (n--) {
        *d8++ = *s8++;
    }
}

__attribute__((used)) 
void __aeabi_memcpy4(void *dest, const void *src, size_t n) {
    uint32_t *d32 = (uint32_t *)dest;
    const uint32_t *s32 = (const uint32_t *)src;

    while (n >= 4) {
        *d32++ = *s32++;
        n -= 4;
    }

    uint8_t *d8 = (uint8_t *)d32;
    const uint8_t *s8 = (const uint8_t *)s32;
    while (n--) {
        *d8++ = *s8++;
    }
}

__attribute__((used)) 
void __aeabi_memcpy8(void *dest, const void *src, size_t n) {
    __aeabi_memcpy4(dest, src, n);
}
