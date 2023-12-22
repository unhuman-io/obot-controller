#ifndef ST_DEVICE_H
#define ST_DEVICE_H

#include "stm32f446xx.h"
#include "stm32f4xx_hal.h"
#define CPU_FREQUENCY_HZ    180e6

#ifdef __cplusplus
inline bool is_rom(void * p) {
    return (uint32_t) p >= FLASH_BASE && (uint32_t) p <= FLASH_END;
}
#endif

#endif
