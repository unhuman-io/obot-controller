#pragma once
#include <cstdint>
// The device header comes first: SPI_TypeDef, DMA_Channel_TypeDef, GPIO_TypeDef

// An spi port that several chips share, each with its own settings and chip
// select. A frame is one chip select low to high.
//
// The tasks of the scheduler take turns on it with the lock:
//     co_await sched.when(Bus::try_lock);
//     Bus::start(chip, out, in, length);      the two dma channels move the frame
//     co_await sched.when(Bus::ready);
//     Bus::finish();
//     Bus::unlock();
// A frame on its way gets through without anyone running, so code that cannot
// wait in the scheduler, a plain function, or the text api's interrupt while a
// debugger has the tasks stopped, does not need the lock to be free:
//     const bool mine = Bus::take();          waits out a frame that is on its way and ends it
//     Bus::select(chip); ... Bus::deselect(chip);
//     Bus::give(mine);
// What was read stays where its dma put it, and the task that started the
// frame finds it done. So the lock keeps the tasks apart, and a frame whole;
// a task that holds it over several frames may find a take() in between them.
//
// spi is the port's number, dma_rx and dma_tx the numbers of two channels of
// DMA1, which is theirs. The clocks of the port, DMA1 and DMAMUX1 are on.
struct SPIChip {
    uint32_t cr1;               // master, baud rate, clock polarity and phase, SPE
    uint32_t cr2;               // the frame length
    GPIO_TypeDef* cs_port;
    uint8_t cs_pin;             // active low
    uint8_t request_rx, request_tx;     // the port's dmamux requests, for the chips that use start()
};

template<int spi, int dma_rx, int dma_tx>
struct SPIBus {
    static SPI_TypeDef& port() {
        if constexpr (spi == 1) return *SPI1;
        else if constexpr (spi == 2) return *SPI2;
        else if constexpr (spi == 3) return *SPI3;
        else return *SPI4;
    }
    static DMA_Channel_TypeDef& channel(int n) { return *reinterpret_cast<DMA_Channel_TypeDef*>(DMA1_Channel1_BASE + (n - 1) * 0x14); }
    static DMAMUX_Channel_TypeDef& mux(int n) { return *reinterpret_cast<DMAMUX_Channel_TypeDef*>(DMAMUX1_Channel0_BASE + (n - 1) * 4); }
    static constexpr uint32_t rx_complete = 2u << 4 * (dma_rx - 1);     // TCIF of the channel, in ISR; IFCR bit 0 of its four clears them
    static constexpr uint32_t clear_flags = 1u << 4 * (dma_rx - 1) | 1u << 4 * (dma_tx - 1);

    static inline volatile bool locked = false;
    static inline volatile bool flying = false;             // a frame is on its way, or through and not ended
    static inline const SPIChip* volatile chip = nullptr;   // whose
    static inline uint8_t sink;                             // what comes in where nobody wants it

    static bool try_lock() {
        if (locked) return false;
        locked = true;
        return true;
    }
    static void unlock() { locked = false; }

    // The port is disabled between frames, which is also what takes its fifos back to empty
    static void select(const SPIChip& c, uint32_t dma_enable = 0) {
        port().CR1 = 0;
        port().CR2 = c.cr2 | dma_enable;
        port().CR1 = c.cr1;                 // on, so the clock is at its idle level when the chip select falls
        c.cs_port->BSRR = 1u << (c.cs_pin + 16);
    }
    static void deselect(const SPIChip& c) {
        while (port().SR & SPI_SR_BSY);
        c.cs_port->BSRR = 1u << c.cs_pin;
        port().CR1 = 0;
    }

    // in may be nullptr: what comes in is dropped. out and in are the caller's
    // until the frame is ended.
    static void start(const SPIChip& c, const uint8_t* out, uint8_t* in, uint32_t length) {
        uint32_t primask;       // on its way or not, for a take() from an interrupt
        __asm__ volatile("mrs %0, primask\n cpsid i" : "=r"(primask));
        channel(dma_rx).CCR = 0;
        channel(dma_tx).CCR = 0;
        DMA1->IFCR = clear_flags;
        mux(dma_rx).CCR = c.request_rx;
        mux(dma_tx).CCR = c.request_tx;
        channel(dma_rx).CPAR = reinterpret_cast<uint32_t>(&port().DR);
        channel(dma_rx).CMAR = reinterpret_cast<uint32_t>(in != nullptr ? in : &sink);
        channel(dma_rx).CNDTR = length;
        channel(dma_tx).CPAR = reinterpret_cast<uint32_t>(&port().DR);
        channel(dma_tx).CMAR = reinterpret_cast<uint32_t>(out);
        channel(dma_tx).CNDTR = length;
        channel(dma_rx).CCR = (in != nullptr ? DMA_CCR_MINC : 0) | DMA_CCR_EN;
        select(c, SPI_CR2_RXDMAEN | SPI_CR2_TXDMAEN);
        channel(dma_tx).CCR = DMA_CCR_MINC | DMA_CCR_DIR | DMA_CCR_EN;
        chip = &c;
        flying = true;
        __asm__ volatile("msr primask, %0" : : "r"(primask));
    }
    static bool ready() { return !flying || (DMA1->ISR & rx_complete) != 0; }
    // Ends the frame that start() began, after the rest of it. More than once does no harm.
    static void finish() {
        if (!flying) return;
        while (!ready());
        uint32_t primask;
        __asm__ volatile("mrs %0, primask\n cpsid i" : "=r"(primask));
        if (flying) {
            channel(dma_tx).CCR = 0;
            channel(dma_rx).CCR = 0;
            deselect(*chip);
            flying = false;
        }
        __asm__ volatile("msr primask, %0" : : "r"(primask));
    }

    // For code that cannot wait in the scheduler. True if the lock was free and is the caller's now.
    static bool take() {
        const bool mine = try_lock();
        if (!mine) finish();
        return mine;
    }
    static void give(bool mine) {
        if (mine) unlock();
    }
};
