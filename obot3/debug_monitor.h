#pragma once
#include <cstdint>
#include <cstring>
#include "text_api.h"

// Pause, step and breakpoints for gdb without a debug probe, on a Cortex-M4:
// the core's debug monitor exception, driven by the gdb remote protocol
// packets that come in over the text api (text_api.h, `motor_util gdbserver`).
// Include the device header first.
//
// The monitor is an exception with a priority of its own. What is more urgent
// keeps running while the node is stopped, and that has to include the
// interrupt that takes the text requests in; what is less urgent is what gdb
// stops, steps and sees:
//
//     fast loop, text api interrupt       run on, breakpoints there are ignored
//     debug monitor
//     thread 2..: interrupts, as listed   stopped
//     thread 1: the background            stopped
//
// gdb gets these as threads. One that is not active when the node stops is
// shown at the entry of its handler, about to interrupt what runs. A step is
// one instruction of the thread that runs; for its duration the interrupts of
// the other threads are as urgent as the text api, so they run as they would
// and the step never ends up in them.
//
// The node is not stopped by gdb connecting: until the first continue the
// registers are those at the moment of the request, and memory is live.
//
// Hardware breakpoints (6) and watchpoints (4), also for gdb's software
// breakpoints: the code is in flash. A breakpoint in code that runs above the
// monitor's priority can't stop there: it is a hard fault, which takes the
// breakpoint out and goes on (debug.breakpoints_dropped counts them). A
// watchpoint that is hit there is not noticed.
//
// None of this works while a debug probe has halting debug enabled
// (DHCSR C_DEBUGEN, which only a power cycle or the probe clears): the events
// then halt the core for the probe instead. debug_probe_attached() tells.

// What an entry (DEBUG_FRAME_ENTRY, the monitor's) has on the stack: r4 to r11
// and the exception return value, pushed by it, below what the cpu stacked
struct DebugFrame {
    uint32_t pad, r4, r5, r6, r7, r8, r9, r10, r11, exc_return;
    uint32_t r0, r1, r2, r3, r12, lr, pc, xpsr;
    uint32_t s[16], fpscr, reserved;                            // there if extended()
    bool extended() const { return (exc_return & 0x10) == 0; }
    uint32_t exception() const { return xpsr & 0x1FF; }         // 0: it was the background
    uint32_t sp() const {                                       // of what was interrupted
        return reinterpret_cast<uint32_t>(&r0) + (extended() ? 0x68 : 0x20) + (xpsr & 0x200 ? 4 : 0);
    }
};

// Where a debugger finds the caller of an entry, cfa_offset bytes of stack in
#define DEBUG_FRAME_CFI(cfa_offset) \
    ".cfi_def_cfa_offset " #cfa_offset "\n .cfi_offset 4, -36\n .cfi_offset 5, -32\n .cfi_offset 6, -28\n .cfi_offset 7, -24\n" \
    ".cfi_offset 8, -20\n .cfi_offset 9, -16\n .cfi_offset 10, -12\n .cfi_offset 11, -8\n .cfi_offset 14, -4\n"

// An interrupt handler `name` that calls `extern "C" void body(DebugFrame*)`
// with the context it interrupted
#define DEBUG_FRAME_ENTRY(name, body) \
    [[gnu::naked]] void name() { __asm__("push {r3-r11, lr}\n" DEBUG_FRAME_CFI(40) "mov r0, sp\n bl " #body "\n pop {r3-r11, pc}"); }

// A handler `name` for the interrupt of thread number `thread` (2..), which
// runs function()
#define DEBUG_THREAD_ENTRY(name, thread, function) \
    extern "C" [[gnu::used]] void name##_thread(DebugFrame* frame) { \
        debug.thread_frame[thread - 1] = frame; \
        function(); \
    } \
    DEBUG_FRAME_ENTRY(name, name##_thread)

struct DebugThread {
    const char* name;
    int irq;                // the interrupt line. Not used for thread 1, the background
    void (*entry)();        // its handler in the vector table
    uint8_t priority;       // as in the IRQInit
};

inline constexpr uint32_t debug_thread_max = 4;

struct DebugMonitor {
    const DebugThread* threads;             // [0] is thread 1
    uint32_t thread_count;
    uint8_t text_priority;                  // of the interrupt that calls debug_request
    void (*idle)();                         // called over and over while stopped
    DebugFrame* thread_frame[debug_thread_max];     // what the thread's handler interrupted
    DebugFrame* volatile frame;             // what the monitor interrupted, while stopped
    DebugFrame* sample;                     // what the text api interrupt interrupted
    volatile bool stopped, resume, step, raised;
    volatile uint8_t pause_signal;
    uint8_t signal;                         // why it stopped: 2 on request, else 5
    uint8_t watch_function;                 // if it was a watchpoint
    uint32_t watch_address;
    uint32_t general_thread;                // the one gdb selected for the registers, 0: the one that runs
    uint32_t breakpoints_dropped;           // hit where the monitor can't run
};
inline constinit DebugMonitor debug{};

namespace debug_reg {
inline constexpr uint32_t DEMCR_MON_EN = 1u << 16, DEMCR_MON_PEND = 1u << 17, DEMCR_MON_STEP = 1u << 18, DEMCR_TRCENA = 1u << 24;
inline constexpr uint32_t DFSR_HALTED = 1u << 0, DFSR_BKPT = 1u << 1, DFSR_DWTTRAP = 1u << 2;
inline volatile uint32_t& fp_ctrl() { return *reinterpret_cast<volatile uint32_t*>(0xE0002000); }
inline volatile uint32_t& fp_comp(uint32_t n) { return reinterpret_cast<volatile uint32_t*>(0xE0002008)[n]; }
inline constexpr uint32_t FP_CTRL_ENABLE = 1u << 0, FP_CTRL_KEY = 1u << 1;
inline volatile uint32_t& dwt_comp(uint32_t n) { return (&DWT->COMP0)[4 * n]; }
inline volatile uint32_t& dwt_mask(uint32_t n) { return (&DWT->MASK0)[4 * n]; }
inline volatile uint32_t& dwt_function(uint32_t n) { return (&DWT->FUNCTION0)[4 * n]; }
inline constexpr uint32_t DWT_FUNCTION_MATCHED = 1u << 24;
inline uint32_t breakpoint_count() { return (fp_ctrl() >> 4) & 0xF; }
inline uint32_t watchpoint_count() { return DWT->CTRL >> 28; }
}

inline bool debug_probe_attached() {
    return (CoreDebug->DHCSR & CoreDebug_DHCSR_C_DEBUGEN_Msk) != 0;
}

inline void debug_breakpoints_clear() {
    using namespace debug_reg;
    for (uint32_t n = 0; n < breakpoint_count(); n++) fp_comp(n) = 0;
    for (uint32_t n = 0; n < watchpoint_count(); n++) dwt_function(n) = 0;
}

// threads[0] names the background, the others are interrupts less urgent than
// monitor_priority. text_priority is that of the interrupt that calls
// debug_request(), more urgent than the monitor.
inline void debug_monitor_init(const DebugThread* threads, uint32_t thread_count, uint8_t monitor_priority,
                               uint8_t text_priority, void (*idle)()) {
    debug.threads = threads;
    debug.thread_count = thread_count < debug_thread_max ? thread_count : debug_thread_max;
    debug.text_priority = text_priority;
    debug.idle = idle;
    NVIC_SetPriority(DebugMonitor_IRQn, monitor_priority);
    CoreDebug->DEMCR = CoreDebug->DEMCR | debug_reg::DEMCR_MON_EN | debug_reg::DEMCR_TRCENA;
    debug_breakpoints_clear();
    debug_reg::fp_ctrl() = debug_reg::FP_CTRL_KEY | debug_reg::FP_CTRL_ENABLE;
}

// Thread number of what runs as exception number `exception`
inline uint32_t debug_thread_of(uint32_t exception) {
    for (uint32_t i = 1; i < debug.thread_count; i++) {
        if (exception == static_cast<uint32_t>(debug.threads[i].irq) + 16) return i + 1;
    }
    return 1;
}

inline void debug_thread_priorities(bool raise, uint32_t except_thread) {
    for (uint32_t i = 1; i < debug.thread_count; i++) {
        const DebugThread& thread = debug.threads[i];
        if (raise && i + 1 == except_thread) continue;
        NVIC_SetPriority(static_cast<IRQn_Type>(thread.irq), raise ? debug.text_priority : thread.priority);
    }
    debug.raised = raise;
}

// The monitor exception: here the node is stopped, until a request says to go on
extern "C" [[gnu::used]] inline void debug_monitor(DebugFrame* frame) {
    using namespace debug_reg;
    if (debug.raised) debug_thread_priorities(false, 0);
    CoreDebug->DEMCR = CoreDebug->DEMCR & ~DEMCR_MON_STEP;
    const uint32_t reason = SCB->DFSR;
    SCB->DFSR = reason;
    debug.signal = (reason & (DFSR_HALTED | DFSR_BKPT | DFSR_DWTTRAP)) != 0 ? 5 : debug.pause_signal;
    debug.pause_signal = 5;
    debug.watch_function = 0;
    if (reason & DFSR_DWTTRAP) {
        for (uint32_t n = 0; n < watchpoint_count(); n++) {
            const uint32_t function = dwt_function(n);      // reading it clears matched
            if ((function & DWT_FUNCTION_MATCHED) != 0 && debug.watch_function == 0) {
                debug.watch_function = function & 0xF;
                debug.watch_address = dwt_comp(n);
            }
        }
    }
    debug.frame = frame;
    debug.general_thread = 0;
    debug.resume = false;
    debug.stopped = true;
    while (!debug.resume) debug.idle();
    if (debug.step) {
        debug.step = false;
        // The other threads' interrupts that are pending run right here
        debug_thread_priorities(true, debug_thread_of(frame->exception()));
        CoreDebug->DEMCR = CoreDebug->DEMCR | DEMCR_MON_STEP;
    }
    debug.stopped = false;
}

// For the debug monitor's vector. The callee saved floating point registers
// go below the frame, which also puts the others into what the cpu stacked.
[[gnu::naked]] inline void debug_monitor_entry() {
    __asm__("push {r3-r11, lr}\n vpush {s16-s31}\n" DEBUG_FRAME_CFI(104)
        "add r0, sp, #64\n bl debug_monitor\n vpop {s16-s31}\n pop {r3-r11, pc}");
}

// For the hard fault's vector: a breakpoint where the monitor can't run is
// removed, and the instruction it was on runs when this returns. Any other
// hard fault stays here.
extern "C" [[gnu::used]] inline void debug_hard_fault(uint32_t* stacked) {
    using namespace debug_reg;
    constexpr uint32_t HFSR_DEBUGEVT = 1u << 31;
    bool removed = false;
    if (SCB->HFSR & HFSR_DEBUGEVT) {
        const uint32_t pc = stacked[6];
        const uint32_t value = (pc & 0x1FFFFFFC) | (pc & 2 ? 2u << 30 : 1u << 30) | 1;
        for (uint32_t n = 0; n < breakpoint_count(); n++) {
            if (fp_comp(n) == value) {
                fp_comp(n) = 0;
                removed = true;
            }
        }
    }
    while (!removed) __asm__ volatile("");
    SCB->HFSR = HFSR_DEBUGEVT;
    SCB->DFSR = DFSR_BKPT;
    debug.breakpoints_dropped = debug.breakpoints_dropped + 1;
    __asm__ volatile("dsb\nisb");
}
[[gnu::naked]] inline void debug_hard_fault_entry() {
    __asm__("mov r0, sp\n b debug_hard_fault");
}

// The context that runs now, or ran when the node stopped
inline DebugFrame* debug_top_frame() {
    return debug.stopped ? debug.frame : debug.sample;
}
inline uint32_t debug_top_thread() {
    const DebugFrame* frame = debug_top_frame();
    return frame != nullptr ? debug_thread_of(frame->exception()) : 1;
}

// The context of a thread, nullptr if it is not active: from the one on top
// down through what each handler interrupted
inline DebugFrame* debug_thread_context(uint32_t thread) {
    DebugFrame* frame = debug_top_frame();
    for (uint32_t depth = 0; frame != nullptr && depth < debug.thread_count; depth++) {
        const uint32_t frame_thread = debug_thread_of(frame->exception());
        if (frame_thread == thread) return frame;
        if (frame_thread == 1) break;
        frame = debug.thread_frame[frame_thread - 1];
    }
    return nullptr;
}

// Register numbers as in the target description that motor_util gives gdb:
// 0..15 r0 to pc, 16 xpsr, 17..32 d0 to d15, 33 fpscr.
inline constexpr uint32_t debug_register_count = 34;
struct DebugRegister {
    uint32_t* location = nullptr;   // where it can be read and written, or
    uint32_t value = 0;             // what it reads as
    uint32_t words = 1;
    bool available = true;
};
inline DebugRegister debug_register(uint32_t thread, uint32_t n) {
    DebugRegister reg;
    if (n >= 17 && n <= 32) reg.words = 2;
    DebugFrame* frame = debug_thread_context(thread);
    if (frame == nullptr) {
        // Not active: at its handler, which returns to what runs
        const DebugFrame* top = debug_top_frame();
        if (n >= 17 || top == nullptr || thread < 2 || thread > debug.thread_count) reg.available = n < 17 && top != nullptr;
        else if (n == 13) reg.value = reinterpret_cast<uint32_t>(&top->r0);
        else if (n == 14) reg.value = top->exc_return;
        else if (n == 15) reg.value = reinterpret_cast<uint32_t>(debug.threads[thread - 1].entry) & ~1u;
        else if (n == 16) reg.value = 1u << 24 | (debug.threads[thread - 1].irq + 16);
        return reg;
    }
    if (n < 4) reg.location = &frame->r0 + n;
    else if (n < 12) reg.location = &frame->r4 + (n - 4);
    else if (n == 12) reg.location = &frame->r12;
    else if (n == 13) reg.value = frame->sp();
    else if (n == 14) reg.location = &frame->lr;
    else if (n == 15) reg.location = &frame->pc;
    else if (n == 16) reg.location = &frame->xpsr;
    // Floating point: only in memory once the monitor has run
    else if (!debug.stopped) reg.available = false;
    else if (n < 25 && frame->extended()) reg.location = &frame->s[2 * (n - 17)];
    else if (n >= 25 && n < 33 && frame == debug.frame) reg.location = reinterpret_cast<uint32_t*>(frame) - 16 + 2 * (n - 25);
    else if (n == 33 && frame->extended()) reg.location = &frame->fpscr;
    else reg.available = false;
    return reg;
}

inline void debug_put_register(TextOut& out, uint32_t thread, uint32_t n) {
    const DebugRegister reg = debug_register(thread, n);
    for (uint32_t word = 0; word < reg.words; word++) {
        if (!reg.available) {
            out.put("xxxxxxxx");
            continue;
        }
        const uint32_t value = reg.location != nullptr ? reg.location[word] : reg.value;
        out.put_hex(reinterpret_cast<const uint8_t*>(&value), 4);
    }
}

inline void debug_put_hex(TextOut& out, uint32_t value) {
    constexpr char hex[] = "0123456789abcdef";
    bool digits = false;
    for (int shift = 28; shift >= 0; shift -= 4) {
        const uint32_t digit = (value >> shift) & 0xF;
        if (digit != 0 || digits || shift == 0) {
            out.put(hex[digit]);
            digits = true;
        }
    }
}

// The stop reply: the signal, the watchpoint if it was one, and the thread
inline void debug_put_stop(TextOut& out) {
    const uint8_t signal = debug.stopped ? debug.signal : 5;
    out.put('T');
    out.put_hex(&signal, 1);
    if (debug.stopped && debug.watch_function != 0) {
        out.put(debug.watch_function == 5 ? "rwatch:" : debug.watch_function == 6 ? "watch:" : "awatch:");
        debug_put_hex(out, debug.watch_address);
        out.put(';');
    }
    out.put("thread:");
    debug_put_hex(out, debug_top_thread());
    out.put(';');
}

inline void debug_pause(uint8_t signal) {
    if (debug.stopped) return;
    debug.pause_signal = signal;
    CoreDebug->DEMCR = CoreDebug->DEMCR | debug_reg::DEMCR_MON_PEND;
}

// A breakpoint in a free comparator, or out of the one it is in. False if
// there is none free, or the address is not code.
inline bool debug_breakpoint(bool insert, uint32_t address) {
    using namespace debug_reg;
    if (address >= 0x20000000) return false;
    const uint32_t value = (address & 0x1FFFFFFC) | (address & 2 ? 2u << 30 : 1u << 30) | 1;
    int free = -1;
    for (uint32_t n = 0; n < breakpoint_count(); n++) {
        if (fp_comp(n) == value) {
            if (!insert) fp_comp(n) = 0;
            return true;
        }
        if ((fp_comp(n) & 1) == 0 && free < 0) free = n;
    }
    if (!insert) return true;
    if (free < 0) return false;
    fp_comp(free) = value;
    return true;
}

// function: 5 read, 6 write, 7 both. length is a power of 2.
inline bool debug_watchpoint(bool insert, uint32_t function, uint32_t address, uint32_t length) {
    using namespace debug_reg;
    uint32_t mask = 0;
    while ((1u << mask) < length) mask++;
    address &= ~((1u << mask) - 1);
    int free = -1;
    for (uint32_t n = 0; n < watchpoint_count(); n++) {
        const uint32_t in_use = dwt_function(n) & 0xF;
        if (in_use == function && dwt_comp(n) == address && dwt_mask(n) == mask) {
            if (!insert) dwt_function(n) = 0;
            return true;
        }
        if (in_use == 0 && free < 0) free = n;
    }
    if (!insert) return true;
    if (free < 0) return false;
    dwt_comp(free) = address;
    dwt_mask(free) = mask;
    dwt_function(free) = function;
    return true;
}

// A thread id of a packet: 0 for "any" (0 and -1)
inline uint32_t debug_parse_thread(const char*& p) {
    uint32_t thread = 0;
    if (*p == '-') {
        p += 2;
        return 0;
    }
    parse_hex(p, thread);
    return thread;
}

// The run control packets, `request` with its leading "$" and no checksum.
// False if it is not one of these: memory and monitor commands are in
// text_api.h. Call it from the text api interrupt, after debug.sample is set.
//   ?                       the stop reply for the thread that runs: T05thread:1;
//   y                       "continued" while running, the stop reply once stopped
//   b                       stop. The reply is OK, y tells when it has
//   c, s                    continue, step one instruction: OK, then as above
//   D                       detach: breakpoints out and continue
//   g, p n, P n=value       registers of the selected thread, x for those that are not known.
//                           Written only while stopped; sp and those of a thread that is not active never.
//   Hg thread, Hc thread    select it: OK
//   T thread                OK if there is one
//   qC, qfThreadInfo, qsThreadInfo, qThreadExtraInfo,thread
//   Z type,address,kind     insert, z remove: 0 and 1 breakpoint, 2 write, 3 read, 4 access watchpoint
inline bool debug_request(const char* request, TextOut& out) {
    const char* p = request + 1;
    const uint32_t thread = debug.general_thread != 0 ? debug.general_thread : debug_top_thread();
    switch (*p++) {
        case '?':
            debug_put_stop(out);
            return true;
        case 'y':
            if (debug.stopped) debug_put_stop(out);
            else out.put("continued");
            return true;
        case 'b':
            debug_pause(2);
            out.put("OK");
            return true;
        case 's':
            if (debug.stopped) debug.step = true;
            else debug_pause(5);
            [[fallthrough]];
        case 'c':
            debug.general_thread = 0;
            if (debug.stopped) debug.resume = true;
            out.put("OK");
            return true;
        case 'D':
            debug_breakpoints_clear();
            debug.general_thread = 0;
            debug.step = false;
            if (debug.stopped) debug.resume = true;
            out.put("OK");
            return true;
        case 'g':
            for (uint32_t n = 0; n < 17; n++) debug_put_register(out, thread, n);
            return true;
        case 'p': {
            uint32_t n;
            if (!parse_hex(p, n) || n >= debug_register_count) out.put("E01");
            else debug_put_register(out, thread, n);
            return true;
        }
        case 'P': {
            uint32_t n;
            uint32_t value[2];
            if (!parse_hex(p, n) || *p++ != '=' || n >= debug_register_count) {
                out.put("E01");
                return true;
            }
            const DebugRegister reg = debug_register(thread, n);
            if (!debug.stopped || reg.location == nullptr ||
                    parse_hex_bytes(p, reinterpret_cast<uint8_t*>(value), sizeof(value)) != 4 * reg.words) {
                out.put("E01");
                return true;
            }
            if (n == 15) value[0] &= ~1u;
            for (uint32_t word = 0; word < reg.words; word++) reg.location[word] = value[word];
            out.put("OK");
            return true;
        }
        case 'H': {
            const char operation = *p++;
            const uint32_t selected = debug_parse_thread(p);
            if (selected > debug.thread_count) out.put("E01");
            else {
                if (operation == 'g') debug.general_thread = selected;
                out.put("OK");
            }
            return true;
        }
        case 'T': {
            const uint32_t alive = debug_parse_thread(p);
            out.put(alive >= 1 && alive <= debug.thread_count ? "OK" : "E01");
            return true;
        }
        case 'q':
            if (std::strcmp(p, "C") == 0) {
                out.put("QC");
                debug_put_hex(out, debug_top_thread());
            } else if (std::strcmp(p, "fThreadInfo") == 0) {
                out.put('m');
                for (uint32_t i = 1; i <= debug.thread_count; i++) {
                    if (i != 1) out.put(',');
                    debug_put_hex(out, i);
                }
            } else if (std::strcmp(p, "sThreadInfo") == 0) {
                out.put('l');
            } else if (std::strncmp(p, "ThreadExtraInfo,", 16) == 0) {
                p += 16;
                const uint32_t info = debug_parse_thread(p);
                if (info < 1 || info > debug.thread_count) {
                    out.put("E01");
                    return true;
                }
                const char* name = debug.threads[info - 1].name;
                out.put_hex(reinterpret_cast<const uint8_t*>(name), std::strlen(name));
                const char* state = info == debug_top_thread() ? "" : debug_thread_context(info) != nullptr ? ", interrupted" : ", not active";
                out.put_hex(reinterpret_cast<const uint8_t*>(state), std::strlen(state));
            } else {
                return false;
            }
            return true;
        case 'Z':
        case 'z': {
            const bool insert = p[-1] == 'Z';
            const char type = *p++;
            uint32_t address, kind;
            if (*p++ != ',' || !parse_hex(p, address) || *p++ != ',' || !parse_hex(p, kind)) {
                out.put("E01");
                return true;
            }
            if (type == '0' || type == '1') {
                out.put(debug_breakpoint(insert, address) ? "OK" : "E01");
            } else if (type >= '2' && type <= '4') {
                constexpr uint8_t function[] = {6, 5, 7};
                out.put(debug_watchpoint(insert, function[type - '2'], address, kind) ? "OK" : "E01");
            }
            return true;
        }
    }
    return false;
}
