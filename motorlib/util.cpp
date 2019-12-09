#include "util.h"

void ms_delay(uint16_t ms) {
    uint32_t t_start = get_clock();
    while((get_clock() - t_start) < ms*CPU_FREQUENCY_HZ/1000);
}