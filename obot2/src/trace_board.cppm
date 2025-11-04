module;

import obot_std;
import stm32g474;

export module trace_board;

export class TraceBoard {
  public:
    constexpr static uint32_t cpu_frequency = 170'000'000; // 170 MHz
    constexpr static uint32_t hse_frequency = 24'000'000; // 24 MHz
    TraceBoard() {
    }
};

namespace cpu = stm32g474;
//export using cpu; //<TraceBoard::cpu_frequency, TraceBoard::hse_frequency>;
// This is called from the startup code
extern "C" void stm32g474_init() {
    cpu::enable_cyccnt();
    cpu::enable_boost_mode();
    cpu::set_flash_wait_states();
    cpu::use_hse();
    cpu::set_isr_vector_table();
    cpu::enable_crs();
    cpu::enable_peripheral_clocks();
    cpu::enable_fpu();
    cpu::enable_usb_pins();
}
