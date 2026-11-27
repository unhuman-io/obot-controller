module;
#include <cstddef>
#include <cstdint>
#include "st_device.h"

export module power.g5_uart;

export {
// A UART of the stm32g5, as much as a link of small frames needs: 8 bits, no
// parity, the 8 byte fifos on, an interrupt for received bytes if wanted.
// (This belongs in motorlib/device with the other init modules once a second
// board uses it.)
struct UARTInit {
    uint8_t instance = 2;               // USART2 or USART3
    uint32_t kernel_clock_hz = 0;       // the APB1 clock, unless the RCC is told otherwise
    uint32_t baud = 115'200;
    bool receive_interrupt = false;     // CR1 RXFNEIE: the line's handler takes the bytes with uart_read()
};

namespace uart_bit {
inline constexpr uint32_t CR1_UE = 1u << 0, CR1_RE = 1u << 2, CR1_TE = 1u << 3, CR1_RXFNEIE = 1u << 5, CR1_FIFOEN = 1u << 29;
inline constexpr uint32_t ISR_ERRORS = 0xF, ISR_RXFNE = 1u << 5, ISR_TXFNF = 1u << 7;      // parity, framing, noise, overrun
static_assert(CR1_UE == USART1_USART_CR1_UE_Msk && CR1_RE == USART1_USART_CR1_RE_Msk && CR1_TE == USART1_USART_CR1_TE_Msk);
static_assert(CR1_RXFNEIE == USART1_USART_CR1_RXFNEIE_Msk && CR1_FIFOEN == USART1_USART_CR1_FIFOEN_Msk);
}

template <const UARTInit u>
inline USART1_Type& uart_regs() {
    static_assert(u.instance == 2 || u.instance == 3, "USART2 or USART3");
    return *(u.instance == 2 ? USART2 : USART3);
}

template <const UARTInit u>
inline void device_uart_config() {
    using namespace uart_bit;
    static_assert(u.kernel_clock_hz / u.baud >= 16 && u.kernel_clock_hz / u.baud <= 0xFFFF, "baud rate out of reach of this clock");
    USART1_Type& regs = uart_regs<u>();
    regs.USART_BRR = (u.kernel_clock_hz + u.baud / 2) / u.baud;
    regs.USART_CR1 = CR1_FIFOEN | (u.receive_interrupt ? CR1_RXFNEIE : 0) | CR1_TE | CR1_RE;
    regs.USART_CR1 = regs.USART_CR1 | CR1_UE;
}

// A received byte, if there is one. Errors (noise while the other end starts
// up, an overrun) are cleared and otherwise left to the frame's check.
template <const UARTInit u>
inline bool uart_read(uint8_t& byte) {
    using namespace uart_bit;
    USART1_Type& regs = uart_regs<u>();
    const uint32_t status = regs.USART_ISR;
    if (status & ISR_ERRORS) regs.USART_ICR = status & ISR_ERRORS;
    if (!(status & ISR_RXFNE)) return false;
    byte = static_cast<uint8_t>(regs.USART_RDR);
    return true;
}

// Queues a byte to send; false if the fifo is full
template <const UARTInit u>
inline bool uart_write(uint8_t byte) {
    USART1_Type& regs = uart_regs<u>();
    if (!(regs.USART_ISR & uart_bit::ISR_TXFNF)) return false;
    regs.USART_TDR = byte;
    return true;
}

// Bytes from an interrupt to the background, or the other way: one of each.
// A power of two long; a byte that does not fit is dropped.
template <std::size_t size>
struct ByteQueue {
    static_assert((size & (size - 1)) == 0);
    volatile uint8_t data[size];
    volatile uint32_t head = 0, tail = 0;
    bool put(uint8_t byte) {
        const uint32_t h = head;
        if (h - tail >= size) return false;
        data[h % size] = byte;
        head = h + 1;
        return true;
    }
    bool get(uint8_t& byte) {
        const uint32_t t = tail;
        if (t == head) return false;
        byte = data[t % size];
        tail = t + 1;
        return true;
    }
};
}
