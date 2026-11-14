#pragma once
#include <cstdint>
#include <cstring>
#include "text_api.h"

// Pause, step and breakpoints for gdb without a debug probe, on a Cortex-M4:
// the core's debug monitor exception, driven by the gdb remote protocol
// packets that come in over the text api (text_api.h, `motor_util gdbserver`).
// Include the device header first.
//
// Two exceptions hold what gdb has stopped, each by waiting for gdb's next
// request: what is more urgent than the one that waits keeps running, and
// that always includes the interrupt that takes the text requests in.
//
//     fast loop, text api interrupt       run on, breakpoints there are dropped
//     debug monitor                       holds thread 2.. and with them thread 1
//     thread 2..: interrupts, as listed
//     holder (PendSV)                     holds thread 1 alone
//     thread 1: the background
//
// gdb gets these as threads, and says which of them run with its own
// commands (vCont, below). The interrupt threads are held or run together.
//   - everything stopped: the monitor waits. An interrupt thread that is not
//     active is shown where the one it will interrupt is.
//   - only the background stopped: the holder waits and the interrupts run
//     above it. Breakpoints in them and steps of them work: the monitor is
//     free for those, and then waits on top of the holder.
//   - a step of the background with the others running has the monitor below
//     their interrupts for its duration; with the others held, their
//     interrupts are off for it.
//   - a step of a held interrupt thread that is not active enters its
//     handler: the interrupt is made pending for it.
//   - the background can't run while an interrupt thread is held inside its
//     handler.
// In gdb's all-stop mode everything stops at every event; `set
// scheduler-locking on` or `step` then decides whether the other thread runs
// during a step or a continue. In non-stop mode a stop of the background
// leaves the interrupt threads running.
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
    uint8_t high_priority, low_priority;    // of the monitor: above the threads, and below them
    void (*idle)();                         // called over and over while stopped
    DebugFrame* thread_frame[debug_thread_max];     // what the thread's handler interrupted
    DebugFrame* volatile frame;             // what the monitor interrupted, while stopped
    DebugFrame* sample;                     // what the text api interrupt interrupted
    volatile bool stopped;                  // the monitor or the holder waits: the background is held
    volatile bool waiting, resume;          // the monitor waits: everything is held. resume ends that
    volatile bool exit_step, exit_low, exit_irqs_off;      // how the monitor leaves, set with resume
    DebugFrame* volatile holder_frame;      // what the holder interrupted: the background
    volatile bool holding, holder_release;  // the holder waits, release ends that
    volatile bool holder_step;              // with a step of the background
    volatile bool holder_leaving;           // that step is not yet in the background
    volatile bool holder_pending;           // the holder is on its way
    volatile bool holder_event;             // and its stop is one to tell gdb of
    volatile bool pause_interrupts;         // the stop that was asked for is one of everything
    volatile bool keep_interrupts;          // the interrupt threads stay held over a step of the background
    volatile bool irqs_off;
    volatile bool background_free;          // for gdb the background runs: it is only kept from it by the interrupt
                                            // threads being held, which gdb's non-stop mode does not take as a stop
    volatile bool pause_background;         // the stop that was asked for names the background too
    bool non_stop;                          // gdb's mode
    volatile uint8_t pause_signal;
    volatile uint8_t step_thread;           // the thread a step was for
    volatile uint8_t pause_thread;          // and a stop that was asked for
    uint32_t event_count;                   // stops so far
    uint8_t event_thread;                   // and the thread of the last one
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

inline void debug_monitor_level(bool high) {
    NVIC_SetPriority(DebugMonitor_IRQn, high ? debug.high_priority : debug.low_priority);
}

// threads[0] names the background, the others are interrupts. high_priority
// is a level between the text api's interrupt, which calls debug_request(),
// and those of the threads; low_priority one below all of them, with the
// next one free for the holder.
inline void debug_monitor_init(const DebugThread* threads, uint32_t thread_count, uint8_t high_priority,
                               uint8_t low_priority, void (*idle)()) {
    debug.threads = threads;
    debug.thread_count = thread_count < debug_thread_max ? thread_count : debug_thread_max;
    debug.high_priority = high_priority;
    debug.low_priority = low_priority;
    debug.idle = idle;
    debug.pause_signal = 5;
    debug_monitor_level(true);
    NVIC_SetPriority(PendSV_IRQn, low_priority + 1);
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

// The interrupts of the threads off, for the background to run without them
inline void debug_thread_irqs(bool enable) {
    for (uint32_t i = 1; i < debug.thread_count; i++) {
        const IRQn_Type irq = static_cast<IRQn_Type>(debug.threads[i].irq);
        if (enable) NVIC_EnableIRQ(irq);
        else NVIC_DisableIRQ(irq);
    }
    debug.irqs_off = !enable;
}

inline constexpr uint32_t debug_holder_exception = 14;      // PendSV

// The holder takes the background when everything above it has returned
inline void debug_hold(bool event) {
    debug.holder_event = event;
    debug.holder_pending = true;
    SCB->ICSR = SCB_ICSR_PENDSVSET_Msk;
}

// What a frame interrupted, if that was the holder: the background
inline DebugFrame* debug_below_holder(DebugFrame* frame) {
    return frame != nullptr && frame->exception() == debug_holder_exception ? debug.holder_frame : frame;
}

inline void debug_event(uint8_t signal, uint32_t thread) {
    debug.signal = signal;
    debug.event_thread = thread;
    debug.event_count = debug.event_count + 1;
}

// The monitor exception: a breakpoint, a watchpoint, a step or a stop that was
// asked for. Here everything is held until a request says to go on.
extern "C" [[gnu::used]] inline void debug_monitor(DebugFrame* frame) {
    using namespace debug_reg;
    const uint32_t reason = SCB->DFSR;
    SCB->DFSR = reason;
    if ((reason & DFSR_HALTED) != 0 && debug.holder_leaving) {
        // A step of the background that is still in the holder's way out, or
        // has just returned from it without an instruction of the background: on with it
        if (frame->exception() != debug_holder_exception) debug.holder_leaving = false;
        return;
    }
    debug.holder_leaving = false;
    CoreDebug->DEMCR = CoreDebug->DEMCR & ~DEMCR_MON_STEP;
    debug_monitor_level(true);
    if (debug.irqs_off) debug_thread_irqs(true);
    const uint32_t top = debug_thread_of(debug_below_holder(frame)->exception());
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
    const bool event = (reason & (DFSR_HALTED | DFSR_BKPT | DFSR_DWTTRAP)) != 0;
    // The thread of a step or of a stop that was asked for is the one gdb
    // named, also when a step has left its handler
    const bool stepped = (reason & DFSR_HALTED) != 0 && (reason & (DFSR_BKPT | DFSR_DWTTRAP)) == 0;
    debug_event(event ? 5 : debug.pause_signal, stepped && debug.step_thread != 0 ? debug.step_thread :
        !event && debug.pause_thread != 0 ? debug.pause_thread : top);
    const bool everything = !debug.non_stop || top >= 2 || debug.step_thread >= 2 || debug.pause_interrupts || debug.keep_interrupts;
    debug.pause_signal = 5;
    debug.pause_interrupts = false;
    debug.keep_interrupts = false;
    debug.step_thread = 0;
    debug.pause_thread = 0;
    if (!debug.non_stop || debug.event_thread == 1 || debug.pause_background) debug.background_free = false;
    else if (everything && !debug.holding && !debug.holder_pending) debug.background_free = true;
    debug.pause_background = false;
    if (!everything) {
        // only the background stops: that is the holder's, the interrupts go on
        if (!debug.holding) debug_hold(false);
        return;
    }
    debug.frame = frame;
    debug.resume = false;
    debug.exit_step = false;
    debug.exit_low = false;
    debug.exit_irqs_off = false;
    debug.waiting = true;
    debug.stopped = true;
    while (!debug.resume) debug.idle();
    if (debug.exit_irqs_off) debug_thread_irqs(false);
    if (debug.exit_low) debug_monitor_level(false);
    if (debug.exit_step) CoreDebug->DEMCR = CoreDebug->DEMCR | DEMCR_MON_STEP;
    debug.waiting = false;
    debug.stopped = debug.holding || debug.holder_pending;
}

// The holder: the background alone is held here until a request says to go on
extern "C" [[gnu::used]] inline void debug_holder(DebugFrame* frame) {
    debug.holder_frame = frame;
    if (debug.holder_event) debug_event(debug.pause_signal, 1);
    debug.holder_event = false;
    debug.pause_signal = 5;
    debug.holder_release = false;
    debug.holder_step = false;
    debug.holding = true;
    debug.stopped = true;
    debug.holder_pending = false;
    while (!debug.holder_release) debug.idle();
    debug.holding = false;
    debug.stopped = false;
    if (debug.holder_step) {
        // one instruction of the background, counted from when this has returned
        debug.holder_leaving = true;
        debug_monitor_level(false);
        CoreDebug->DEMCR = CoreDebug->DEMCR | debug_reg::DEMCR_MON_STEP;
    }
}

// For the vectors of the debug monitor and of PendSV. The callee saved
// floating point registers go below the frame, which also puts the others
// into what the cpu stacked.
[[gnu::naked]] inline void debug_monitor_entry() {
    __asm__("push {r3-r11, lr}\n vpush {s16-s31}\n" DEBUG_FRAME_CFI(104)
        "add r0, sp, #64\n bl debug_monitor\n vpop {s16-s31}\n pop {r3-r11, pc}");
}
[[gnu::naked]] inline void debug_holder_entry() {
    __asm__("push {r3-r11, lr}\n vpush {s16-s31}\n" DEBUG_FRAME_CFI(104)
        "add r0, sp, #64\n bl debug_holder\n vpop {s16-s31}\n pop {r3-r11, pc}");
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

// The context that runs now, or the most urgent one that is held
inline DebugFrame* debug_top_frame() {
    if (debug.waiting) return debug_below_holder(debug.frame);
    if (debug.holding) return debug.holder_frame;
    return debug_below_holder(debug.sample);
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
        frame = debug_below_holder(debug.thread_frame[frame_thread - 1]);
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
TEXT_API_SMALL inline DebugRegister debug_register(uint32_t thread, uint32_t n) {
    DebugRegister reg;
    if (n >= 17 && n <= 32) reg.words = 2;
    DebugFrame* frame = debug_thread_context(thread);
    // An interrupt thread that is not active is where what it will interrupt
    // is: stepped out of its handler it is back there, as gdb expects of a
    // signal handler, and a step from there enters the handler.
    if (frame == nullptr) frame = debug_top_frame();
    if (frame == nullptr) {
        reg.available = false;
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
    else if (n >= 25 && n < 33 && ((debug.waiting && frame == debug.frame) || (debug.holding && frame == debug.holder_frame))) reg.location = reinterpret_cast<uint32_t*>(frame) - 16 + 2 * (n - 25);
    else if (n == 33 && frame->extended()) reg.location = &frame->fpscr;
    else reg.available = false;
    return reg;
}

TEXT_API_SMALL inline void debug_put_register(TextOut& out, uint32_t thread, uint32_t n) {
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
TEXT_API_SMALL inline void debug_put_stop(TextOut& out) {
    const uint8_t signal = debug.stopped ? debug.signal : 5;
    out.put('T');
    out.put_hex(&signal, 1);
    if (debug.stopped && debug.watch_function != 0) {
        out.put(debug.watch_function == 5 ? "rwatch:" : debug.watch_function == 6 ? "watch:" : "awatch:");
        debug_put_hex(out, debug.watch_address);
        out.put(';');
    }
    out.put("thread:");
    debug_put_hex(out, debug.stopped ? debug.event_thread : debug_top_thread());
    out.put(';');
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

// Stops everything, wherever it is
inline void debug_pause(uint8_t signal, uint32_t thread, bool background) {
    if (debug.waiting) return;
    debug.pause_signal = signal;
    debug.pause_interrupts = true;
    debug.pause_background = background;
    debug.pause_thread = thread;
    CoreDebug->DEMCR = CoreDebug->DEMCR | debug_reg::DEMCR_MON_PEND;
}

// What gdb wants each thread to do: 'c' continue, 's' step, 't' stop, 0
// nothing. background is thread 1's, interrupts that of the others. False if
// it can't be done.
inline bool debug_resume(char background, char interrupts) {
    const bool background_go = background == 'c' || background == 's';
    const bool interrupts_go = interrupts == 'c' || interrupts == 's';
    if (interrupts_go && debug.irqs_off) debug_thread_irqs(true);
    if (!debug.waiting) {
        // the interrupt threads run
        if (interrupts == 't' || interrupts == 's') {
            debug_pause(interrupts == 't' ? 0 : 5, 2, background == 't' || debug.stopped);
        } else if (!debug.stopped) {
            // and the background: a step of what runs is a stop
            if (background == 't' || background == 's') {
                if (!debug.non_stop) {
                    debug_pause(background == 't' ? 0 : 5, 0, true);
                } else {
                    debug.pause_signal = background == 't' ? 0 : 5;
                    debug_hold(true);
                }
            }
        } else if (background_go && debug.holding) {
                    debug.step_thread = background == 's' ? 1 : 0;
            debug.holder_step = background == 's';
            debug.holder_release = true;
        }
        return true;
    }
    if (background == 't' && debug.background_free) {
        debug.background_free = false;
        debug_event(0, 1);
    }
    if (!background_go && !interrupts_go) return true;
    const uint32_t top = debug_thread_of(debug_below_holder(debug.frame)->exception());
    if (background_go && top >= 2 && !interrupts_go) {
        // Under a handler that is held: it runs once that lets it. No step of it.
        if (background == 's') return false;
        debug.background_free = true;
        if (debug.holding) debug.holder_release = true;
        return true;
    }
    if (background_go) {
        if (interrupts == 's') {
            debug.step_thread = top >= 2 ? top : 2;
            if (top < 2) NVIC_SetPendingIRQ(static_cast<IRQn_Type>(debug.threads[1].irq));
            debug.exit_step = true;
        } else if (interrupts_go) {
            if (background == 's') {
                debug.step_thread = 1;
                debug.exit_step = true;
                debug.exit_low = true;
            }
        } else {
            debug.background_free = false;      // it runs for real
            debug.exit_irqs_off = true;
            debug.keep_interrupts = true;
            if (background == 's') {
                debug.step_thread = 1;
                debug.exit_step = true;
            }
        }
        if (debug.holding) {
            debug.holder_release = true;
            debug.holder_leaving = debug.exit_step && interrupts != 's';    // the step goes through the holder first
        }
    } else if (interrupts == 'c') {
        if (!debug.holding && !debug.background_free) debug_hold(false);    // the background stays where it is
        debug.background_free = false;
    } else {
        debug.step_thread = top >= 2 ? top : 2;
        if (top < 2) NVIC_SetPendingIRQ(static_cast<IRQn_Type>(debug.threads[1].irq));
        debug.exit_step = true;
    }
    debug.resume = true;
    return true;
}

// vCont;action[:thread];...: the first action that names a thread, or none, is its
inline bool debug_vcont(const char* p) {
    char action[2] = {0, 0};       // background, interrupts
    while (*p == ';') {
        const char a = p[1] == 'C' ? 'c' : p[1] == 'S' ? 's' : p[1];
        p += 2;
        while (*p != 0 && *p != ':' && *p != ';') p++;     // the signal of C and S
        uint32_t thread = 0;
        if (*p == ':') {
            p++;
            thread = debug_parse_thread(p);
        }
        for (uint32_t i = 0; i < 2; i++) {
            const bool named = thread == 0 || (i == 0 ? thread == 1 : thread >= 2);
            if (named && action[i] == 0) action[i] = a;
        }
    }
    return debug_resume(action[0], action[1]);
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

// The run control packets, `request` with its leading "$" and no checksum.
// False if it is not one of these: memory and monitor commands are in
// text_api.h. Call it from the text api interrupt, after debug.sample is set.
//   ?                       the stop reply for the thread that runs: T05thread:1;
//   y                       "continued" while a thread 2.. runs, the stop reply once everything is stopped
//   Y                       for non-stop: stops so far;threads stopped, a bit each;the last stop reply
//   b                       stop everything. The reply is OK, y tells when it has
//   c, s                    continue all, step the thread that runs with the others running: OK, then as above
//   vCont;action:thread;... c, s or t (stop) for each thread, see debug_resume
//   QNonStop:1, QNonStop:0  gdb's mode: what else stops with the background
//   D                       detach: breakpoints out and continue
//   g, p n, P n=value       registers of the selected thread, x for those that are not known.
//                           Written only while stopped; sp and those of a thread that is not active never.
//   Hg thread, Hc thread    select it: OK
//   T thread                OK if there is one
//   qC, qfThreadInfo, qsThreadInfo, qThreadExtraInfo,thread
//   Z type,address,kind     insert, z remove: 0 and 1 breakpoint, 2 write, 3 read, 4 access watchpoint
TEXT_API_SMALL inline bool debug_request(const char* request, TextOut& out) {
    const char* p = request + 1;
    const uint32_t thread = debug.general_thread != 0 ? debug.general_thread : debug_top_thread();
    switch (*p++) {
        case '?':
            debug_put_stop(out);
            return true;
        case 'y':
            if (debug.waiting) debug_put_stop(out);
            else out.put("continued");
            return true;
        case 'Y': {
            debug_put_hex(out, debug.event_count);
            out.put(';');
            const bool background = (debug.stopped || debug.holder_pending) && !debug.background_free;
            const bool interrupts = debug.waiting || debug.irqs_off;
            debug_put_hex(out, (background ? 1 : 0) | (interrupts ? ((1u << debug.thread_count) - 1) & ~1u : 0));
            out.put(';');
            debug_put_stop(out);
            return true;
        }
        case 'b':
            debug_pause(2, 0, true);
            out.put("OK");
            return true;
        case 's': {
            const bool top_is_interrupt = debug.waiting && debug_top_thread() >= 2;
            out.put(debug_resume(top_is_interrupt ? 0 : 's', top_is_interrupt ? 's' : 'c') ? "OK" : "E01");
            return true;
        }
        case 'c':
            out.put(debug_resume('c', 'c') ? "OK" : "E01");
            return true;
        case 'v':
            if (std::strncmp(p, "Cont;", 5) != 0) return false;
            out.put(debug_vcont(p + 4) ? "OK" : "E01");
            return true;
        case 'Q':
            if (std::strncmp(p, "NonStop:", 8) != 0) return false;
            debug.non_stop = p[8] == '1';
            out.put("OK");
            return true;
        case 'D':
            debug_breakpoints_clear();
            debug.non_stop = false;
            debug.general_thread = 0;
            debug_resume('c', 'c');
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
                const char* state = info == debug_top_thread() ? "" : debug_thread_context(info) != nullptr ? ", interrupted" :
                    debug.stopped && !debug.waiting ? ", running" : ", not active";
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
