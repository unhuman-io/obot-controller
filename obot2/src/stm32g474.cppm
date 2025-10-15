module;

#include "STM32G474.h"
import obot_std;

export module stm32g474;

#undef CRC
#undef IWDG
#undef WWDG
#undef I2C1
#undef I2C2
#undef I2C3
#undef I2C4
#undef FLASH
#undef DBGMCU
#undef RCC
#undef PWR
#undef RNG
#undef GPIOA
#undef GPIOB
#undef GPIOC
#undef GPIOD
#undef GPIOE
#undef GPIOF
#undef GPIOG
#undef TIM15
#undef TIM16
#undef TIM17
#undef TIM1
#undef TIM20
#undef TIM8
#undef TIM2
#undef TIM3
#undef TIM4
#undef TIM5
#undef TIM6
#undef TIM7
#undef LPTIMER1
#undef USART1
#undef USART2
#undef USART3
#undef UART4
#undef UART5
#undef LPUART1
#undef SPI1
#undef SPI4
#undef SPI3
#undef SPI2
#undef EXTI
#undef RTC
#undef FMC
#undef DMA1
#undef DMA2
#undef DMAMUX
#undef SYSCFG
#undef VREFBUF
#undef COMP
#undef OPAMP
#undef HRTIM_Master
#undef HRTIM_TIMA
#undef HRTIM_TIMB
#undef HRTIM_TIMC
#undef HRTIM_TIMD
#undef HRTIM_TIME
#undef HRTIM_TIMF
#undef HRTIM_Common
#undef QUADSPI
#undef DAC1
#undef DAC2
#undef DAC3
#undef DAC4
#undef ADC1
#undef ADC2
#undef ADC3
#undef ADC4
#undef ADC5
#undef ADC12_Common
#undef ADC345_Common
#undef FMAC
#undef CORDIC
#undef SAI
#undef TAMP
#undef FDCAN
#undef FDCAN1
#undef FDCAN2
#undef FDCAN3
#undef UCPD1
#undef USB_FS_device
#undef CRS


export CRC_Type * const CRC = (CRC_Type *) CRC_BASE;
export IWDG_Type * const IWDG = (IWDG_Type *) IWDG_BASE;
export WWDG_Type * const WWDG = (WWDG_Type *) WWDG_BASE;
export I2C1_Type * const I2C1 = (I2C1_Type *) I2C1_BASE;
export I2C1_Type * const I2C2 = (I2C1_Type *) I2C2_BASE;
export I2C1_Type * const I2C3 = (I2C1_Type *) I2C3_BASE;
export I2C1_Type * const I2C4 = (I2C1_Type *) I2C4_BASE;
export FLASH_Type * const FLASH = (FLASH_Type *) FLASH_BASE;
export DBGMCU_Type * const DBGMCU = (DBGMCU_Type *) DBGMCU_BASE;
export RCC_Type * const RCC = (RCC_Type *) RCC_BASE;
export PWR_Type * const PWR = (PWR_Type *) PWR_BASE;
export RNG_Type * const RNG = (RNG_Type *) RNG_BASE;
export GPIOA_Type * const GPIOA = (GPIOA_Type *) GPIOA_BASE;
export GPIOB_Type * const GPIOB = (GPIOB_Type *) GPIOB_BASE;
export GPIOC_Type * const GPIOC = (GPIOC_Type *) GPIOC_BASE;
export GPIOC_Type * const GPIOD = (GPIOC_Type *) GPIOD_BASE;
export GPIOC_Type * const GPIOE = (GPIOC_Type *) GPIOE_BASE;
export GPIOC_Type * const GPIOF = (GPIOC_Type *) GPIOF_BASE;
export GPIOC_Type * const GPIOG = (GPIOC_Type *) GPIOG_BASE;
export TIM15_Type * const TIM15 = (TIM15_Type *) TIM15_BASE;
export TIM16_Type * const TIM16 = (TIM16_Type *) TIM16_BASE;
export TIM16_Type * const TIM17 = (TIM16_Type *) TIM17_BASE;
export TIM1_Type * const TIM1 = (TIM1_Type *) TIM1_BASE;
export TIM1_Type * const TIM20 = (TIM1_Type *) TIM20_BASE;
export TIM1_Type * const TIM8 = (TIM1_Type *) TIM8_BASE;
export TIM2_Type * const TIM2 = (TIM2_Type *) TIM2_BASE;
export TIM2_Type * const TIM3 = (TIM2_Type *) TIM3_BASE;
export TIM2_Type * const TIM4 = (TIM2_Type *) TIM4_BASE;
export TIM2_Type * const TIM5 = (TIM2_Type *) TIM5_BASE;
export TIM6_Type * const TIM6 = (TIM6_Type *) TIM6_BASE;
export TIM6_Type * const TIM7 = (TIM6_Type *) TIM7_BASE;
export LPTIMER1_Type * const LPTIMER1 = (LPTIMER1_Type *) LPTIMER1_BASE;
export USART1_Type * const USART1 = (USART1_Type *) USART1_BASE;
export USART1_Type * const USART2 = (USART1_Type *) USART2_BASE;
export USART1_Type * const USART3 = (USART1_Type *) USART3_BASE;
export UART4_Type * const UART4 = (UART4_Type *) UART4_BASE;
export UART4_Type * const UART5 = (UART4_Type *) UART5_BASE;
export LPUART1_Type * const LPUART1 = (LPUART1_Type *) LPUART1_BASE;
export SPI1_Type * const SPI1 = (SPI1_Type *) SPI1_BASE;
export SPI4_Type * const SPI4 = (SPI4_Type *) SPI4_BASE;
export SPI4_Type * const SPI3 = (SPI4_Type *) SPI3_BASE;
export SPI4_Type * const SPI2 = (SPI4_Type *) SPI2_BASE;
export EXTI_Type * const EXTI = (EXTI_Type *) EXTI_BASE;
export RTC_Type * const RTC = (RTC_Type *) RTC_BASE;
export FMC_Type * const FMC = (FMC_Type *) FMC_BASE;
export DMA1_Type * const DMA1 = (DMA1_Type *) DMA1_BASE;
export DMA1_Type * const DMA2 = (DMA1_Type *) DMA2_BASE;
export DMAMUX_Type * const DMAMUX = (DMAMUX_Type *) DMAMUX_BASE;
export SYSCFG_Type * const SYSCFG = (SYSCFG_Type *) SYSCFG_BASE;
export VREFBUF_Type * const VREFBUF = (VREFBUF_Type *) VREFBUF_BASE;
export COMP_Type * const COMP = (COMP_Type *) COMP_BASE;
export OPAMP_Type * const OPAMP = (OPAMP_Type *) OPAMP_BASE;
export HRTIM_Master_Type * const HRTIM_Master = (HRTIM_Master_Type *) HRTIM_Master_BASE;
export HRTIM_TIMA_Type * const HRTIM_TIMA = (HRTIM_TIMA_Type *) HRTIM_TIMA_BASE;
export HRTIM_TIMB_Type * const HRTIM_TIMB = (HRTIM_TIMB_Type *) HRTIM_TIMB_BASE;
export HRTIM_TIMC_Type * const HRTIM_TIMC = (HRTIM_TIMC_Type *) HRTIM_TIMC_BASE;
export HRTIM_TIMD_Type * const HRTIM_TIMD = (HRTIM_TIMD_Type *) HRTIM_TIMD_BASE;
export HRTIM_TIME_Type * const HRTIM_TIME = (HRTIM_TIME_Type *) HRTIM_TIME_BASE;
export HRTIM_TIMF_Type * const HRTIM_TIMF = (HRTIM_TIMF_Type *) HRTIM_TIMF_BASE;
export HRTIM_Common_Type * const HRTIM_Common = (HRTIM_Common_Type *) HRTIM_Common_BASE;
export QUADSPI_Type * const QUADSPI = (QUADSPI_Type *) QUADSPI_BASE;
export DAC1_Type * const DAC1 = (DAC1_Type *) DAC1_BASE;
export DAC1_Type * const DAC2 = (DAC1_Type *) DAC2_BASE;
export DAC1_Type * const DAC3 = (DAC1_Type *) DAC3_BASE;
export DAC1_Type * const DAC4 = (DAC1_Type *) DAC4_BASE;
export ADC1_Type * const ADC1 = (ADC1_Type *) ADC1_BASE;
export ADC1_Type * const ADC2 = (ADC1_Type *) ADC2_BASE;
export ADC3_Type * const ADC3 = (ADC3_Type *) ADC3_BASE;
export ADC1_Type * const ADC4 = (ADC1_Type *) ADC4_BASE;
export ADC3_Type * const ADC5 = (ADC3_Type *) ADC5_BASE;
export ADC12_Common_Type * const ADC12_Common = (ADC12_Common_Type *) ADC12_Common_BASE;
export ADC12_Common_Type * const ADC345_Common = (ADC12_Common_Type *) ADC345_Common_BASE;
export FMAC_Type * const FMAC = (FMAC_Type *) FMAC_BASE;
export CORDIC_Type * const CORDIC = (CORDIC_Type *) CORDIC_BASE;
export SAI_Type * const SAI = (SAI_Type *) SAI_BASE;
export TAMP_Type * const TAMP = (TAMP_Type *) TAMP_BASE;
export FDCAN_Type * const FDCAN = (FDCAN_Type *) FDCAN_BASE;
export FDCAN_Type * const FDCAN1 = (FDCAN_Type *) FDCAN1_BASE;
export FDCAN_Type * const FDCAN2 = (FDCAN_Type *) FDCAN2_BASE;
export FDCAN_Type * const FDCAN3 = (FDCAN_Type *) FDCAN3_BASE;
export UCPD1_Type * const UCPD1 = (UCPD1_Type *) UCPD1_BASE;
export USB_FS_device_Type * const USB_FS_Device = (USB_FS_device_Type *) USB_FS_device_BASE;
export CRS_Type * const CRS = (CRS_Type *) CRS_BASE;

export template<uint32_t cpu_frequency = 170'000'000,
                uint32_t hse_frequency = 24'000'000>
class stm32g474 {
  public:

    static void enable_cyccnt() {
        CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk; // enable trace
        DWT->CYCCNT = 0; // reset cycle counter
        DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk; // enable cycle counter
    }

    static uint32_t cyccnt() {
        return DWT->CYCCNT;
    }

    // Enable boost mode for > 150 MHz operation
    // Requires enable_cyccnt() to be called first
    static void enable_boost_mode() {
        PWR->PWR_CR5_b.R1MODE = 0; // R1MODE -> 0 for > 150 MHz operation
        // PWR_CR5_R1MODE change recommends 1 us startup
        // startup clock is at 16 MHz
        auto t_start = cyccnt();
        while((cyccnt()-t_start) < (16'000'000/1'000'000));
    }

    static void set_flash_wait_states() {
        FLASH->ACR_b = {
            .LATENCY = 4, // 4 flash wait states for 170 MHz
            .PRFTEN = 1, // enable prefetch
            .ICEN = 1, // enable instruction cache
            .DCEN = 1, // enable data cache
            .DBG_SWEN = 1, // enable debug
        };
    }

    static void use_hse() {
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
        RCC->RCC_CFGR_b.SW = 3; // PLL clock
    }

    static void wait_ms(uint32_t ms) {
        auto t_start = cyccnt();
        while((cyccnt() - t_start) < (170'000'000 / 1000 * ms));
    }

}; // namespace stm32g474_fun


