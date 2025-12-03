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
        std::string_view pin;
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


export struct GPIORegsInit {
  struct Regs {
    uint32_t moder;
    uint32_t otyper;
    uint32_t ospeedr;
    uint32_t pupdr;
    uint32_t bsrr;
    uint32_t lckr;
    uint32_t afr[2];
  } a, b, c, d, e, f, g;
};

export consteval const GPIORegsInit::Regs get_gpio_regs_init(const decltype(GPIOInit::a)& g) {
  GPIORegsInit::Regs init{};
  uint32_t moder = 0;
  for (int i = 0; i < 16; i++) {
    moder |= static_cast<uint32_t>(g[i].mode) << 2 * i;
  }
  init.moder = moder;

  uint32_t otyper = 0;
  for (int i = 0; i < 16; i++) {
    otyper |= static_cast<uint32_t>(g[i].otype) << i;
  }
  init.otyper = otyper;

  uint32_t ospeedr = 0;
  for (int i = 0; i < 16; i++) {
    ospeedr |= static_cast<uint32_t>(g[i].speed) << 2 * i;
  }
  init.ospeedr = ospeedr;

  uint32_t pupdr = 0;
  for (int i = 0; i < 16; i++) {
    pupdr |= static_cast<uint32_t>(g[i].pullupd) << 2 * i;
  }
  init.pupdr = pupdr;

  uint32_t af1 = 0;
  for (int i = 0; i < 8; i++) {
    af1 |= static_cast<uint32_t>(g[i].af) << 4 * i;
  }
  init.afr[0] = af1;

  uint32_t af2 = 0;
  for (int i = 0; i < 8; i++) {
    af2 |= static_cast<uint32_t>(g[i + 8].af) << 4 * i;
  }
  init.afr[1] = af2;
  uint32_t bsrr = 0;
  for (int i = 0; i < 16; i++) {
    if (g[i].bsrr == GPIOSet::SET) {
      bsrr |= 1 << i;
    } else if (g[i].bsrr == GPIOSet::RESET) {
      bsrr |= 1 << (16 + i);
    }
  }
  init.bsrr = bsrr;
  // todo lckr
  return init;
}

export consteval const GPIORegsInit get_gpio_regs_init(const GPIOInit& g) {
  GPIORegsInit init{};
  init.a = get_gpio_regs_init(g.a);
  init.b = get_gpio_regs_init(g.b);
  init.c = get_gpio_regs_init(g.c);
  init.d = get_gpio_regs_init(g.d);
  init.e = get_gpio_regs_init(g.e);
  init.f = get_gpio_regs_init(g.f);
  init.g = get_gpio_regs_init(g.g);
  return init;
}

export constexpr void init_gpio_regs(GPIORegs auto regs, const GPIORegsInit::Regs& g) {
  regs->MODER = g.moder;
  regs->OTYPER = g.otyper;
  regs->OSPEEDR = g.ospeedr;
  regs->PUPDR = g.pupdr;
  regs->BSRR = g.bsrr;
  regs->LCKR = g.lckr;
  regs->AFRL = g.afr[0];
  regs->AFRH = g.afr[1];
}
