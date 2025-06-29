module;

import stm32g474;
import <bit>;

export module trace_board;

export class TraceBoard {
  public:
    TraceBoard() {
        enable_cyccnt();
        enable_boost_mode();
        set_flash_wait_states();
        use_hse();
    }

    void enable_boost_mode() {
        PWR->PWR_CR5_b.R1MODE = 0; // R1MODE -> 0 for > 150 MHz operation
        // PWR_CR5_R1MODE change recommends 1 us startup
        // startup clock is at 16 MHz
        auto t_start = cyccnt();
        while((cyccnt()-t_start) < (16'000'000/1'000'000));
    }

    void set_flash_wait_states() {
        FLASH->ACR_b = {
            .LATENCY = 4, // 4 flash wait states for 170 MHz
            .PRFTEN = 1, // enable prefetch
            .ICEN = 1, // enable instruction cache
            .DCEN = 1, // enable data cache
            .DBG_SWEN = 1, // enable debug
        };
    }
    void use_hse() {
        RCC->RCC_PLLCFGR_b = {
            .PLLSRC = 3, // HSE is pll source (24 MHz)
            .PLLM = 5, // div6
            .PLLN = 170'000'000/2'000'000, // x85
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
        RCC->RCC_CFGR_b.SW = 3; // PLL clock
    }
};
