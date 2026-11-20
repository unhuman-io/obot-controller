#pragma once
// DEBUG_CALL in front of a function keeps it in the build with the text api
// and gdb (OBOT3_TEXT_API 1), also when nothing calls it or every call of it
// is inlined, so that gdb can call it by name through the call thread of
// debug_monitor.h:
//   (gdb) thread apply 3 print tmotor_drv8323_status()
// DEBUG_KEEP is the same for a variable that is only there for gdb to read.
// In the release build they are nothing. An inline function is still inlined
// where it is used.
//
// In a module DEBUG_CALL also takes the function out of the module's names
// (extern "C++"): gdb knows a function of a module only as
// 'tmotor_led@tmotor_driver', in quotes.
#if !OBOT3_TEXT_API
#define DEBUG_KEEP
#elif defined(__clang__)
#define DEBUG_KEEP [[gnu::used, gnu::retain]]   // used: emitted, retain: not dropped by the linker
#else
#define DEBUG_KEEP [[gnu::used]]                // gcc for this target has no retain, and its linker keeps what is used
#endif
#if OBOT3_TEXT_API
#define DEBUG_CALL extern "C++" DEBUG_KEEP
#else
#define DEBUG_CALL
#endif
