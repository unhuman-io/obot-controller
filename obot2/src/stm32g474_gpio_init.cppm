export module stm32g474_gpio_init;

import obot_std;

export enum class GPIOMode : unsigned int {
    INPUT = 0,
    OUTPUT = 1,
    ALTERNATE = 2,
    ANALOG = 3,
};

export enum class GPIOOType : unsigned int {
    PUSH_PULL = 0,
    OPEN_DRAIN = 1,
};

export enum class GPIOSpeed : unsigned int {
    LOW = 0,
    MEDIUM = 1,
    HIGH = 2,
    VERY_HIGH = 3,
};

export enum class GPIOPullUpD : unsigned int {
    NO_PULL = 0,
    PULL_UP = 1,
    PULL_DOWN = 2,
};

export enum class GPIOSet : unsigned int {
    NO_SET = 0,
    RESET = 1,
    SET = 2,
};

export enum class GPIOLock : unsigned int {
    NO_LOCK = 0,
    LOCK = 1,
};

export enum class GPIOAF : unsigned int {
    AF0 = 0,
    AF1 = 1,
    AF2 = 2,
    AF3 = 3,
    AF4 = 4,
    AF5 = 5,
    AF6 = 6,
    AF7 = 7,
    AF8 = 8,
    AF9 = 9,
    AF10 = 10,
    AF11 = 11,
    AF12 = 12,
    AF13 = 13,
    AF14 = 14,
    AF15 = 15,
};

export struct GPIOInit {
    struct {
        std::string_view name;
        std::string_view function;
        GPIOMode mode;
        GPIOOType otype;
        GPIOSpeed speed;
        GPIOPullUpD pullupd;
        GPIOSet bsrr;
        GPIOLock lck;
        GPIOAF af;
    } a[16], b[16], c[16], d[16], e[16], f[16], g[16];
};

template<typename Regs>
concept GPIORegs = requires(Regs &regs) {
    { regs->MODER } -> std::same_as<volatile uint32_t&>;
    { regs->OTYPER } -> std::same_as<volatile uint32_t&>;
    { regs->OSPEEDR } -> std::same_as<volatile uint32_t&>;
    { regs->PUPDR } -> std::same_as<volatile uint32_t&>;
    { regs->BSRR } -> std::same_as<volatile uint32_t&>;
    { regs->LCKR } -> std::same_as<volatile uint32_t&>;
    { regs->AFRL } -> std::same_as<volatile uint32_t&>;
    { regs->AFRH } -> std::same_as<volatile uint32_t&>;
};

export constexpr void init_gpio_regs(GPIORegs auto regs, const decltype(GPIOInit::a) &g) {
    uint32_t moder = 0;
    for (int i = 0; i < 16; i++) {
        moder |= static_cast<uint32_t>(g[i].mode) << 2*i;
    }
    regs->MODER = moder;

    uint32_t otyper = 0;
    for (int i = 0; i < 16; i++) {
        otyper |= static_cast<uint32_t>(g[i].otype) << i;
    }
    regs->OTYPER = otyper;

    uint32_t ospeedr = 0;
    for (int i = 0; i < 16; i++) {
        ospeedr |= static_cast<uint32_t>(g[i].speed) << 2*i;
    }
    regs->OSPEEDR = ospeedr;

    uint32_t pupdr = 0;
    for (int i = 0; i < 16; i++) {
        pupdr |= static_cast<uint32_t>(g[i].pullupd) << 2*i;
    }
    regs->PUPDR = pupdr;

    uint32_t af1 = 0;
    for (int i = 0; i < 8; i++) {
        af1 |= static_cast<uint32_t>(g[i].af) << 4*i;
    }
    regs->AFRL = af1;

    uint32_t af2 = 0;
    for (int i = 0; i < 8; i++) {
        af2 |= static_cast<uint32_t>(g[i+8].af) << 4*i;
    }
    regs->AFRH = af2;

    uint32_t bsrr = 0;
    for (int i = 0; i < 16; i++ ) {
        if (g[i].bsrr == GPIOSet::SET) {
            bsrr |= 1 << i;
        } else if (g[i].bsrr == GPIOSet::RESET) {
            bsrr |= 1 << (16 + i);
        }
    }
    regs->BSRR = bsrr;

    uint32_t lckr1 = 0;
    uint32_t lckr2 = 0;
}


