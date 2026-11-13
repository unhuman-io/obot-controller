#include <atomic>
#include <cstring>
#include "../motorlib/task.h"   // device header first: the device.* modules carry it in their global module fragment
#include "../motorlib/motor_messages/motor_messages.h"
#include "system.h"
#include "basic_config.h"
import tmotor_driver;

template<auto config = TMotorConfig{}>
struct Config : BasicConfig<config> {};

using MySystem = System<Config<>>;

// ADC5 end of injected sequence, 50 kHz
void fast_loop_isr() {
    std::atomic_signal_fence(std::memory_order_acq_rel);
    MySystem::fast_loop.update();
    tmotor_fast_loop_irq_clear();
}

// HRTIM master cmp1, 10 kHz
void main_loop_isr() {
    std::atomic_signal_fence(std::memory_order_acq_rel);
    MySystem::main_loop.update();
    tmotor_main_loop_irq_clear();
}

// main.cpp polls this from the background loop. Here the main loop has its
// own interrupt.
void tim2_isr() {}

// The fast loop preempts the main loop
constexpr auto tmotor_irq_init = []() consteval {
    IRQInit init{};
    init.irq[ADC5_IRQn] = {.handler = fast_loop_isr, .priority = 0};
    init.irq[HRTIM1_Master_IRQn] = {.handler = main_loop_isr, .priority = 1};
    return init;
}();

extern "C" void Reset_Handler();
__attribute__((section(".isr_vector"), used))
constinit const auto vector_table = get_vector_table<tmotor_irq_init>(&_estack, Reset_Handler);

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

void pin_config() {
    device_rcc_config<tmotor_rcc_init>();
    device_pin_config<tmotor_gpio_init>();
    tmotor_startup_init();
    tmotor_startup();
    tmotor_device_init();
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

// The obot text api over can, as far as a host needs it to find this node and
// to get it back into the bootloader, plus a few readings. Requests are
// strings to the ascii id of this address; each gets one reply, empty for an
// unknown name.
struct TextValue {
    const char* name;
    int32_t (*get)();
};
constexpr TextValue text_values[] = {
    {"fast_loop_count", [] { return static_cast<int32_t>(MySystem::fast_loop.count); }},
    {"main_loop_count", [] { return static_cast<int32_t>(MySystem::main_loop.sensors.count); }},
    {"mode", [] { return static_cast<int32_t>(MySystem::main_loop.controller.status.read().flags.mode); }},
    {"menc", [] { return MySystem::main_loop.controller.status.read().motor_encoder; }},
    {"ia", [] { return static_cast<int32_t>(tmotor_ia()); }},        // adc counts
    {"ib", [] { return static_cast<int32_t>(tmotor_ib()); }},
    {"ic", [] { return static_cast<int32_t>(tmotor_ic()); }},
    {"vbus", [] { return static_cast<int32_t>(tmotor_vbus()); }},
    {"vbus_mv", [] { return static_cast<int32_t>(MySystem::fast_loop.get_status().measured.vbus * 1000); }},
    {"vq_mv", [] { return static_cast<int32_t>(MySystem::fast_loop.get_status().controller.v_q * 1000); }},
    {"iq_ma", [] { return static_cast<int32_t>(MySystem::fast_loop.get_status().controller.i_q * 1000); }},
    {"iq_desired_ma", [] { return static_cast<int32_t>(MySystem::fast_loop.get_status().controller.i_q_desired * 1000); }},
    {"drv_errors", [] { return static_cast<int32_t>(tmotor_drv8323_errors); }},     // 0: configured
    {"drv_fault", [] { return static_cast<int32_t>(tmotor_drv8323_fault()); }},     // the fault pin
    {"drv_status", [] { return static_cast<int32_t>(tmotor_drv8323_status()); }},   // status 2 << 16 | status 1
    {"invalid_command_count", [] { return static_cast<int32_t>(MySystem::main_loop.controller.invalid_command_count); }},
    {"temperature", [] { return static_cast<int32_t>(temperature_reading); }},
    {"vrefint", [] { return static_cast<int32_t>(vrefint_reading); }},
};

// A string of two or more characters goes out with its terminator, as the
// obot_g474 does
void text_reply(const char* string) {
    uint32_t length = std::strlen(string);
    if (length > 1) length++;
    can_write<tmotor_can_init>(obot_can_id(obot_can_ascii_response, tmotor_can_address()), string, length);
}

void text_reply(int32_t value) {
    char buffer[12];
    char* p = buffer + sizeof(buffer);
    uint32_t magnitude = value < 0 ? -static_cast<uint32_t>(value) : value;
    *--p = 0;
    do {
        *--p = '0' + magnitude % 10;
        magnitude /= 10;
    } while (magnitude != 0);
    if (value < 0) *--p = '-';
    text_reply(p);
}

void text_request(const char* request) {
    if (std::strcmp(request, "name") == 0) {
        text_reply("obot3_tmotor_driver");
    } else if (std::strcmp(request, "serial") == 0) {
        char serial[13];
        system_serial_number(serial);
        text_reply(serial);
    } else if (std::strcmp(request, "messages_version") == 0) {
        text_reply(MOTOR_MESSAGES_VERSION);
    } else if (std::strcmp(request, "version") == 0) {
        text_reply(OBOT3_VERSION);
    } else if (std::strcmp(request, "go_to_bootloader=b007") == 0) {
        bootloader_request = true;      // for the next reset
        text_reply("b007");
    } else if (std::strcmp(request, "reset") == 0) {
        text_reply("");
        reset_request = true;
    } else {
        for (const TextValue& value : text_values) {
            if (std::strcmp(request, value.name) == 0) {
                text_reply(value.get());
                return;
            }
        }
        text_reply("");
    }
}

void host_poll() {
    uint32_t id;
    char data[65];
    if (can_read<tmotor_can_init>(tmotor_can_ascii_fifo, id, data) >= 0) {
        data[64] = 0;
        host_idle_ticks = 0;
        text_request(data);
    }
    // commands, as the obot_g474: a command frame is the new MotorCommand, a
    // command status frame is that and a request for the status, a status
    // frame only the request
    if (const int length = can_read<tmotor_can_init>(tmotor_can_command_fifo, id, data); length >= 0) {
        host_idle_ticks = 0;
        const uint32_t message = id >> 7;
        if (message == obot_can_enum) {
            can_write<tmotor_can_init>(obot_can_id(obot_can_enum, tmotor_can_address()), data, 0);   // here, with no data
        } else {
            if (message != obot_can_status && length >= static_cast<int>(sizeof(obot::MotorCommand))) {
                obot::MotorCommand command;
                std::memcpy(&command, data, sizeof(command));
                TMotorHostCommand::latest.write(command);
            }
            if (message != obot_can_command) {
                can_write<tmotor_can_init>(obot_can_id(obot_can_status, tmotor_can_address()),
                    MySystem::main_loop.controller.status.read());
            }
        }
    }
}

void os_loop() {
    host_poll();
    sched.poll();
    if (watchdog_ok) watchdog_kick();
}
