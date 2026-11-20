#include <atomic>
#include <cstring>
#include "../motorlib/task.h"   // device header first: the device.* modules carry it in their global module fragment
#include "../motorlib/motor_messages/motor_messages.h"
#include "system.h"
#include "basic_config.h"
#include "obot_enum.h"
#define CYCLE_STATS OBOT3_TEXT_API      // the text api's cpu request is what shows them
#include "cycle_stats.h"
#include "bmi270.h"
#include "bootloader/bootloader_protocol.h"
// OBOT3_TEXT_API 1: the text api, its variables and gdb. 0, the release build:
// neither, and a host has the enum, command and status frames.
#if OBOT3_TEXT_API
#include "text_api.h"
#include "debug_monitor.h"
#endif
#include "debug_call.h"
#include "control/chrono_types.h"
#include <initializer_list>
#include <iterator>
import tmotor_driver;

template<auto config = TMotorConfig{}>
struct Config : BasicConfig<config> {};

using MySystem = System<Config<>>;

// Time, see control/clock.cppm: the steady clock is the cpu's cycle counter
// since pin_config, and the system clock a host's time on it, once the host
// sends it (host_poll). Both are std::chrono clocks; time() of either is
// seconds and nanoseconds.
inline uint32_t clock_cycles() { return DWT->CYCCNT; }
using SteadyClock = CycleClock<tmotor_rcc_clock_init.cpu_frequency_hz, clock_cycles>;
using SystemClock = SyncedClock<SteadyClock>;
using TimeSync = TimeSyncReceiver<SystemClock>;
static_assert(std::chrono::is_clock_v<SteadyClock> && std::chrono::is_clock_v<SystemClock>);
volatile uint32_t time_frames_late;     // time frames that waited too long in host_poll for their count to tell when they came

// How long the loops take and how far apart they start, cpu cycles
volatile uint32_t t_exec_fastloop, t_period_fastloop, t_exec_mainloop, t_period_mainloop;

// The same for every interrupt and every task of the background, with the
// extremes and net of what interrupts them: the text api's cpu request
inline uint32_t stats_clock() { return DWT->CYCCNT; }
using StatsTimer = CycleTimer<stats_clock>;
constinit CycleStats fast_loop_stats{"fast_loop"}, main_loop_stats{"main_loop"}, text_isr_stats{"text_isr"},
    status_task_stats{"status_task"}, imu_task_stats{"imu_task"}, led_task_stats{"led_task"},
    text_task_stats{"text_task"}, host_task_stats{"host_task"}, call_task_stats{"call_task"};
constinit CycleStats* const cycle_stats[] = {&fast_loop_stats, &main_loop_stats, &text_isr_stats,
    &status_task_stats, &imu_task_stats, &led_task_stats, &text_task_stats, &host_task_stats, &call_task_stats};

// ADC5 end of injected sequence, 50 kHz
void fast_loop_isr() {
    const CycleTimer<stats_clock, true> timer(fast_loop_stats);
    static uint32_t t_last;
    const uint32_t t_start = DWT->CYCCNT;
    std::atomic_signal_fence(std::memory_order_acq_rel);
    MySystem::fast_loop.update();
    SteadyClock::update();      // here: this loop is the one that gdb never stops
    tmotor_fast_loop_irq_clear();
    t_period_fastloop = t_start - t_last;
    t_last = t_start;
    t_exec_fastloop = DWT->CYCCNT - t_start;
}

// HRTIM master cmp1, 10 kHz. The time it takes includes the fast loops that
// interrupt it. For gdb it is thread 2, the background thread 1 and the call
// thread, which runs functions for gdb from os_loop, thread 3: see debug_monitor.h.
void main_loop_update() {
    const StatsTimer timer(main_loop_stats);
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
    auto adc2 = adc_calibrate_async<2>(sched);
    auto adc3 = adc_calibrate_async<3>(sched);
    auto adc4 = adc_calibrate_async<4>(sched);
    auto adc5 = adc_calibrate_async<5>(sched);
    auto gate_driver = drv8323::config_async<tmotor_drv8323_init, TMotorDRV8323IO>(sched);
    while (!adc1.is_done() || !adc2.is_done() || !adc3.is_done() || !adc4.is_done() || !adc5.is_done() || !gate_driver.is_done()) {
        sched.poll();
    }
    tmotor_drv8323_configured(gate_driver.get_result());
}

void debug_idle();

volatile uint32_t imu_status;       // chip id << 8 | init status: 0x2401 is a BMI270 that runs
BMI270Data imu_data;

void pin_config() {
    SteadyClock::start();
    device_rcc_config<tmotor_rcc_init>();
    device_pin_config<tmotor_gpio_init>();
    tmotor_startup_init();
    tmotor_startup();
    tmotor_device_init();
#if OBOT3_TEXT_API
    debug_monitor_init(debug_threads, std::size(debug_threads), debug_monitor_priority, debug_monitor_low_priority, debug_idle);
    debug_call_init();      // thread 3: functions for gdb, run from debug_call_task
    tmotor_bus_yield = [] {
        if (!debug_call_can_yield()) return false;
        debug_call_yield();
        return true;
    };
    can_interrupt_enable<tmotor_can_init>(can_interrupt_rx_fifo[tmotor_can_ascii_fifo]);
#endif
    device_irq_config<tmotor_irq_init>(vector_table);   // last: the loops run from here on
}

// Background, in place of the 1 kHz system loop of the obot_g474
CycleScheduler sched;
volatile uint32_t* const cpu_clock = &DWT->CYCCNT;

// The watchdog, which the bootloader starts, is fed from the background loop
// while this holds. It drops when a loop stops: the watchdog reset ends in the
// bootloader, where new firmware can be loaded. A node that no host talks to
// keeps running.
constexpr uint32_t status_frequency_hz = 10;
constexpr uint32_t startup_ticks = status_frequency_hz;             // 1 s for the loops to start
volatile bool watchdog_ok = true;
volatile bool reset_request = false, bootloader_request = false;
uint64_t uptime_cycles;             // as of uptime_clock, which is never more than a turn of the cpu's counter back
uint32_t uptime_clock;
volatile uint32_t temperature_reading, vrefint_reading, motor_thermistor_reading;

// 10 Hz: the watchdog condition and the slow ADC
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
        const StatsTimer timer(status_task_stats);
        uptime_cycles += get_clock() - uptime_clock;
        uptime_clock = get_clock();
        if (reset_request) {                            // a tick after the request, so the reply is out
            if (bootloader_request) system_reset_to_bootloader();
            system_reset();
        }

        ticks++;

        const uint32_t fast_loop_count = MySystem::fast_loop.count;
        const uint32_t main_loop_count = MySystem::main_loop.sensors.count;
        const bool loops_run = fast_loop_count != last_fast_loop_count && main_loop_count != last_main_loop_count;
        last_fast_loop_count = fast_loop_count;
        last_main_loop_count = main_loop_count;
        watchdog_ok = ticks < startup_ticks || loops_run;

        temperature_reading = tmotor_temperature();
        vrefint_reading = tmotor_vrefint();
        motor_thermistor_reading = tmotor_motor_thermistor();
        adc_start_injected<1>();
        adc_start_injected<2>();
    }
}

auto status_task_instance = status_task(sched);

#if OBOT3_TEXT_API
// What runs while gdb has the node stopped, in place of the background: the
// watchdog and the reset
void debug_idle() {
    if (reset_request && can_tx_idle<tmotor_can_init>()) {
        if (bootloader_request) system_reset_to_bootloader();
        system_reset();
    }
    watchdog_kick();
}

// The loops by a short name, for the variables below and for gdb:
// `p fast_loop.controller` instead of the System<...> they are members of
DEBUG_KEEP constinit auto& fast_loop = MySystem::fast_loop;
DEBUG_KEEP constinit auto& main_loop = MySystem::main_loop;

// The module's constexpr values, for gdb: `p foc_param`. A copy here is a value
// in the debug info of the elf, which the module's own are not, and nothing in
// the binary.
constexpr auto foc_param = tmotor_foc_param;
constexpr auto controller_param = tmotor_controller_param;

// The obot text api over can: requests are strings to the ascii id of this
// address, and each gets one reply.
//   name, serial, version, messages_version     what a host needs to find this node
//   go_to_bootloader=b007, reset                and to get it back into the bootloader
//   <variable>, <variable>=<value>              the variables below, see text_api.h
//   api_length, api_name=<n>, help              their number and names
//   cpu                                         the cpu time of the interrupts and tasks, see cpu_request
//   time, uptime                                the system and the steady clock, seconds.nanoseconds
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
    text_int("motor_thermistor", [] { return static_cast<int64_t>(motor_thermistor_reading); }),   // adc counts
    text_float("Tmotor", [] { return tmotor_motor_temperature(motor_thermistor_reading); }),        // deg C
    // the BMI270: g and rad/s
    text_int("imu_status", [] { return static_cast<int64_t>(imu_status); }),       // 0x2401 = 9217: found and running
    text_float("ax", [] { return imu_data.acc[0] * bmi270_g_per_count; }),
    text_float("ay", [] { return imu_data.acc[1] * bmi270_g_per_count; }),
    text_float("az", [] { return imu_data.acc[2] * bmi270_g_per_count; }),
    text_float("gx", [] { return imu_data.gyr[0] * bmi270_rad_s_per_count; }),
    text_float("gy", [] { return imu_data.gyr[1] * bmi270_rad_s_per_count; }),
    text_float("gz", [] { return imu_data.gyr[2] * bmi270_rad_s_per_count; }),
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

    // the host's time, see control/clock.cppm and the time and uptime requests
    text_int("time_syncs", [] { return static_cast<int64_t>(SystemClock::count()); }),         // times the host has set the system clock
    text_int("time_step", [] { return static_cast<int64_t>(SystemClock::last_step_ns()); }),   // ns that the last of them moved it by
    text_int("time_rate", [] { return static_cast<int64_t>(SystemClock::rate_ppb()); }),       // ppb that the host's time gains on this node's
    text_int("time_dropped", [] { return static_cast<int64_t>(TimeSync::dropped() + time_frames_late); }),     // time frames that set nothing
    text_int("debug_probe", [] { return static_cast<int64_t>(debug_probe_attached()); }),    // 1: no step or breakpoints for gdb
    text_int("breakpoints_dropped", [] { return static_cast<int64_t>(debug.breakpoints_dropped); }),   // set above the debug monitor
};
#undef TUNE
#undef FAST_STATUS

struct TextMemory {
    static uint32_t read(uint32_t address, uint8_t* data, uint32_t length) { return memory_read(address, data, length); }
    static uint32_t write(uint32_t address, const uint8_t* data, uint32_t length) { return memory_write(address, data, length); }
};

// cpu: what every interrupt handler and every task of the background took
// since the last cpu request, a line each:
//   name  percent of the cpu  runs  cycles min/mean/max  period min/max
// in cpu cycles, net of what interrupted it. The rest is the background
// going around the scheduler.
struct StatsLock {
    StatsLock() { __asm__ volatile("mrs %0, primask\n cpsid i" : "=r"(state)); }
    ~StatsLock() { __asm__ volatile("msr primask, %0" : : "r"(state)); }
    uint32_t state;
};
DEBUG_SMALL void cpu_request(TextOut& out) {
    static uint64_t last_uptime;
    const uint64_t uptime = uptime_cycles + (get_clock() - uptime_clock);
    const uint64_t elapsed = uptime - last_uptime;
    last_uptime = uptime;
    const auto tenths = [&](uint64_t cycles) {
        const uint32_t value = elapsed == 0 ? 0 : static_cast<uint32_t>(cycles * 1000 / elapsed);
        out.put_number(static_cast<int64_t>(value / 10));
        out.put('.');
        out.put(static_cast<char>('0' + value % 10));
    };
    const auto number = [&](uint64_t value) {
        out.put(' ');
        out.put_number(static_cast<int64_t>(value));
    };
    uint64_t total = 0;
    for (CycleStats* stats : cycle_stats) {
        const CycleStats s = cycle_stats_take<StatsLock>(*stats);
        total += s.sum;
        out.put(s.name);
        out.put(' ');
        tenths(s.sum);
        out.put('%');
        number(s.count);
        if (s.count != 0) {
            number(s.min);
            number(s.sum / s.count);
            number(s.max);
            if (s.period_max != 0) {
                number(s.period_min);
                number(s.period_max);
            }
        }
        out.put('\n');
    }
    out.put("total ");
    tenths(total);
    out.put("% of ");
    out.put_number(static_cast<int64_t>(elapsed / (CPU_FREQUENCY_HZ / 1000)));
    out.put(" ms");
}

DEBUG_SMALL void put_time(TextOut& out, ClockTime time) {
    out.put_number(static_cast<int64_t>(time.seconds));
    char fraction[11] = ".";
    for (int i = 9; i >= 1; i--) {
        fraction[i] = static_cast<char>('0' + time.nanoseconds % 10);
        time.nanoseconds /= 10;
    }
    out.put(fraction);
}

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
    } else if (std::strcmp(request, "cpu") == 0) {
        cpu_request(out);
    } else if (std::strcmp(request, "time") == 0) {
        put_time(out, SystemClock::time());
    } else if (std::strcmp(request, "uptime") == 0) {
        put_time(out, SteadyClock::time());
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
    const StatsTimer timer(text_isr_stats);
    debug.sample = frame;
    can_interrupt_flags<tmotor_can_init>();
    uint32_t id;
    char data[65];
    for (int length; (length = can_read<tmotor_can_init>(tmotor_can_ascii_fifo, id, data)) >= 0;) {
        data[length] = 0;
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
//   time             to all nodes: the host's time, which sets the system clock
// and on the command id the 8 byte BOOTLOADER_ENTER request of
// bootloader_protocol.h, which is answered and ends in the bootloader.

// When a frame started, in cpu cycles: its count of TIM3 against the count
// now, with nothing in between the two that are read here
inline uint32_t can_frame_cycles(uint16_t timestamp) {
    uint32_t state;
    __asm__ volatile("mrs %0, primask\n cpsid i" : "=r"(state));
    const uint16_t now = can_timestamp<tmotor_can_init>();
    const uint32_t cycles = DWT->CYCCNT;
    __asm__ volatile("msr primask, %0" : : "r"(state));
    return cycles - static_cast<uint16_t>(now - timestamp) * tmotor_can_timestamp_cycles;
}

void host_poll() {
    // A frame that is read here has come in since the fifo was last seen
    // empty. Its count tells when, as long as that is less than the count's
    // way around ago: not when gdb had the background stopped in between.
    static uint32_t last_empty;
    uint32_t id;
    char data[64];
    uint16_t timestamp;
    const int length = can_read<tmotor_can_init>(tmotor_can_command_fifo, id, data, &timestamp);
    if (length < 0) {
        last_empty = DWT->CYCCNT;
        return;
    }
    const uint32_t message = id >> 7;
    if (message == obot_can_time) {
        if (DWT->CYCCNT - last_empty < 0x8000 * tmotor_can_timestamp_cycles) {
            TimeSync::frame(reinterpret_cast<const uint8_t*>(data), length, SteadyClock::time_at(can_frame_cycles(timestamp)));
        } else {
            time_frames_late = time_frames_late + 1;
        }
    } else if (message == obot_can_enum) {
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

// The imu is brought up here, with the node already answering, and then read
// at 200 Hz. Each of its frames is a wait for the bus and for the dma, see
// bmi270.h and spi_bus.h. Without an imu the task ends.
GlobalTask<void, 2, 160> imu_task(CycleScheduler& sched) {
    constexpr uint32_t period_cycles = CPU_FREQUENCY_HZ / 200;
    co_await sched.async_delay_us(1000);    // first: the task starts before pin_config
    imu_status = co_await bmi270_init<TMotorIMUIO>(sched);
    if (imu_status != (bmi270_chip_id << 8 | bmi270_init_ok)) co_return;
    uint32_t wake_time = get_clock();
    while (true) {
        wake_time += period_cycles;
        if (static_cast<int32_t>(get_clock() - wake_time) > 0) wake_time = get_clock();     // gdb had it stopped
        co_await sched.delay_until(wake_time);
        const BMI270Data data = co_await bmi270_data<TMotorIMUIO>(sched);
        const StatsTimer timer(imu_task_stats);
        const TextLock lock;        // the text api reads it from its interrupt while gdb has the node stopped
        imu_data = data;
    }
}

// The led as on the obot_g474 (motorlib/main_loop.h, led.h): the color is the
// mode, breathing once a second, and red at twice that on a fault. One step
// of 200 a second.
void led_update() {
    static uint32_t phase;
    struct Color { uint8_t red, green, blue; };     // in halves
    constexpr Color red{2, 0, 0}, orange{2, 1, 0}, yellow{2, 2, 0}, chartreuse{1, 2, 0}, green{0, 2, 0},
        azure{0, 1, 2}, blue{0, 0, 2}, violet{1, 0, 2}, magenta{2, 0, 2}, rose{2, 0, 1};
    Color color = azure;
    uint32_t rate = 1;
    switch (MySystem::main_loop.controller.status.read().flags.mode) {
        case obot::DAMPED: color = orange; break;
        case obot::CURRENT: color = green; break;
        case obot::POSITION:
        case obot::VELOCITY: color = blue; break;
        case obot::IMPEDANCE: color = chartreuse; break;
        case obot::TORQUE: color = rose; break;
        case obot::STATE: color = magenta; break;
        case obot::VOLTAGE: color = violet; break;
        case obot::PHASE_LOCK: color = yellow; break;
        case obot::FAULT: color = red; rate = 2; break;
        default: break;     // open
    }
    phase = (phase + rate) % 200;
    const float t = phase * (1.f / 200);
    const float triangle = t < .5f ? 2 * t : 2 - 2 * t;
    const uint32_t intensity = static_cast<uint32_t>(32767 * triangle * triangle * triangle);     // of half scale
    tmotor_led(color.red * intensity, color.green * intensity, color.blue * intensity);
}

GlobalTask<void, 3, 96> led_task(CycleScheduler& sched) {
    constexpr uint32_t period_cycles = CPU_FREQUENCY_HZ / 200;
    uint32_t wake_time = get_clock();
    while (true) {
        wake_time += period_cycles;
        if (static_cast<int32_t>(get_clock() - wake_time) > 0) wake_time = get_clock();
        co_await sched.delay_until(wake_time);      // first: the task starts before pin_config
        const StatsTimer timer(led_task_stats);
        led_update();
    }
}

// The host's frames, every pass of the background: what came in is served
// between two yields
GlobalTask<void, 4, 144> text_task(CycleScheduler& sched) {
    while (true) {
        co_await sched.yield();
        const StatsTimer timer(text_task_stats);
        text_poll();
    }
}
GlobalTask<void, 5, 96> host_task(CycleScheduler& sched) {
    while (true) {
        co_await sched.yield();
        const StatsTimer timer(host_task_stats);
        host_poll();
    }
}

#if OBOT3_TEXT_API
// The functions that gdb calls, thread 3: one of the tasks, each on a pass of
// the loop until it returns or yields (debug_monitor.h). One that waits for
// the spi bus or for a coroutine does so here, with the other tasks running.
GlobalTask<void, 6, 96> debug_call_task(CycleScheduler& sched) {
    while (true) {
        co_await sched.yield();
        const StatsTimer timer(call_task_stats);
        debug_call_poll();
    }
}
auto debug_call_task_instance = debug_call_task(sched);

// A register of the imu, for gdb: `thread apply 3 print/x tmotor_imu_get(0)`
// is the chip id. The frame is a task's like the imu task's own, between them.
DEBUG_CALL uint32_t tmotor_imu_get(uint32_t address) {
    return debug_call_wait(bmi270_get<TMotorIMUIO>(sched, address));
}
#endif

// The background is the scheduler: status_task above and these
auto imu_task_instance = imu_task(sched);
auto led_task_instance = led_task(sched);
auto text_task_instance = text_task(sched);
auto host_task_instance = host_task(sched);

void os_loop() {
    sched.poll();
    if (watchdog_ok) watchdog_kick();
}
