#include <atomic>
#include <cstring>
#include "../motorlib/task.h"   // device header first: the device.* modules carry it in their global module fragment
#include "../motorlib/motor_messages/motor_messages.h"
#include "system.h"
#include "basic_config.h"
#include "obot_enum.h"
#include "bootloader/bootloader_protocol.h"
// OBOT3_TEXT_API 1: the text api, its variables and gdb. 0, the release build:
// neither, and a host has the enum, command and status frames.
#if OBOT3_TEXT_API
#include "text_api.h"
#include "debug_monitor.h"
#endif
#include "debug_call.h"
#include <initializer_list>
#include <iterator>
import tmotor_driver;

template<auto config = TMotorConfig{}>
struct Config : BasicConfig<config> {};

using MySystem = System<Config<>>;

// How long the loops take and how far apart they start, cpu cycles
volatile uint32_t t_exec_fastloop, t_period_fastloop, t_exec_mainloop, t_period_mainloop;

// ADC5 end of injected sequence, 50 kHz
void fast_loop_isr() {
    static uint32_t t_last;
    const uint32_t t_start = DWT->CYCCNT;
    std::atomic_signal_fence(std::memory_order_acq_rel);
    MySystem::fast_loop.update();
    tmotor_fast_loop_irq_clear();
    t_period_fastloop = t_start - t_last;
    t_last = t_start;
    t_exec_fastloop = DWT->CYCCNT - t_start;
}

// HRTIM master cmp1, 10 kHz. The time it takes includes the fast loops that
// interrupt it. For gdb it is thread 2, the background thread 1 and the call
// thread, which runs functions for gdb from os_loop, thread 3: see debug_monitor.h.
void main_loop_update() {
    static uint32_t t_last;
    const uint32_t t_start = DWT->CYCCNT;
    std::atomic_signal_fence(std::memory_order_acq_rel);
    MySystem::main_loop.update();
    tmotor_main_loop_irq_clear();
    t_period_mainloop = t_start - t_last;
    t_last = t_start;
    t_exec_mainloop = DWT->CYCCNT - t_start;
}

#if OBOT3_TEXT_API
DEBUG_THREAD_ENTRY(main_loop_isr, 2, main_loop_update)
#else
void main_loop_isr() { main_loop_update(); }
#endif

// main.cpp polls this from the background loop. Here the main loop has its
// own interrupt.
void tim2_isr() {}

#if OBOT3_TEXT_API
// The fast loop preempts everything. The text api comes next, and the debug
// monitor either between it and the main loop or below the main loop: gdb can
// stop and step the main loop and the background, or the background alone,
// with the fast loop running and the node answering.
void text_isr();
constexpr uint8_t text_priority = 1, debug_monitor_priority = 2, main_loop_priority = 3, debug_monitor_low_priority = 4;
constexpr auto tmotor_irq_init = []() consteval {
    IRQInit init{};
    init.irq[ADC5_IRQn] = {.handler = fast_loop_isr, .priority = 0};
    init.irq[FDCAN3_IT0_IRQn] = {.handler = text_isr, .priority = text_priority};
    init.irq[HRTIM1_Master_IRQn] = {.handler = main_loop_isr, .priority = main_loop_priority};
    return init;
}();
constexpr DebugThread debug_threads[] = {
    {.name = "background"},
    {.name = "main_loop", .irq = HRTIM1_Master_IRQn, .entry = main_loop_isr, .priority = main_loop_priority},
};

extern "C" void Reset_Handler();
__attribute__((section(".isr_vector"), used))
constinit const auto vector_table = []() consteval {
    auto table = get_vector_table<tmotor_irq_init>(&_estack, Reset_Handler);
    table.vector[DebugMonitor_IRQn + 15] = debug_monitor_entry;
    table.vector[PendSV_IRQn + 15] = debug_holder_entry;
    table.vector[HardFault_IRQn + 15] = debug_hard_fault_entry;
    return table;
}();
#else
constexpr auto tmotor_irq_init = []() consteval {
    IRQInit init{};
    init.irq[ADC5_IRQn] = {.handler = fast_loop_isr, .priority = 0};
    init.irq[HRTIM1_Master_IRQn] = {.handler = main_loop_isr, .priority = 3};
    return init;
}();

extern "C" void Reset_Handler();
__attribute__((section(".isr_vector"), used))
constinit const auto vector_table = get_vector_table<tmotor_irq_init>(&_estack, Reset_Handler);
#endif

void clock_config() {
    device_rcc_clock_config<tmotor_rcc_clock_init>();
}

// Whatever takes time at startup, in parallel as coroutines on a scheduler
// local to this function: the calibration of the four converters, and the
// gate driver's wake up and register frames. The driver comes up with the pwm
// outputs off: its inputs are low until the hrtim starts.
void tmotor_startup() {
    CycleScheduler sched;
    auto adc1 = adc_calibrate_async<1>(sched);
    auto adc3 = adc_calibrate_async<3>(sched);
    auto adc4 = adc_calibrate_async<4>(sched);
    auto adc5 = adc_calibrate_async<5>(sched);
    auto drv8323 = drv8323_config_async<tmotor_drv8323_init, TMotorDRV8323IO>(sched);
    while (!adc1.is_done() || !adc3.is_done() || !adc4.is_done() || !adc5.is_done() || !drv8323.is_done()) {
        sched.poll();
    }
    tmotor_drv8323_configured(drv8323.get_result());
}

void debug_idle();

void pin_config() {
    device_rcc_config<tmotor_rcc_init>();
    device_pin_config<tmotor_gpio_init>();
    tmotor_startup_init();
    tmotor_startup();
    tmotor_device_init();
#if OBOT3_TEXT_API
    debug_monitor_init(debug_threads, std::size(debug_threads), debug_monitor_priority, debug_monitor_low_priority, debug_idle);
    debug_call_init();      // thread 3: functions for gdb, run from os_loop
    can_interrupt_enable<tmotor_can_init>(can_interrupt_rx_fifo[tmotor_can_ascii_fifo]);
#endif
    device_irq_config<tmotor_irq_init>(vector_table);   // last: the loops run from here on
}

// Background, in place of the 1 kHz system loop of the obot_g474
CycleScheduler sched;
volatile uint32_t* const cpu_clock = &DWT->CYCCNT;

// The watchdog, which the bootloader starts, is fed from the background loop
// while this holds. It drops when a loop stops, and when no host has talked to
// this node for a while: either way the watchdog reset ends in the bootloader,
// where new firmware can be loaded. A firmware that can't be reached is never
// left running.
constexpr uint32_t status_frequency_hz = 10;
constexpr uint32_t startup_ticks = status_frequency_hz;             // 1 s for the loops to start
constexpr uint32_t host_timeout_ticks = 60 * status_frequency_hz;   // 60 s without a frame from a host
volatile bool watchdog_ok = true;
volatile uint32_t host_idle_ticks = 0;
volatile bool reset_request = false, bootloader_request = false;
volatile uint32_t temperature_reading, vrefint_reading;

// 10 Hz: heartbeat on the green led, the watchdog condition and the slow ADC
// channels. The injected sequences take 6 us per channel, so the results of
// one pass are read at the start of the next.
GlobalTask<void, 1, 160> status_task(CycleScheduler &sched) {
    constexpr uint32_t PERIOD_CYCLES = CPU_FREQUENCY_HZ / status_frequency_hz;
    uint32_t target_wake_time = get_clock();
    uint32_t ticks = 0;
    uint32_t last_fast_loop_count = 0, last_main_loop_count = 0;
    while (1) {
        target_wake_time += PERIOD_CYCLES;
        if (static_cast<int32_t>(get_clock() - target_wake_time) > 0) target_wake_time = get_clock();   // gdb had it stopped
        co_await sched.delay_until(target_wake_time);   // first: the task starts before pin_config
        if (reset_request) {                            // a tick after the request, so the reply is out
            if (bootloader_request) system_reset_to_bootloader();
            system_reset();
        }

        ticks++;
        tmotor_led(0, ticks & 1 ? 0x2000 : 0, 0);

        const uint32_t fast_loop_count = MySystem::fast_loop.count;
        const uint32_t main_loop_count = MySystem::main_loop.sensors.count;
        const bool loops_run = fast_loop_count != last_fast_loop_count && main_loop_count != last_main_loop_count;
        last_fast_loop_count = fast_loop_count;
        last_main_loop_count = main_loop_count;
        host_idle_ticks = host_idle_ticks + 1;
        watchdog_ok = ticks < startup_ticks || (loops_run && host_idle_ticks < host_timeout_ticks);

        temperature_reading = tmotor_temperature();
        vrefint_reading = tmotor_vrefint();
        adc_start_injected<1>();
        adc_start_injected<2>();
    }
}

auto status_task_instance = status_task(sched);

#if OBOT3_TEXT_API
// What runs while gdb has the node stopped, in place of the background: the
// watchdog, with the same host timeout, and the reset
void debug_idle() {
    static uint32_t t_tick;
    if (get_clock() - t_tick > CPU_FREQUENCY_HZ / status_frequency_hz) {
        t_tick = get_clock();
        host_idle_ticks = host_idle_ticks + 1;
    }
    if (reset_request && can_tx_idle<tmotor_can_init>()) {
        if (bootloader_request) system_reset_to_bootloader();
        system_reset();
    }
    if (host_idle_ticks < host_timeout_ticks) watchdog_kick();
}

// The loops by a short name, for the variables below and for gdb:
// `p fast_loop.controller` instead of the System<...> they are members of
DEBUG_KEEP constinit auto& fast_loop = MySystem::fast_loop;
DEBUG_KEEP constinit auto& main_loop = MySystem::main_loop;

// The obot text api over can: requests are strings to the ascii id of this
// address, and each gets one reply.
//   name, serial, version, messages_version     what a host needs to find this node
//   go_to_bootloader=b007, reset                and to get it back into the bootloader
//   <variable>, <variable>=<value>              the variables below, see text_api.h
//   api_length, api_name=<n>, help              their number and names
//   $...                                        gdb remote protocol packets, see text_api.h and debug_monitor.h
// An unknown request gets an empty reply. The interrupt that takes the
// requests in preempts the main loop, so it only serves the gdb packets
// itself; the others are left for the background, see text_poll.
constexpr float fast_dt = 1.f / tmotor_pwm_frequency_hz;
#define TUNE(name, value) text_float(name, [] { return static_cast<float>(value); }, [](float v) { value = v; })
#define FAST_STATUS(name, member) text_float(name, [] { return static_cast<float>(fast_loop.get_status().member); })
constexpr TextVariable text_variables[] = {
    // the current controllers, in use by the fast loop
    TUNE("ikp", fast_loop.controller.pi_q().kp),
    TUNE("iki", fast_loop.controller.pi_q().ki),
    TUNE("iki_limit", fast_loop.controller.pi_q().ki_limit),
    TUNE("imax", fast_loop.controller.pi_q().command_max),                  // V
    TUNE("idkp", fast_loop.controller.pi_d().kp),
    TUNE("idki", fast_loop.controller.pi_d().ki),
    TUNE("idki_limit", fast_loop.controller.pi_d().ki_limit),
    TUNE("idmax", fast_loop.controller.pi_d().command_max),
    text_float("iq_filter", [] { return fast_loop.controller.get_current_filter_frequency(); },    // Hz, both axes
        [](float hz) { fast_loop.controller.set_current_filter_frequency(hz); }),
    TUNE("vbus", fast_loop.sensors.vbus),                                   // V, what the pwm scale assumes
    text_int("electrical_zero_pos", [] { return static_cast<int64_t>(fast_loop.sensors.electrical_zero); },
        [](int64_t counts) { fast_loop.sensors.electrical_zero = counts; }),

    // the position controller
    TUNE("kp", main_loop.controller.position_controller.pid().kp_),
    TUNE("kd", main_loop.controller.position_controller.pid().kd_),
    TUNE("ki", main_loop.controller.position_controller.pid().ki_),
    TUNE("ki_limit", main_loop.controller.position_controller.pid().ki_limit_),
    TUNE("max", main_loop.controller.position_controller.pid().command_max_),          // A
    text_float("velocity_filter", [] { return main_loop.controller.position_controller.pid().get_velocity_filter_frequency(); },
        [](float hz) { main_loop.controller.position_controller.pid().set_velocity_filter_frequency(hz); }),
    text_float("output_filter", [] { return main_loop.controller.position_controller.pid().get_output_filter_frequency(); },
        [](float hz) { main_loop.controller.position_controller.pid().set_output_filter_frequency(hz); }),

    // the velocity controller
    TUNE("vkp", main_loop.controller.velocity_controller.pid().kp_),
    TUNE("vki", main_loop.controller.velocity_controller.pid().ki_),
    TUNE("vki_limit", main_loop.controller.velocity_controller.pid().ki_limit_),
    TUNE("vmax", main_loop.controller.velocity_controller.pid().command_max_),
    text_float("voutput_filt", [] { return main_loop.controller.velocity_controller.pid().get_output_filter_frequency(); },
        [](float hz) { main_loop.controller.velocity_controller.pid().set_output_filter_frequency(hz); }),

    // what the fast loop measures and puts out: A, V, rad, rad/s
    FAST_STATUS("ia", measured.i_a),
    FAST_STATUS("ib", measured.i_b),
    FAST_STATUS("ic", measured.i_c),
    FAST_STATUS("id", controller.i_d),
    FAST_STATUS("iq", controller.i_q),
    FAST_STATUS("i0", controller.i_0),
    FAST_STATUS("id_des", controller.i_d_desired),
    FAST_STATUS("iq_des", controller.i_q_desired),
    FAST_STATUS("va", controller.v_a),
    FAST_STATUS("vb", controller.v_b),
    FAST_STATUS("vc", controller.v_c),
    FAST_STATUS("vd", controller.v_d),
    FAST_STATUS("vq", controller.v_q),
    FAST_STATUS("motor_position", measured.motor_position),
    FAST_STATUS("motor_velocity", measured.motor_velocity),
    text_float("output_position", [] { return main_loop.controller.status.read().joint_position; }),
    text_float("torque", [] { return main_loop.controller.status.read().torque; }),
    text_float("num_poles", [] { return tmotor_num_poles; }),

    text_int("mode", [] { return static_cast<int64_t>(main_loop.controller.status.read().flags.mode); }),
    text_int("menc", [] { return static_cast<int64_t>(main_loop.controller.status.read().motor_encoder); }),
    text_int("IA", [] { return static_cast<int64_t>(tmotor_ia()); }),      // adc counts
    text_int("IB", [] { return static_cast<int64_t>(tmotor_ib()); }),
    text_int("IC", [] { return static_cast<int64_t>(tmotor_ic()); }),
    text_int("temperature", [] { return static_cast<int64_t>(temperature_reading); }),
    text_int("vrefint", [] { return static_cast<int64_t>(vrefint_reading); }),
    text_int("drv_errors", [] { return static_cast<int64_t>(tmotor_drv8323_errors); }),     // 0: configured
    text_int("drv_fault", [] { return static_cast<int64_t>(tmotor_drv8323_fault()); }),     // the fault pin
    text_int("drv_status", [] { return static_cast<int64_t>(tmotor_drv8323_status()); }),   // status 2 << 16 | status 1
    text_int("invalid_command_count", [] { return static_cast<int64_t>(main_loop.controller.invalid_command_count); }),

    // timing, cpu cycles
    text_int("fast_loop_count", [] { return static_cast<int64_t>(fast_loop.count); }),
    text_int("main_loop_count", [] { return static_cast<int64_t>(main_loop.sensors.count); }),
    text_int("t_exec_fastloop", [] { return static_cast<int64_t>(t_exec_fastloop); }),
    text_int("t_exec_mainloop", [] { return static_cast<int64_t>(t_exec_mainloop); }),
    text_int("t_period_fastloop", [] { return static_cast<int64_t>(t_period_fastloop); }),
    text_int("t_period_mainloop", [] { return static_cast<int64_t>(t_period_mainloop); }),
    text_int("cpu_frequency", [] { return static_cast<int64_t>(CPU_FREQUENCY_HZ); }),
    text_int("debug_probe", [] { return static_cast<int64_t>(debug_probe_attached()); }),    // 1: no step or breakpoints for gdb
    text_int("breakpoints_dropped", [] { return static_cast<int64_t>(debug.breakpoints_dropped); }),   // set above the debug monitor
};
#undef TUNE
#undef FAST_STATUS

struct TextMemory {
    static uint32_t read(uint32_t address, uint8_t* data, uint32_t length) { return memory_read(address, data, length); }
    static uint32_t write(uint32_t address, const uint8_t* data, uint32_t length) { return memory_write(address, data, length); }
};

void text_request(const char* request, TextOut& out) {
    if (request[0] == '$') {
        if (!debug_request(request, out)) gdb_request<TextMemory>(request, out, text_request);
    } else if (std::strcmp(request, "name") == 0) {
        out.put("obot3_tmotor_driver");
    } else if (std::strcmp(request, "serial") == 0) {
        char serial[13];
        system_serial_number(serial);
        out.put(serial);
    } else if (std::strcmp(request, "messages_version") == 0) {
        out.put(MOTOR_MESSAGES_VERSION);
    } else if (std::strcmp(request, "version") == 0) {
        out.put(OBOT3_VERSION);
    } else if (std::strcmp(request, "go_to_bootloader=b007") == 0) {
        bootloader_request = true;      // for the next reset
        out.put("b007");
    } else if (std::strcmp(request, "reset") == 0) {
        reset_request = true;
    } else if (std::strcmp(request, "api_length") == 0) {
        out.put_number(static_cast<int64_t>(std::size(text_variables)));
    } else if (std::strncmp(request, "api_name=", 9) == 0) {
        uint32_t index = 0;
        const char* number = request + 9;
        std::from_chars(number, number + std::strlen(number), index);
        if (index < std::size(text_variables)) out.put(text_variables[index].name);
    } else if (std::strcmp(request, "help") == 0) {
        out.put_number(static_cast<int64_t>(std::size(text_variables)));
        out.put(" variables:\n");
        for (const TextVariable& variable : text_variables) {
            out.put(variable.name);
            out.put('\n');
        }
    } else {
        text_variable_request(text_variables, request, out);
    }
}

// A reply of up to 63 characters is one frame, with its terminator if it has
// two or more characters, as the obot_g474 does. A longer one goes out as the
// long packets of motor_messages.h, in frames that the can fd lengths don't
// pad: a host takes every byte of those as text. Three frames fit the tx
// fifo; the rest follows from the interrupt when it is empty.
char text_reply[1200];
uint32_t text_reply_total, text_reply_offset;
uint16_t text_reply_number;

void text_send_more() {
    const uint32_t id = obot_can_id(obot_can_ascii_response, tmotor_can_address());
    struct {
        APIControlPacket header;
        char data[56];
    } frame;
    while (text_reply_offset < text_reply_total) {
        uint32_t size = 56;
        for (const uint32_t fit : {56, 40, 24, 16, 12, 8, 4}) {
            size = fit;
            if (fit <= text_reply_total - text_reply_offset) break;
        }
        frame.header = {.control_packet_id = 0, .type = LONG_PACKET,
            .long_packet = {.total_length = static_cast<uint16_t>(text_reply_total), .packet_number = text_reply_number}};
        std::memcpy(frame.data, text_reply + text_reply_offset, size);
        if (!can_write<tmotor_can_init>(id, &frame, sizeof(frame.header) + size)) {
            can_interrupt_enable<tmotor_can_init>(can_interrupt_tx_empty);
            return;
        }
        text_reply_offset += size;
        text_reply_number++;
    }
    can_interrupt_disable<tmotor_can_init>(can_interrupt_tx_empty);
}

void text_send(TextOut& out) {
    text_reply_total = 0;
    if (out.length() < 64) {
        can_write<tmotor_can_init>(obot_can_id(obot_can_ascii_response, tmotor_can_address()),
            out.text(), out.length() > 1 ? out.length() + 1 : out.length());
        return;
    }
    while (out.length() % 4 != 0 && out.room() != 0) out.put(' ');
    text_reply_total = out.length() & ~3u;
    if (out.text() != text_reply) std::memcpy(text_reply, out.text(), text_reply_total);
    text_reply_offset = 0;
    text_reply_number = 1;
    text_send_more();
}

// A request that the interrupt left for the background, the latest one
char text_pending[65];
volatile bool text_pending_request;

// FDCAN3: a text request has come in, or the rest of a long reply can go out.
// What it interrupted is what gdb gets as the registers while the node runs.
// The gdb packets are served here, which is what lets gdb stop the background
// and the main loop, and so is everything else while those are stopped.
extern "C" [[gnu::used]] void text_isr_frame(DebugFrame* frame) {
    debug.sample = frame;
    can_interrupt_flags<tmotor_can_init>();
    uint32_t id;
    char data[65];
    for (int length; (length = can_read<tmotor_can_init>(tmotor_can_ascii_fifo, id, data)) >= 0;) {
        data[length] = 0;
        host_idle_ticks = 0;
        if (data[0] == '$' || debug.stopped) {
            TextOut out(text_reply, sizeof(text_reply));
            text_request(data, out);
            text_send(out);
        } else {
            std::memcpy(text_pending, data, length + 1);
            text_pending_request = true;
        }
    }
    if (text_reply_offset < text_reply_total) text_send_more();
    debug.sample = nullptr;
}
DEBUG_FRAME_ENTRY(text_isr, text_isr_frame)

// The background's frames share the tx fifo with the text api's
struct TextLock {
    TextLock() { __set_BASEPRI(text_priority << (8 - __NVIC_PRIO_BITS)); }
    ~TextLock() { __set_BASEPRI(0); }
};

// The text requests that are no gdb packets, from the background: the main
// loop is not held up by them. The reply shares the interrupt's way out.
void text_poll() {
    if (!text_pending_request) return;
    char request[sizeof(text_pending)];
    {
        const TextLock lock;
        std::memcpy(request, text_pending, sizeof(request));
        text_pending_request = false;
    }
    static char reply[sizeof(text_reply)];
    TextOut out(reply, sizeof(reply));
    text_request(request, out);
    const TextLock lock;
    text_send(out);
}

#else
struct TextLock { TextLock() {} };     // nothing else sends
void text_poll() {}
#endif

// What a host has without the text api, as the obot_g474:
//   enum             to all nodes: an empty reply, that this one is here. To this
//                    one: its EnumResponse
//   command          a new MotorCommand. BOARD_RESET as its mode resets the node
//   command status   that and a request for the status
//   status           only the request
// and on the command id the 8 byte BOOTLOADER_ENTER request of
// bootloader_protocol.h, which is answered and ends in the bootloader.
void host_poll() {
    uint32_t id;
    char data[64];
    if (const int length = can_read<tmotor_can_init>(tmotor_can_command_fifo, id, data); length >= 0) {
        host_idle_ticks = 0;
        const uint32_t message = id >> 7;
        if (message == obot_can_enum) {
            uint8_t serial[6];
            system_serial_bytes(serial);
            const obot::EnumResponse response = obot_enum_response(OBOT3_TEXT_API ? "obot3_tmotor_driver" : "obot3_tmotor_release", serial);
            const bool to_all = (id & 0x7F) == obot_can_broadcast_address;
            const TextLock lock;
            can_write<tmotor_can_init>(obot_can_id(obot_can_enum, tmotor_can_address()), &response, to_all ? 0 : sizeof(response));
        } else if (message == obot_can_command && length == offsetof(BootloaderRequest, data)) {
            if (static_cast<uint8_t>(data[0]) == BOOTLOADER_ENTER) {
                BootloaderReply reply{};
                reply.command = BOOTLOADER_ENTER | BOOTLOADER_REPLY;
                reply.sequence = data[1];
                bootloader_request = true;
                reset_request = true;
                const TextLock lock;
                can_write<tmotor_can_init>(obot_can_id(obot_can_status, tmotor_can_address()), &reply, offsetof(BootloaderReply, data));
            }
        } else {
            if (message != obot_can_status && length >= static_cast<int>(sizeof(obot::MotorCommand))) {
                obot::MotorCommand command;
                std::memcpy(&command, data, sizeof(command));
                TMotorHostCommand::latest.write(command);
                if (command.mode_desired == obot::BOARD_RESET) reset_request = true;
            }
            if (message != obot_can_command) {
                const auto status = MySystem::main_loop.controller.status.read();
                const TextLock lock;
                can_write<tmotor_can_init>(obot_can_id(obot_can_status, tmotor_can_address()), status);
            }
        }
    }
}

void os_loop() {
#if OBOT3_TEXT_API
    debug_call_poll();      // a function that gdb wants run, between the tasks of this loop
#endif
    text_poll();
    host_poll();
    sched.poll();
    if (watchdog_ok) watchdog_kick();
}
