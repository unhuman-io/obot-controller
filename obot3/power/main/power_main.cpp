// The power board's main controller (STM32G564): it measures the battery, the
// motor bus and the computer's two supplies, switches the 12 V output, is on
// the CAN bus, and is one of the two that have to agree before the motor bus
// is on. The other is the radio module (../radio), which is always powered,
// starts this controller by starting the 5 V converter, and tells it twenty
// times a second what the remote says. What is decided here is in
// power.logic (MainLogic) and tested on a PC; this file is the hardware
// around it.
//
// Not here yet: the battery pack's data line (its protocol is not known), the
// text api and the debug monitor of the other obot3 firmware, a serial number
// in the enum reply.
#include <atomic>
#include <cstring>
#include "../motorlib/task.h"   // device header first: the device.* modules carry it in their global module fragment
#include "obot_enum.h"
#include "board.h"
import power.messages;
import power.logic;

volatile uint32_t* const cpu_clock = &DWT->CYCCNT;
CycleScheduler sched;

// ---- time in milliseconds, from the cycle counter ---------------------------------------
static uint32_t now_ms;
static void time_update() {
    static uint32_t last_cycles, left_over;
    constexpr uint32_t cycles_per_ms = power_cpu_hz / 1000;
    const uint32_t cycles = get_clock();
    left_over += cycles - last_cycles;
    last_cycles = cycles;
    now_ms += left_over / cycles_per_ms;
    left_over %= cycles_per_ms;
}

// ---- the link to the radio module ----------------------------------------------------
static ByteQueue<64> link_received, link_to_send;
void link_isr() {
    uint8_t byte;
    while (uart_read<power_link_init>(byte)) link_received.put(byte);
}

static RadioReport radio;
static uint32_t radio_heard_ms;
static bool radio_heard;

static void link_poll() {
    static LinkReceiver receiver;
    uint8_t byte;
    while (link_received.get(byte)) {
        if (receiver.put(byte) && receiver.kind() == link_kind_radio && receiver.length() == radio_report_size) {
            radio = unpack_radio_report(receiver.payload());
            radio_heard_ms = now_ms;
            radio_heard = true;
        }
    }
    // what is waiting to go out, as far as the UART's fifo takes it
    const uint32_t tail = link_to_send.tail;
    if (tail != link_to_send.head && uart_write<power_link_init>(link_to_send.data[tail % 64])) link_to_send.tail = tail + 1;
}

// ---- what is measured --------------------------------------------------------------------
static struct Measured {
    bool monitor_ok[3];
    int32_t mv[3], ma[3];       // battery, 12 V output, 5 V output
} measured;

static bool monitor_setup(const ina228::Monitor& m) {
    uint8_t id[2];
    if (!i2c_read<power_i2c_init>(m.address, ina228::DEVICE_ID, id, 2)) return false;
    if (((id[0] << 8 | id[1]) & ina228::device_id_mask) != ina228::device_id) return false;
    const uint16_t config = ina228::config(m);
    const uint8_t range[3] = {ina228::CONFIG, static_cast<uint8_t>(config >> 8), static_cast<uint8_t>(config)};
    const uint8_t adc[3] = {ina228::ADC_CONFIG, static_cast<uint8_t>(ina228::adc_config >> 8), static_cast<uint8_t>(ina228::adc_config & 0xFF)};
    return i2c_write<power_i2c_init>(m.address, range, 3) && i2c_write<power_i2c_init>(m.address, adc, 3);
}

// Every 50 ms each monitor's two results, one monitor a pass of the main loop.
// A monitor that does not answer is set up again when it does (it may have
// lost its supply and its settings with it).
GlobalTask<void, 1, 256> monitor_task(CycleScheduler& sched) {
    co_await sched.async_delay_us(2000);        // first: the task starts before pin_config
    co_await adc_calibrate_async<1>(sched);
    adc_start<1>();
    uint32_t wake = get_clock();
    for (;;) {
        for (int k = 0; k < 3; k++) {
            const ina228::Monitor& m = power_monitors[k];
            uint8_t volts[3], shunt[3];
            bool ok = measured.monitor_ok[k] || monitor_setup(m);
            ok = ok && i2c_read<power_i2c_init>(m.address, ina228::VBUS, volts, 3) && i2c_read<power_i2c_init>(m.address, ina228::VSHUNT, shunt, 3);
            if (ok) {
                measured.mv[k] = ina228::bus_mv(volts);
                measured.ma[k] = ina228::current_ma(m, shunt);
            }
            measured.monitor_ok[k] = ok;
            co_await sched.yield();
        }
        wake += power_cpu_hz / 20;
        co_await sched.delay_until(wake);
    }
}
auto monitor_task_instance = monitor_task(sched);

// ---- the watchdog: a controller that hangs is reset, and a reset is "bus off" ------------
static void watchdog_start() {
    IWDG->IWDG_KR = 0xCCCC;         // start (this also starts its 32 kHz oscillator)
    IWDG->IWDG_KR = 0x5555;         // allow the two writes below
    IWDG->IWDG_PR = 3;              // / 32: 1 kHz
    IWDG->IWDG_RLR = 250;           // 250 ms
    for (uint32_t start = get_clock(); (IWDG->IWDG_SR & 3) != 0 && get_clock() - start < power_cpu_hz / 20;) {}
    IWDG->IWDG_KR = 0xAAAA;
}
static void watchdog_feed() { IWDG->IWDG_KR = 0xAAAA; }

// ---- deciding and switching, a hundred times a second -----------------------------------
static MainLogic logic;
static MainReport report;

GlobalTask<void, 2, 256> control_task(CycleScheduler& sched) {
    co_await sched.async_delay_us(2000);
    uint32_t wake = get_clock();
    for (;;) {
        const auto centi = [](int32_t milli) { return static_cast<int16_t>(milli / 10 > 32767 ? 32767 : milli / 10 < -32768 ? -32768
                                                                                                                            : milli / 10); };
        MainInputs in;
        in.now_ms = now_ms;
        in.radio_fresh = radio_heard && now_ms - radio_heard_ms < radio_timeout_ms;
        in.radio = radio;
        in.monitors_ok = measured.monitor_ok[0] && measured.monitor_ok[1] && measured.monitor_ok[2];
        in.battery_cv = static_cast<uint16_t>(measured.mv[0] / 10);
        in.battery_ca = centi(measured.ma[0]);
        in.out12_ca = centi(measured.ma[1]);
        in.out5_ca = centi(measured.ma[2]);
        in.bus_cv = power_bus_cv(adc_read<1>());
        in.bus_good = power_bus_good();
        in.outputs_good = power_outputs_good();
        const MainOutputs out = logic.update(in);
        power_bus_enable(out.bus_enable);
        power_output12(out.output12_on);
        report = out.report;
        // the light: on while the motor bus is enabled, a short flash a second while it is not
        power_led(out.bus_enable || now_ms % 1000 < 50);
        watchdog_feed();
        wake += power_cpu_hz / 100;
        co_await sched.delay_until(wake);
    }
}
auto control_task_instance = control_task(sched);

// ---- telling the radio module and the CAN bus --------------------------------------------
GlobalTask<void, 3, 256> report_task(CycleScheduler& sched) {
    co_await sched.async_delay_us(5000);
    uint32_t wake = get_clock();
    for (int count = 0;; count++) {
        uint8_t frame[link_frame_max];
        const std::size_t length = make_link_frame(report, frame);
        for (std::size_t k = 0; k < length; k++) link_to_send.put(frame[k]);
        if (count % 2 == 0) {
            uint8_t status[can_status_size];
            make_can_status(radio, report, status);
            can_write<power_can_init>(power_can_address | 3u << 7, status, can_status_size);        // an obot status frame
        }
        wake += power_cpu_hz / 20;
        co_await sched.delay_until(wake);
    }
}
auto report_task_instance = report_task(sched);

// A host that asks who is on the bus (the obot enum request, to all nodes or to this one)
static void can_poll() {
    uint32_t id;
    char data[64];
    if (can_read<power_can_init>(0, id, data) < 0) return;
    if (id >> 7 != 0xF) return;
    const uint8_t serial[6]{};
    const obot::EnumResponse response = obot_enum_response("obot3_power", serial);
    const bool to_all = (id & 0x7F) == 0x7F;
    can_write<power_can_init>(power_can_address | 0xFu << 7, &response, to_all ? 0 : sizeof(response));
}

// ---- start up and the main loop (main.cpp) ----------------------------------------------
constexpr auto power_irq_init = []() consteval {
    IRQInit init{};
    init.irq[USART2_IRQn] = {.handler = link_isr, .priority = 1};
    return init;
}();

extern "C" void Reset_Handler();
__attribute__((section(".isr_vector"), used)) constinit const auto vector_table = get_vector_table<power_irq_init>(&_estack, Reset_Handler);

void clock_config() {
    device_rcc_clock_config<power_rcc_clock_init>();
}

void pin_config() {
    device_rcc_config<power_rcc_init>();
    device_pin_config<power_gpio_init>();       // BUS_EN and OUT12_ON low, as they were
    device_can_config<power_can_init>();
    can_filter<power_can_init>(0, power_can_address | 0xFu << 7, 0);
    can_filter<power_can_init>(1, 0x7F | 0xFu << 7, 0);
    device_uart_config<power_link_init>();
    device_i2c_config<power_i2c_init>();
    device_adc_config<power_adc_init>();
    watchdog_start();
    device_irq_config<power_irq_init>(vector_table);   // last: the handlers run from here on
}

void tim2_isr() {}      // (main.cpp's loop calls it: no fast loop on this board)

void os_loop() {
    std::atomic_signal_fence(std::memory_order_acq_rel);
    time_update();
    link_poll();
    can_poll();
    sched.poll();
}
