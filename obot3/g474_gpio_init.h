#pragma once

#include <string_view>
#include <cstdint>
#include <concepts>
#include "stm32g474xx.h"

enum class GPIOMode : unsigned int {
  INPUT = 0,
  OUTPUT = 1,
  ALTERNATE = 2,
  ANALOG = 3,
};

enum class GPIOOType : unsigned int {
  PUSH_PULL = 0,
  OPEN_DRAIN = 1,
};

enum class GPIOSpeed : unsigned int {
  LOW = 0,
  MEDIUM = 1,
  HIGH = 2,
  VERY_HIGH = 3,
};

enum class GPIOPullUpD : unsigned int {
  NO_PULL = 0,
  PULL_UP = 1,
  PULL_DOWN = 2,
};

enum class GPIOSet : unsigned int {
  NO_SET = 0,
  RESET = 1,
  SET = 2,
};

enum class GPIOLock : unsigned int {
  NO_LOCK = 0,
  LOCK = 1,
};

enum class GPIOAF : unsigned int {
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

struct GPIOInit {
  struct {
    const char* name;
    const char* net;
    const char* function;
    const char* pin;
    GPIOMode mode = GPIOMode::ANALOG;
    GPIOOType otype = GPIOOType::PUSH_PULL;
    GPIOSpeed speed = GPIOSpeed::LOW;
    GPIOPullUpD pullupd = GPIOPullUpD::NO_PULL;
    GPIOSet bsrr = GPIOSet::NO_SET;
    GPIOLock lck = GPIOLock::NO_LOCK;
    GPIOAF af = GPIOAF::AF0;
  } a[16], b[16], c[16], d[16], e[16], f[16], g[16];
};

inline constexpr GPIOInit get_default_gpio_init() {
    GPIOInit init {};
    init.a[13].mode = GPIOMode::ALTERNATE;
    init.a[14].mode = GPIOMode::ALTERNATE;
    init.a[15].mode = GPIOMode::ALTERNATE;
    init.b[3].mode = GPIOMode::ALTERNATE;
    init.b[4].mode = GPIOMode::ALTERNATE;
    init.a[13].speed = GPIOSpeed::VERY_HIGH;
    init.a[13].pullupd = GPIOPullUpD::PULL_UP;
    init.a[14].pullupd = GPIOPullUpD::PULL_DOWN;
    init.a[15].pullupd = GPIOPullUpD::PULL_UP;
    init.b[4].pullupd = GPIOPullUpD::PULL_UP;
    return init;
}

template <typename Regs>
concept GPIORegs = requires(Regs& regs) {
  { regs->MODER } -> std::same_as<volatile uint32_t&>;
  { regs->OTYPER } -> std::same_as<volatile uint32_t&>;
  { regs->OSPEEDR } -> std::same_as<volatile uint32_t&>;
  { regs->PUPDR } -> std::same_as<volatile uint32_t&>;
  { regs->BSRR } -> std::same_as<volatile uint32_t&>;
  { regs->LCKR } -> std::same_as<volatile uint32_t&>;
  { regs->AFR[0] } -> std::same_as<volatile uint32_t&>;
  { regs->AFR[1] } -> std::same_as<volatile uint32_t&>;
};

struct GPIORegsInit {
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

inline consteval const GPIORegsInit::Regs get_gpio_regs_init(const decltype(GPIOInit::a)& g) {
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

inline consteval const GPIORegsInit get_gpio_regs_init(const GPIOInit& g) {
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

template<const GPIORegsInit::Regs g, const GPIORegsInit::Regs g_default>
inline constexpr void init_gpio_regs(GPIORegs auto regs) {
  if constexpr (g.moder != g_default.moder) regs->MODER = g.moder;
  if constexpr (g.otyper != g_default.otyper) regs->OTYPER = g.otyper;
  if constexpr (g.ospeedr != g_default.ospeedr) regs->OSPEEDR = g.ospeedr;
  if constexpr (g.pupdr != g_default.pupdr) regs->PUPDR = g.pupdr;
  if constexpr (g.bsrr != g_default.bsrr) regs->BSRR = g.bsrr;
  if constexpr (g.lckr != g_default.lckr) regs->LCKR = g.lckr;
  if constexpr (g.afr[0] != g_default.afr[0]) regs->AFR[0] = g.afr[0];
  if constexpr (g.afr[1] != g_default.afr[1]) regs->AFR[1] = g.afr[1];
}

template<const GPIORegsInit g>
inline constexpr void init_gpio() {
  constexpr GPIORegsInit g_default = get_gpio_regs_init(get_default_gpio_init());
  init_gpio_regs<g.a , g_default.a>(GPIOA);
  init_gpio_regs<g.b , g_default.b>(GPIOB);
  init_gpio_regs<g.c , g_default.c>(GPIOC);
  init_gpio_regs<g.d , g_default.d>(GPIOD);
  init_gpio_regs<g.e , g_default.e>(GPIOE);
  init_gpio_regs<g.f , g_default.f>(GPIOF);
  init_gpio_regs<g.g , g_default.g>(GPIOG);
}

template<GPIOInit gpio_init>
inline constexpr void g474_pin_config() { 
    init_gpio<get_gpio_regs_init(gpio_init)>();
}
