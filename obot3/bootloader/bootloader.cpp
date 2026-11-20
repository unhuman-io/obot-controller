#include <cstddef>
#include <cstdint>
#include <cstring>
#include <initializer_list>
#include "st_device.h"      // device header first: the device.* modules carry it in their global module fragment
#include "../motorlib/motor_messages/motor_messages.h"
#include "bootloader_protocol.h"
#include "obot_enum.h"
import device.rcc_init;
import device.gpio_init;
import device.can_init;
import device.irq_init;
import device.flash;
import device.crc_init;
import device.system;

// The can bootloader of the obot_g474 motor boards (the tmotor driver among
// them), in the first 8K of the flash. It is the obot3 counterpart of
// motorlib/bootloader/stm32g4.
//
// Out of reset it starts the watchdog and hands over to the application at
// 0x08002000, before it sets up anything, unless
//   - the watchdog caused the reset: an application that stops ends up here
//   - the application asked for it: a software reset with 0xB007 in the first
//     word of the ram (system_reset_to_bootloader())
//   - there is no application: its vector table has no stack in the ram or no
//     entry point in the flash, which is what an erased or half loaded flash
//     looks like
// Then it is the node "bootloader" at the can address in the first byte of the
// parameters (0x08060000): it answers the obot enumeration and the text api
// (name, serial, version, messages_version, reset) like an application, and
// loads flash with the protocol in bootloader_protocol.h. It leaves through a
// reset only, so the application always starts from the reset state.

extern unsigned int _estack, _sidata, _sdata, _edata, __bss_start__, __bss_end__;
extern "C" void Reset_Handler();
[[noreturn]] void bootloader();

constexpr IRQInit irq_init{};   // no interrupts
__attribute__((section(".isr_vector"), used))
constinit const auto vector_table = get_vector_table<irq_init>(&_estack, Reset_Handler);

constexpr uint32_t ram_base = 0x20000000, ram_end = 0x20020000;     // sram1, sram2 and the ccm sram behind them

// Nothing here may depend on .data or .bss: they are the application's until
// bootloader() is chosen
extern "C" void Reset_Handler() {
    watchdog_start();
    const uint32_t reset_flags = RCC->CSR;
    RCC->CSR = reset_flags | RCC_CSR_RMVF;
    volatile uint32_t& flag = bootloader_flag();
    const bool requested = (reset_flags & RCC_CSR_SFTRSTF) && flag == bootloader_flag_value;
    flag = 0;
    const uint32_t* application = reinterpret_cast<const uint32_t*>(application_address);
    const uint32_t stack = application[0], entry = application[1];
    const bool application_valid = stack > ram_base && stack <= ram_end && (stack & 3) == 0 &&
        entry >= application_address && entry < flash_end() && (entry & 1) == 1;
    if (application_valid && !requested && !(reset_flags & (RCC_CSR_IWDGRSTF | RCC_CSR_WWDGRSTF))) {
        __asm__ volatile("msr msp, %0\nbx %1" : : "r"(stack), "r"(entry));
    }

    unsigned int* source = &_sidata;
    for (unsigned int* p = &_sdata; p < &_edata;) *p++ = *source++;
    for (unsigned int* p = &__bss_start__; p < &__bss_end__;) *p++ = 0;
    bootloader();
}

constexpr auto rcc_clock_init = RCCClockInit{.fdcan_clock = RCCFDCANClock::PCLK1};    // 170 MHz from the 24 MHz crystal
constexpr auto rcc_init = RCCInit{
    .crc = RCCEnable::ENABLED,
    .gpiob = RCCEnable::ENABLED,
    .gpioa = RCCEnable::ENABLED,
    .fdcan = RCCEnable::ENABLED};

constexpr auto gpio_init = []() consteval {
    GPIOInit init = get_default_gpio_init();
    for (auto* pin : {&init.a[8], &init.b[4]}) {
        pin->mode = GPIOMode::ALTERNATE;
        pin->speed = GPIOSpeed::VERY_HIGH;
        pin->af = GPIOAF::AF11;
    }
    init.a[8].function = "fdcan3 rx";
    init.b[4].function = "fdcan3 tx";
    init.b[8].mode = GPIOMode::OUTPUT;
    init.b[8].function = "led b";
    return init;
}();

constexpr auto can_init = CANInit{
    .instance = 2,                              // FDCAN3
    .kernel_clock_hz = rcc_clock_init.cpu_frequency_hz,
    .nominal_bitrate_hz = 1'000'000,
    .data_bitrate_hz = 8'000'000,
    .standard_filter_count = 4,
};
constexpr int request_fifo = 0, text_fifo = 1;

// obot can ids: message << 7 | address
constexpr uint32_t obot_can_command = 1, obot_can_status = 3, obot_can_ascii = 4, obot_can_ascii_response = 5, obot_can_enum = 0xF;
constexpr uint32_t obot_can_broadcast_address = 0x7F;
constexpr uint32_t obot_can_id(uint32_t message, uint32_t address) { return message << 7 | address; }
constexpr uint32_t param_address = 0x08060000;

uint32_t can_address;       // read once: the parameters can be loaded from here
bool reset_request;

// [address, address + length) inside [low, high)
bool in_range(uint32_t address, uint32_t length, uint32_t low, uint32_t high) {
    return address >= low && address <= high && length <= high - address;
}

bool blank(uint32_t address, uint32_t length) {
    const uint32_t* p = reinterpret_cast<const uint32_t*>(address);
    for (uint32_t i = 0; i < length / 4; i++) {
        if (p[i] != 0xFFFFFFFF) return false;
    }
    return true;
}

// One request, one reply. The application's flash is everything behind the
// bootloader: only that can be erased and written.
void load_request(const BootloaderRequest& request, BootloaderReply& reply) {
    const uint32_t address = request.address;
    switch (request.command) {
        case BOOTLOADER_INFO: {
            const BootloaderInfo info = {application_address, flash_end(), flash_page_size(), sizeof(request.data)};
            reply.value = BOOTLOADER_PROTOCOL_VERSION;
            reply.length = sizeof(info);
            std::memcpy(reply.data, &info, sizeof(info));
            break;
        }
        case BOOTLOADER_ERASE: {
            uint32_t length;
            std::memcpy(&length, request.data, sizeof(length));
            if (!in_range(address, length, application_address, flash_end())) {
                reply.status = BOOTLOADER_BAD_RANGE;
                break;
            }
            const uint32_t page_size = flash_page_size();
            for (uint32_t page = address & ~(page_size - 1); page < address + length; page += page_size) {
                watchdog_kick();
                if (blank(page, page_size)) continue;       // 20 us instead of 22 ms
                if (const uint32_t errors = flash_erase_page(page); errors != 0) {
                    reply.status = BOOTLOADER_FLASH_ERROR;
                    reply.value = errors;
                    break;
                }
            }
            break;
        }
        case BOOTLOADER_WRITE: {
            const uint32_t length = request.length;
            if (length > sizeof(request.data) || (address & 7) != 0 || !in_range(address, length, application_address, flash_end())) {
                reply.status = BOOTLOADER_BAD_RANGE;
                break;
            }
            // A request that is repeated because its reply was lost finds its data there
            if (std::memcmp(reinterpret_cast<const void*>(address), request.data, length) == 0) break;
            uint32_t words[sizeof(request.data) / 4];
            std::memset(words, 0xFF, sizeof(words));    // up to the next double word
            std::memcpy(words, request.data, length);
            for (uint32_t i = 0; i < (length + 7) / 8; i++) {
                if (const uint32_t errors = flash_program(address + 8 * i, words[2 * i], words[2 * i + 1]); errors != 0) {
                    reply.status = BOOTLOADER_FLASH_ERROR;
                    reply.value = errors;
                    return;
                }
            }
            if (std::memcmp(reinterpret_cast<const void*>(address), request.data, length) != 0) reply.status = BOOTLOADER_VERIFY_ERROR;
            break;
        }
        case BOOTLOADER_READ:
            if (request.length > sizeof(reply.data) || !in_range(address, request.length, flash_base, flash_end())) {
                reply.status = BOOTLOADER_BAD_RANGE;
                break;
            }
            reply.length = request.length;
            std::memcpy(reply.data, reinterpret_cast<const void*>(address), request.length);
            break;
        case BOOTLOADER_CRC: {
            uint32_t length;
            std::memcpy(&length, request.data, sizeof(length));
            if (!in_range(address, length, flash_base, flash_end())) {
                reply.status = BOOTLOADER_BAD_RANGE;
                break;
            }
            reply.value = crc_compute<crc32_init>(reinterpret_cast<const void*>(address), length);
            break;
        }
        case BOOTLOADER_RESET:
            reset_request = true;
            break;
        case BOOTLOADER_ENTER:      // it is in
            break;
        default:
            reply.status = BOOTLOADER_BAD_COMMAND;
            break;
    }
}

// The MotorCommand of an application with BOARD_RESET as its mode, which is
// what `motor_util set --mode reset` sends: the way out of the bootloader
// without the text api, as it is the reset of an application. A MotorCommand
// starts with the host's time stamp where a request has its command, so one
// whose first byte is a command of bootloader_protocol.h is taken as that.
bool motor_command_reset(const BootloaderRequest& request, int length) {
    const bool bootloader_command = request.command >= BOOTLOADER_INFO && request.command <= BOOTLOADER_ENTER;
    const uint8_t* bytes = reinterpret_cast<const uint8_t*>(&request);
    return !bootloader_command && length >= static_cast<int>(sizeof(obot::MotorCommand)) &&
        bytes[offsetof(obot::MotorCommand, mode_desired)] == obot::BOARD_RESET;
}

// A string of two or more characters goes out with its terminator, as the
// applications do
void text_reply(const char* string) {
    uint32_t length = std::strlen(string);
    if (length > 1) length++;
    can_write<can_init>(obot_can_id(obot_can_ascii_response, can_address), string, length);
}

void text_request(const char* request) {
    if (std::strcmp(request, "name") == 0) {
        text_reply("bootloader");
    } else if (std::strcmp(request, "serial") == 0) {
        char serial[13];
        system_serial_number(serial);
        text_reply(serial);
    } else if (std::strcmp(request, "messages_version") == 0) {
        text_reply(MOTOR_MESSAGES_VERSION);
    } else if (std::strcmp(request, "version") == 0) {
        text_reply(OBOT3_VERSION);
    } else if (std::strcmp(request, "reset") == 0) {
        text_reply("");
        reset_request = true;
    } else {
        text_reply("");
    }
}

[[noreturn]] void bootloader() {
    device_rcc_clock_config<rcc_clock_init>();
    device_rcc_config<rcc_init>();
    device_pin_config<gpio_init>();
    can_address = *reinterpret_cast<const uint8_t*>(param_address) & 0x7F;
    device_can_config<can_init>();
    can_filter<can_init>(0, obot_can_id(obot_can_command, can_address), request_fifo);
    can_filter<can_init>(1, obot_can_id(obot_can_enum, obot_can_broadcast_address), request_fifo);
    can_filter<can_init>(2, obot_can_id(obot_can_ascii, can_address), text_fifo);
    can_filter<can_init>(3, obot_can_id(obot_can_enum, can_address), request_fifo);
    device_crc_config<crc32_init>();
    flash_unlock();

    constexpr uint32_t reset_timeout_cycles = rcc_clock_init.cpu_frequency_hz / 100;    // 10 ms for the last reply to go out
    uint32_t reset_time = 0;
    while (true) {
        watchdog_kick();
        const uint32_t now = DWT->CYCCNT;
        if (now & 1u << 26) gpio_set(gpio_port<'B'>(), 8);      // blue led, 1.3 Hz
        else gpio_clear(gpio_port<'B'>(), 8);

        uint32_t id;
        union {
            BootloaderRequest request;
            char text[65];
        } frame;
        if (const int length = can_read<can_init>(request_fifo, id, &frame); length >= 0) {
            if (id >> 7 == obot_can_enum) {
                uint8_t serial[6];
                system_serial_bytes(serial);
                const obot::EnumResponse response = obot_enum_response("bootloader", serial);
                // to all nodes: an empty reply, that this one is here
                const bool to_all = (id & 0x7F) == obot_can_broadcast_address;
                can_write<can_init>(obot_can_id(obot_can_enum, can_address), &response, to_all ? 0 : sizeof(response));
            } else if (motor_command_reset(frame.request, length)) {
                reset_request = true;       // no reply, as from an application
            } else {
                BootloaderReply reply{};
                reply.command = frame.request.command | BOOTLOADER_REPLY;
                reply.sequence = frame.request.sequence;
                load_request(frame.request, reply);
                can_write<can_init>(obot_can_id(obot_can_status, can_address), &reply, offsetof(BootloaderReply, data) + reply.length);
            }
        }
        if (can_read<can_init>(text_fifo, id, &frame) >= 0) {
            frame.text[64] = 0;
            text_request(frame.text);
        }

        if (!reset_request) {
            reset_time = now;
        } else if (can_tx_idle<can_init>() || now - reset_time > reset_timeout_cycles) {
            flash_lock();
            system_reset();
        }
    }
}
