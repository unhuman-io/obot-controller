module;

#include "STM32G474.h"
import obot_std;

export module stm32g474;

export import stm32g474_gpio_init;

export {
#include "generated/STM32G474_peripherals.cppm.gen"
}

export {
    using ::IRQn_Type;
}

// from core_cm4.h
export {
#undef SCnSCB
extern "C" SCnSCB_Type SCnSCB_address;
constexpr SCnSCB_Type * const SCnSCB { &SCnSCB_address };

#undef SCB
extern "C" SCB_Type SCB_address;
constexpr SCB_Type * const SCB { &SCB_address };

#undef SysTick
extern "C" SysTick_Type SysTick_address;
constexpr SysTick_Type * const SysTick { &SysTick_address };

#undef NVIC
extern "C" NVIC_Type NVIC_address;
constexpr NVIC_Type * const NVIC { &NVIC_address };

#undef ITM
extern "C" ITM_Type ITM_address;
constexpr ITM_Type * const ITM { &ITM_address };

#undef DWT
extern "C" DWT_Type DWT_address;
constexpr DWT_Type * const DWT { &DWT_address };

#undef TPI
extern "C" TPI_Type TPI_address;
constexpr TPI_Type * const TPI { &TPI_address };

#undef CoreDebug
extern "C" CoreDebug_Type CoreDebug_address;
constexpr CoreDebug_Type * const CoreDebug { &CoreDebug_address };

#undef FPU
extern "C" FPU_Type FPU_address;
constexpr FPU_Type * const FPU { &FPU_address };

#undef NVIC_EnableIRQ
    void NVIC_EnableIRQ(IRQn_Type IRQn) {
        __NVIC_EnableIRQ(IRQn);
    }

#undef NVIC_SystemReset
    void NVIC_SystemReset() {
        __NVIC_SystemReset();
    }

    void set_nvic_priority(IRQn_Type IRQn, uint32_t priority) {
        __NVIC_SetPriority(IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), priority, 0));
    }
}

export volatile uint32_t go_to_bootloader;

// export template<uint32_t cpu_frequency1 = 170'000'000,
//                 uint32_t hse_frequency1 = 24'000'000>
export namespace stm32g474 {
    constexpr uint32_t hse_frequency = 24'000'000; // 24 MHz
    constexpr uint32_t cpu_frequency = 170'000'000; // 170 MHz

    void enable_cyccnt() {
        CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk; // enable trace
        DWT->CYCCNT = 0; // reset cycle counter
        DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk; // enable cycle counter
    }

    uint32_t cyccnt() {
        return DWT->CYCCNT;
    }

    // Enable boost mode for > 150 MHz operation
    // Requires enable_cyccnt() to be called first
    void enable_boost_mode() {
        PWR->PWR_CR5_b.R1MODE = 0; // R1MODE -> 0 for > 150 MHz operation
        // PWR_CR5_R1MODE change recommends 1 us startup
        // startup clock is at 16 MHz
        auto t_start = cyccnt();
        while((cyccnt()-t_start) < (16'000'000/1'000'000));
    }

    void set_flash_wait_states() {
        FLASH->FLASH_ACR_b = {
            .LATENCY = 4, // 4 flash wait states for 170 MHz
            .PRFTEN = 1, // enable prefetch
            .ICEN = 1, // enable instruction cache
            .DCEN = 1, // enable data cache
            .DBG_SWEN = 1, // enable debug
        };
    }

    void use_hse() {
        static_assert(hse_frequency == 24'000'000, "HSE frequency must be 24 MHz");
        static_assert(cpu_frequency/2'000'000*2'000'000 == cpu_frequency, "CPU frequency must be even multiple of 2 MHz");
        RCC->RCC_PLLCFGR_b = {
            .PLLSRC = 3, // HSE is pll source (24 MHz)
            .PLLM = 5, // div6
            .PLLN = cpu_frequency/2'000'000, // x85
            //.PLLPEN = 1, // enable P output
            .PLLQ = 0, // 0: div2
            //.PLLQEN = 1, // enable Q output
            .PLLREN = 1, // enable R output
            .PLLR = 0, // 0:div2
            .PLLPDIV = 2, // 2: div2
        };
        // RCC->RCC_CR_b.HSION = 1; // enable HSI
        // RCC->RCC_CR_b.HSEON = 1; // enable HSE
        // RCC->RCC_CR_b.PLLON = 1; // enable PLL
        RCC->RCC_CR = std::bit_cast<decltype(RCC->RCC_CR)>(
            decltype(RCC->RCC_CR_b){
            .HSION = 1, // enable HSI
            .HSEON = 1, // enable HSE
            .PLLON = 1 // enable PLL
        });
        while(!RCC->RCC_CR_b.PLLRDY); // wait for PLL ready
        RCC->RCC_CFGR_b.HPRE = 1; // AHB prescaler /2
        RCC->RCC_CFGR_b.SW = 3; // PLL clock
        auto t_start = cyccnt();
        while((cyccnt()-t_start) < (85'000'000/1'000'000));
        RCC->RCC_CFGR_b.HPRE = 0; // AHB prescaler /1
    }

    void set_isr_vector_table() {
        // Set the interrupt vector table location
        // We are using the default location at the start of flash (0x08000000)
        SCB->VTOR = 0x0000000;
    }

    void enable_crs() {
        RCC->RCC_CRRCR_b.HSI48ON = 1;
        while (!RCC->RCC_CRRCR_b.HSI48RDY);
        RCC->RCC_APB1ENR1_b.CRSEN = 1; // enable CRS clock
        RCC->RCC_APB1SMENR1_b.CRSSMEN = 1; // enable CRS clock in sleep mode
        CRS->CFGR_b = {
            .RELOAD = (48'000'000/1'000) - 1, // 1 kHz target
            .FELIM = 34,
            .SYNCSRC = 2, //source is USB SOF
        };
        CRS->CR_b = {
            .CEN = 1,
            .AUTOTRIMEN = 1,
        };
    }

    void enable_peripheral_clocks() {
        RCC->RCC_APB1ENR1_b.USBEN = 1;
        RCC->RCC_AHB2ENR_b.GPIOAEN = 1;
    }

    void enable_fpu() {
        SCB->CPACR |= ((3UL << (10 * 2)) | (3UL << (11 * 2))); /* set CP10 and CP11 Full Access */
    }

    void wait_ms(uint32_t ms) {
        auto t_start = stm32g474::cyccnt();
        while((stm32g474::cyccnt() - t_start) < (cpu_frequency / 1000 * ms));
    }

    uint32_t us_to_cyccnt(uint32_t us) {
        return (cpu_frequency / 1'000'000) * us;
    }

    constexpr void init_gpio(const GPIORegsInit& g) {
        init_gpio_regs(GPIOA, g.a);
        init_gpio_regs(GPIOB, g.b);
        init_gpio_regs(GPIOC, g.c);
        init_gpio_regs(GPIOD, g.d);
        init_gpio_regs(GPIOE, g.e);
        init_gpio_regs(GPIOF, g.f);
        init_gpio_regs(GPIOG, g.g);
    }

}; // namespace stm32g474_fun


