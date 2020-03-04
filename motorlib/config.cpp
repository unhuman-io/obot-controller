#include "config.h"
#include "led.h"
#include "communication.h"
#include "control_fun.h"
#include "encoder.h"
#include "peripheral/pwm.h"
#include "gpio.h"
#include "peripheral/usb.h"
#include <cstring>


#include "actuator.h"

const Config config;
static USB1 usb_;
#include "config_g474_boost.cpp"
static Actuator actuator_ = {config_items.fast_loop, config_items.main_loop};

// limited to 64 bytes
void send_string(const char * str) {
    if (!usb_.tx_active(1)) {
        usb_.send_data(1, (const uint8_t *) str, std::strlen(str)+1, false);
    }
}

char *get_string() {
    static char buf[64];
    int count = usb_.receive_data(1, (uint8_t *) buf, 64);
    buf[count] = 0;
    if (count) {
        return buf;
    } else {
        return NULL;
    }
}

Config::Config() :
    fast_loop(config_items.fast_loop),
    main_loop(config_items.main_loop),
    actuator(actuator_)
    {}

void actuator_run() {
    actuator_.run();
}

void usb_interrupt() {
    usb_.interrupt();
}