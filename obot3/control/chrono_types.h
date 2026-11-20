#pragma once
// std::chrono's durations, time points and is_clock, for the clocks of
// clock.cppm and for what uses them. With gcc that is <chrono>. With clang it
// can't be: the builds here make floating constants single precision, and
// libc++ has the length of a year as one, so its calendar, which <chrono>
// brings in, does not compile. Its headers for these parts do.
#if __has_include(<__chrono/time_point.h>)
#include <__chrono/duration.h>
#include <__chrono/time_point.h>
#if __has_include(<__chrono/is_clock.h>)
#include <__chrono/is_clock.h>
#endif
#else
#include <chrono>
#endif
