#ifndef UTIL_H
#define UTIL_H

#include "stdint.h"
#include "core_cm4.h"

static inline uint32_t get_clock() { return DWT->CYCCNT; }
static inline uint8_t get_cpi_count() { return DWT->CPICNT; }
static inline uint8_t get_lsu_count() { return DWT->LSUCNT; }

#endif