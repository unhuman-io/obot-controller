#ifndef USB_H
#define USB_H

#include <cstdint>
// #include "stm32f4xx_hal.h"
// #include "stm32f4xx_ll_usb.h"
#include "stm32g4xx_hal.h"


typedef struct { // up to 1024 bytes, 16 bit access only, first table is 64 bytes, reception buffers need two additional bytes for CRC
    struct {
    __IO uint16_t ADDR_TX;
    __IO uint16_t COUNT_TX;
    __IO uint16_t ADDR_RX;
    __IO uint16_t COUNT_RX;
    } btable[8];

    // below buffer is user specified
    struct {
    __IO uint16_t EP_TX[32];
    __IO uint16_t EP_RX[48];
    } buffer[3];
} USBPMA_TypeDef;
#define USBPMA ((USBPMA_TypeDef *) USB_PMAADDR)

typedef struct {
    struct {
        __IO uint16_t EPR;
        __IO uint16_t reserved;
    } EP[8];
} USBEPR_TypeDef;
#define USBEPR ((USBEPR_TypeDef *) &(USB->EP0R))


class USB1 {
 public:
    // limited to 64 bytes
    void send_data(uint8_t endpoint, const uint8_t *data, uint8_t length, bool wait = true) {
        while ((USBEPR->EP[endpoint].EPR & USB_EPTX_STAT) == USB_EP_TX_VALID) {
            if (wait) {
                continue;
            } else {
                epr_set_toggle(endpoint, USB_EP_TX_NAK, USB_EPTX_STAT);
            }
        }
        
        uint8_t length16 = (length+1)>>1;
        __IO uint16_t * pma_address = USBPMA->buffer[endpoint].EP_TX;
        for(int i=0; i<length16; i++) {
            pma_address[i] = ((const uint16_t *) data)[i];
        }
        USBPMA->btable[endpoint].COUNT_TX = length;
        epr_set_toggle(endpoint, USB_EP_TX_VALID, USB_EPTX_STAT);
    }

    // receive up to length bytes from endpoint, return number of bytes read
    int receive_data(uint8_t endpoint, uint8_t * const data, uint8_t length);


    void send_string(uint8_t endpoint, const char *str, uint8_t length) {
        uint16_t str_out[length+1] = {};
        uint8_t length_total = 2 + 2*length;
        str_out[0] = length_total | (3 << 8); // header
        for (int i=0; i<length; i++) {
            str_out[i+1] = str[i];
        }
        send_data(endpoint, reinterpret_cast<const uint8_t *>(str_out), length_total);
    }

    // send tx stall
    void send_stall(uint8_t endpoint) {
        epr_set_toggle(endpoint, USB_EP_TX_STALL, USB_EPTX_STAT);
    }

    void read_pma(uint8_t byte_count, __IO uint16_t * pma_address, uint8_t *buffer_out) {
        int count_received_16 = (byte_count + 1) >> 1;
        for(int i=0; i<count_received_16; i++) {
            ((uint16_t *) buffer_out)[i] = pma_address[i];
        }
    }

    // special function due to difficulty of toggle bits and clear bits;
    // hopefully hardware doesn't change values during this function
    void epr_set_toggle(uint8_t endpoint, uint16_t set_bits, uint16_t set_mask) {
        uint16_t epr = USBEPR->EP[endpoint].EPR;
        uint16_t epr_toggle = (epr & set_mask) ^ set_bits;
        uint16_t epr_normal = epr & USB_EPREG_MASK;
        uint16_t epr_total = epr_normal | epr_toggle | USB_EP_CTR_TX | USB_EP_CTR_RX; // always write 1 to not clear CTR
        USBEPR->EP[endpoint].EPR = epr_total;
    }

    void interrupt() {
        /* Handle Reset Interrupt */
        if (USB->ISTR & USB_ISTR_RESET)
        {
            // Set up endpoint 0
            USB->EP0R = USB_EP_CONTROL;
            USBPMA->btable[0].ADDR_TX = offsetof(USBPMA_TypeDef, buffer[0].EP_TX);
            epr_set_toggle(0, USB_EP_TX_NAK, USB_EPTX_STAT | USB_EP_DTOG_TX);
                // sets the toggle only bits to DIS and clears DTOG, hardware better not change EPR during operation
            USBPMA->btable[0].ADDR_RX = offsetof(USBPMA_TypeDef, buffer[0].EP_RX);
            USBPMA->btable[0].COUNT_RX =  (1 << USB_COUNT0_RX_BLSIZE_Pos) | (2 << USB_COUNT0_RX_NUM_BLOCK_Pos); // 1:2 -> 96 byte allocation
            epr_set_toggle(0, USB_EP_RX_VALID, USB_EPRX_STAT | USB_EP_DTOG_RX);
                // similar to above TX

            // enable interrupts

            USB->DADDR = USB_DADDR_EF; // device address 0 is effective

            USB->ISTR &= ~USB_ISTR_RESET;
        }

        // Suspend interrupt
        if (USB->ISTR & USB_ISTR_SUSP) {
            USB->ISTR &= ~USB_ISTR_SUSP;
        }
    
        // Endpoint correct transfer interrupt
        if(USB->ISTR & USB_ISTR_CTR)
        {
            uint16_t istr = USB->ISTR;
            switch (istr & USB_ISTR_EP_ID) {
                case 0:
                    if (istr & USB_ISTR_DIR) { // RX
                        if (USB->EP0R & USB_EP_SETUP) {
                            uint8_t buffer[64];
                            uint8_t byte_count = USBPMA->btable[0].COUNT_RX & USB_COUNT0_RX_COUNT0_RX;
                            read_pma(byte_count, USBPMA->buffer[0].EP_RX, buffer);
                            handle_setup_packet(buffer);
                            // clear CTR
                            USB->EP0R &= USB_EPREG_MASK & ~USB_EP_CTR_RX;
                            // renable rx on ep0
                            epr_set_toggle(0, USB_EP_RX_VALID, USB_EPRX_STAT);
                        } else { // non setup rx
                            USB->EP0R &= USB_EPREG_MASK & ~USB_EP_CTR_RX;
                            epr_set_toggle(0, USB_EP_RX_VALID, USB_EPRX_STAT);
                        }
                    }
                    if (USB->EP0R & USB_EP_CTR_TX) {
                        USB->EP0R &= USB_EPREG_MASK & ~USB_EP_CTR_TX;
                    }
                    break;
                case 2:
                    if (istr & USB_ISTR_DIR) { // RX
                        USB->EP2R &= USB_EPREG_MASK & ~USB_EP_CTR_RX;
                        count_rx_ = (USBPMA->btable[2].COUNT_RX & USB_COUNT2_RX_COUNT2_RX);
                        read_pma(count_rx_, USBPMA->buffer[2].EP_RX, rx_buffer_);
                        new_rx_data_ = true;
                        epr_set_toggle(2, USB_EP_RX_VALID, USB_EPRX_STAT);
                    }
                    if (USB->EP2R & USB_EP_CTR_TX) {
                        USB->EP2R &= USB_EPREG_MASK & ~USB_EP_CTR_TX;
                    }
                    break;
            }
        }

        if (USB->ISTR & USB_ISTR_ERR) {
            error_count_++;
            USB->ISTR &= ~USB_ISTR_ERR;
        }
    }

 private:
    void handle_setup_packet(uint8_t *setup_data);

    uint8_t device_address_ = 0;
    uint16_t interface_ = 0;
    bool new_rx_data_ = false;
    uint8_t count_rx_ = 0;
    uint8_t rx_buffer_[64];
    uint32_t error_count_ = 0;
    uint8_t go_to_bootloader = 0; // todo different
};

#endif
