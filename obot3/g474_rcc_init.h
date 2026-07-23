#pragma once
#include "stm32g474xx.h"

enum class RCCEnable : bool {
    DISABLED,
    ENABLED
};

struct RCCInit {
    // AHB1
    RCCEnable crc       = RCCEnable::DISABLED;
    RCCEnable flash     = RCCEnable::ENABLED;
    RCCEnable fmac      = RCCEnable::DISABLED;
    RCCEnable cordic    = RCCEnable::DISABLED;
    RCCEnable dmamux1   = RCCEnable::DISABLED;
    RCCEnable dma2      = RCCEnable::DISABLED;
    RCCEnable dma1      = RCCEnable::DISABLED;

    // AHB2
    RCCEnable rng       = RCCEnable::DISABLED;
    RCCEnable dac4      = RCCEnable::DISABLED;
    RCCEnable dac3      = RCCEnable::DISABLED;
    RCCEnable dac2      = RCCEnable::DISABLED;
    RCCEnable dac1      = RCCEnable::DISABLED;
    RCCEnable adc345    = RCCEnable::DISABLED;
    RCCEnable adc12     = RCCEnable::DISABLED;
    RCCEnable gpiog     = RCCEnable::DISABLED;
    RCCEnable gpiof     = RCCEnable::DISABLED;
    RCCEnable gpioe     = RCCEnable::DISABLED;
    RCCEnable gpiod     = RCCEnable::DISABLED;
    RCCEnable gpioc     = RCCEnable::DISABLED;
    RCCEnable gpiob     = RCCEnable::DISABLED;
    RCCEnable gpioa     = RCCEnable::DISABLED;

    // AHB3
    RCCEnable qspi      = RCCEnable::DISABLED;
    RCCEnable fmc       = RCCEnable::DISABLED;

    // APB1ENR1
    RCCEnable lptim1    = RCCEnable::DISABLED;
    RCCEnable i2c3      = RCCEnable::DISABLED;
    RCCEnable pwr       = RCCEnable::DISABLED;
    RCCEnable fdcan     = RCCEnable::DISABLED;
    RCCEnable usb       = RCCEnable::DISABLED;
    RCCEnable i2c2      = RCCEnable::DISABLED;
    RCCEnable i2c1      = RCCEnable::DISABLED;
    RCCEnable uart5     = RCCEnable::DISABLED;
    RCCEnable uart4     = RCCEnable::DISABLED;
    RCCEnable usart3    = RCCEnable::DISABLED;
    RCCEnable usart2    = RCCEnable::DISABLED;
    RCCEnable spi3      = RCCEnable::DISABLED;
    RCCEnable spi2      = RCCEnable::DISABLED;
    RCCEnable wwdg      = RCCEnable::DISABLED;
    RCCEnable rtcapb    = RCCEnable::DISABLED;
    RCCEnable crs       = RCCEnable::ENABLED;
    RCCEnable tim7      = RCCEnable::DISABLED;
    RCCEnable tim6      = RCCEnable::DISABLED;
    RCCEnable tim5      = RCCEnable::DISABLED;
    RCCEnable tim4      = RCCEnable::DISABLED;
    RCCEnable tim3      = RCCEnable::DISABLED;
    RCCEnable tim2      = RCCEnable::DISABLED;

    // APB1ENR2
    RCCEnable ucpd1     = RCCEnable::DISABLED;
    RCCEnable i2c4      = RCCEnable::DISABLED;
    RCCEnable lpuart1   = RCCEnable::DISABLED;

    // APB2
    RCCEnable hrtim1    = RCCEnable::DISABLED;
    RCCEnable sai1      = RCCEnable::DISABLED;
    RCCEnable tim20     = RCCEnable::DISABLED;
    RCCEnable tim17     = RCCEnable::DISABLED;
    RCCEnable tim16     = RCCEnable::DISABLED;
    RCCEnable tim15     = RCCEnable::DISABLED;
    RCCEnable spi4      = RCCEnable::DISABLED;
    RCCEnable usart1    = RCCEnable::DISABLED;
    RCCEnable tim8      = RCCEnable::DISABLED;
    RCCEnable spi1      = RCCEnable::DISABLED;
    RCCEnable tim1      = RCCEnable::DISABLED;
    RCCEnable syscfg    = RCCEnable::DISABLED; 
};

template<const RCCInit r>
inline consteval uint32_t get_ahb1enr() {
    return static_cast<uint32_t>(r.crc) << RCC_AHB1ENR_CRCEN_Pos |
        static_cast<uint32_t>(r.flash) << RCC_AHB1ENR_FLASHEN_Pos |
        static_cast<uint32_t>(r.fmac) << RCC_AHB1ENR_FMACEN_Pos |
        static_cast<uint32_t>(r.cordic) << RCC_AHB1ENR_CORDICEN_Pos |
        static_cast<uint32_t>(r.dmamux1) << RCC_AHB1ENR_DMAMUX1EN_Pos |
        static_cast<uint32_t>(r.dma2) << RCC_AHB1ENR_DMA2EN_Pos |
        static_cast<uint32_t>(r.dma1) << RCC_AHB1ENR_DMA1EN_Pos;
}

template<const RCCInit r>
inline consteval uint32_t get_ahb2enr() {
    return static_cast<uint32_t>(r.rng) << RCC_AHB2ENR_RNGEN_Pos |
        static_cast<uint32_t>(r.dac4) << RCC_AHB2ENR_DAC4EN_Pos |
        static_cast<uint32_t>(r.dac3) << RCC_AHB2ENR_DAC3EN_Pos |
        static_cast<uint32_t>(r.dac2) << RCC_AHB2ENR_DAC2EN_Pos |
        static_cast<uint32_t>(r.dac1) << RCC_AHB2ENR_DAC1EN_Pos |
        static_cast<uint32_t>(r.adc345) << RCC_AHB2ENR_ADC345EN_Pos |
        static_cast<uint32_t>(r.adc12) << RCC_AHB2ENR_ADC12EN_Pos |
        static_cast<uint32_t>(r.gpiog) << RCC_AHB2ENR_GPIOGEN_Pos |
        static_cast<uint32_t>(r.gpiof) << RCC_AHB2ENR_GPIOFEN_Pos |
        static_cast<uint32_t>(r.gpioe) << RCC_AHB2ENR_GPIOEEN_Pos |
        static_cast<uint32_t>(r.gpiod) << RCC_AHB2ENR_GPIODEN_Pos |
        static_cast<uint32_t>(r.gpioc) << RCC_AHB2ENR_GPIOCEN_Pos |
        static_cast<uint32_t>(r.gpiob) << RCC_AHB2ENR_GPIOBEN_Pos |
        static_cast<uint32_t>(r.gpioa) << RCC_AHB2ENR_GPIOAEN_Pos;
}

template<const RCCInit r>
inline consteval uint32_t get_ahb3enr() {
    return static_cast<uint32_t>(r.qspi) << RCC_AHB3ENR_QSPIEN_Pos |
        static_cast<uint32_t>(r.fmc) << RCC_AHB3ENR_FMCEN_Pos;
}

template<const RCCInit r>
inline consteval uint32_t get_apb1enr1() {
    return static_cast<uint32_t>(r.lptim1) << RCC_APB1ENR1_LPTIM1EN_Pos |
        static_cast<uint32_t>(r.i2c3) << RCC_APB1ENR1_I2C3EN_Pos |
        static_cast<uint32_t>(r.pwr) << RCC_APB1ENR1_PWREN_Pos |
        static_cast<uint32_t>(r.fdcan) << RCC_APB1ENR1_FDCANEN_Pos |
        static_cast<uint32_t>(r.usb) << RCC_APB1ENR1_USBEN_Pos |
        static_cast<uint32_t>(r.i2c2) << RCC_APB1ENR1_I2C2EN_Pos |
        static_cast<uint32_t>(r.i2c1) << RCC_APB1ENR1_I2C1EN_Pos |
        static_cast<uint32_t>(r.uart5) << RCC_APB1ENR1_UART5EN_Pos |
        static_cast<uint32_t>(r.uart4) << RCC_APB1ENR1_UART4EN_Pos |
        static_cast<uint32_t>(r.usart3) << RCC_APB1ENR1_USART3EN_Pos |
        static_cast<uint32_t>(r.usart2) << RCC_APB1ENR1_USART2EN_Pos |
        static_cast<uint32_t>(r.spi3) << RCC_APB1ENR1_SPI3EN_Pos |
        static_cast<uint32_t>(r.spi2) << RCC_APB1ENR1_SPI2EN_Pos |
        static_cast<uint32_t>(r.wwdg) << RCC_APB1ENR1_WWDGEN_Pos |
        static_cast<uint32_t>(r.rtcapb) << RCC_APB1ENR1_RTCAPBEN_Pos |
        static_cast<uint32_t>(r.crs) << RCC_APB1ENR1_CRSEN_Pos |
        static_cast<uint32_t>(r.tim7) << RCC_APB1ENR1_TIM7EN_Pos |
        static_cast<uint32_t>(r.tim6) << RCC_APB1ENR1_TIM6EN_Pos |
        static_cast<uint32_t>(r.tim5) << RCC_APB1ENR1_TIM5EN_Pos |
        static_cast<uint32_t>(r.tim4) << RCC_APB1ENR1_TIM4EN_Pos |
        static_cast<uint32_t>(r.tim3) << RCC_APB1ENR1_TIM3EN_Pos |
        static_cast<uint32_t>(r.tim2) << RCC_APB1ENR1_TIM2EN_Pos;
}

template<const RCCInit r>
inline consteval uint32_t get_apb1enr2() {
    return static_cast<uint32_t>(r.ucpd1) << RCC_APB1ENR2_UCPD1EN_Pos |
        static_cast<uint32_t>(r.i2c4) << RCC_APB1ENR2_I2C4EN_Pos |
        static_cast<uint32_t>(r.lpuart1) << RCC_APB1ENR2_LPUART1EN_Pos;
}

template<const RCCInit r>
inline consteval uint32_t get_apb2enr() {
    return static_cast<uint32_t>(r.hrtim1) << RCC_APB2ENR_HRTIM1EN_Pos |
        static_cast<uint32_t>(r.sai1) << RCC_APB2ENR_SAI1EN_Pos |
        static_cast<uint32_t>(r.tim20) << RCC_APB2ENR_TIM20EN_Pos |
        static_cast<uint32_t>(r.tim17) << RCC_APB2ENR_TIM17EN_Pos |
        static_cast<uint32_t>(r.tim16) << RCC_APB2ENR_TIM16EN_Pos |
        static_cast<uint32_t>(r.tim15) << RCC_APB2ENR_TIM15EN_Pos |
        static_cast<uint32_t>(r.spi4) << RCC_APB2ENR_SPI4EN_Pos |
        static_cast<uint32_t>(r.usart1) << RCC_APB2ENR_USART1EN_Pos |
        static_cast<uint32_t>(r.tim8) << RCC_APB2ENR_TIM8EN_Pos |
        static_cast<uint32_t>(r.spi1) << RCC_APB2ENR_SPI1EN_Pos |
        static_cast<uint32_t>(r.tim1) << RCC_APB2ENR_TIM1EN_Pos |
        static_cast<uint32_t>(r.syscfg) << RCC_APB2ENR_SYSCFGEN_Pos;
}

template<const RCCInit r>
inline constexpr void init_rcc() {
    if constexpr (get_ahb1enr<r>() != get_ahb1enr<RCCInit{}>()) {
        RCC->AHB1ENR = get_ahb1enr<r>();
    }
    if constexpr (get_ahb2enr<r>() != get_ahb2enr<RCCInit{}>()) {
        RCC->AHB2ENR = get_ahb2enr<r>();
    }
    if constexpr (get_ahb3enr<r>() != get_ahb3enr<RCCInit{}>()) {
        RCC->AHB3ENR = get_ahb3enr<r>();
    }
    if constexpr (get_apb1enr1<r>() != get_apb1enr1<RCCInit{}>()) {
        RCC->APB1ENR1 = get_apb1enr1<r>();
    }
    if constexpr (get_apb1enr2<r>() != get_apb1enr2<RCCInit{}>()) {
        RCC->APB1ENR2 = get_apb1enr2<r>();
    }
    if constexpr (get_apb2enr<r>() != get_apb2enr<RCCInit{}>()) {
        RCC->APB2ENR = get_apb2enr<r>();
    }
}

template<RCCInit rcc_init>
inline constexpr void g474_rcc_config() {
    init_rcc<rcc_init>();
}