#include "usb.h"

#include "stm32g474xx.h"

void USBD::init() {} 

void USBD::usb_interrupt() {
    int a = 3;
    int b = a+3;
    if (USB->ISTR & USB_ISTR_RESET) {
        USB->EP0R = USB_EP_CONTROL | USB_EP_CTR_RX | USB_EP_CTR_TX;

        USB->DADDR = USB_DADDR_EF;
        USB->ISTR &= ~USB_ISTR_RESET;
    }
    if (USB->ISTR & USB_ISTR_CTR) { // correct transfer interrupt
        if ((USB->ISTR & USB_ISTR_EP_ID) == 0) {    // EP0
            if ((USB->ISTR & USB_ISTR_DIR) == 0 ) { // 0->IN
                
            } else {    // OUT
                if (USB->EP0R & USB_EP_SETUP) {
                    int a = USB->BTABLE;
                }
            }
        }
    }
}
