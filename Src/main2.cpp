#include "usb.h"
#include <functional>
#include "main2.h"

USBD usb;
std::function<void ()> f;

void init() {
    usb.init();
 //   f = std::bind(&USBD::usb_interrupt, &usb);
}

void usb_interrupt() {
 //   f();
}