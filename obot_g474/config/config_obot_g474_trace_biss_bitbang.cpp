#include "st_device.h"
#include <fcntl.h>
#include <cstring>
#include "../../motorlib/logger.h"
#include "../../motorlib/peripheral/stm32g4/pin_config.h"
#include "../../motorlib/gpio.h"
#include "../../motorlib/biss_bitbang.h"

Logger::CIndex log_index;
char log_queue[LOGGING_MAX_SIZE];
__attribute__ ((init_priority(LOGGER_INIT_PRIORITY))) Logger logger(log_index, log_queue);

extern "C" {
void SystemClock_Config();
void board_init() {
    SystemClock_Config();
}
void system_loop_interrupt() {}
void fast_loop_interrupt() {}
void main_loop_interrupt() {}
void usb_interrupt() {}


void system_init() {
    RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN | RCC_AHB2ENR_GPIODEN | RCC_AHB2ENR_GPIOEEN;
    
    GPIO_SETL(E, 2, GPIO_MODE::ALT_FUN, GPIO_SPEED::HIGH, 0); // trace port
    GPIO_SETL(E, 3, GPIO_MODE::ALT_FUN, GPIO_SPEED::HIGH, 0);
    GPIO_SETL(E, 4, GPIO_MODE::ALT_FUN, GPIO_SPEED::HIGH, 0);
    GPIO_SETL(E, 5, GPIO_MODE::ALT_FUN, GPIO_SPEED::HIGH, 0);
    GPIO_SETL(E, 6, GPIO_MODE::ALT_FUN, GPIO_SPEED::HIGH, 0);

    ITM->TCR = 1;

    GPIO_SETL(A, 0, GPIO_MODE::OUTPUT, GPIO_SPEED::LOW, 0);
    GPIO_SETL(B, 0, GPIO_MODE::INPUT, GPIO_SPEED::LOW, 0);

    GPIO clk(*GPIOA, 0, GPIO::OUTPUT);
    GPIO dat(*GPIOB, 0, GPIO::INPUT);
    clk.set();
    us_delay(21);
    BISSBitBang biss(clk, dat, 12);
    bool cds_value;
    uint32_t value;
    int err = biss.transfer_one(true, cds_value, value);
    printf("err: %d, cds: %d, value: %lu\n", err, cds_value, value);
    biss.transfer_one(false, cds_value, value);
}

void system_run() {
}

void _write(int fd, const char *buf, size_t count) {
    for (size_t i = 0; i < count; i++) {
        ITM_SendChar(buf[i]);
    }
}
int _fstat (int fd, struct stat * st) {
    memset (st, 0, sizeof (* st));
    st->st_mode = S_IFCHR;
    setbuf(stdout, NULL); // Disable buffering for stdout
    setbuf(stderr, NULL);
    return (0);
}
int _isatty (int fd) {
    return (1);
}


} // extern "C"

void config_init() {}

void config_maintenance() {}

