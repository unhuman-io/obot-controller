module;

#include <cstddef>
#include <cstring>
#include <algorithm>

export module usb;

import obot_std;
import stm32g474;

enum class EP_STAT {
    DISABLED = 0,
    STALL = 1,
    NAK = 2,
    VALID = 3
};

enum class EP_TYPE {
    BULK = 0,
    CONTROL = 1,
    ISO = 2,
    INTERRUPT = 3
};

constexpr uint32_t USB_PMAADDR = 0x40006000; // USB packet memory area address
constexpr uint32_t USB_COUNT_RX_BLSIZE_Pos = 15;
constexpr uint32_t USB_COUNT_RX_NUM_BLOCK_Pos = 10;
constexpr uint32_t USB_COUNT_RX_COUNT_RX = 0x3FFul;

// signal to exit
extern uint32_t go_to_bootloader;
struct usb_control_request;
class USB {
 public:
    USB();
    void connect();
    // limited to 64 bytes
    void send_data(uint8_t endpoint, const uint8_t * const data, uint16_t length, bool wait = true, uint32_t wait_timeout_us = 10000);
    void _send_data(uint8_t endpoint, const uint8_t *data, uint8_t length);

    // receive up to length bytes from endpoint, return number of bytes read
    int receive_data(uint8_t endpoint, uint8_t * const data, uint8_t length);

    void send_string(uint8_t endpoint, const char * const str, uint8_t length);

    bool tx_active(uint8_t endpoint);

    void interrupt();

    bool new_rx_data(uint8_t endpoint) const { return new_rx_data_[endpoint]; }
    bool tx_data_ack(uint8_t endpoint) { bool tx_data_ack = tx_data_ack_[endpoint]; tx_data_ack_[endpoint] = false; return tx_data_ack; }

    bool cancel_transfer(uint8_t endpoint, uint32_t timeout_ns = 5000);

     // send tx stall
    void send_stall(uint8_t endpoint);

    void handle_setup_packet(usb_control_request *setup_data);

    // special function due to difficulty of toggle bits and clear bits;
    // hopefully hardware doesn't change values during this function
    void epr_set_stat_rx(uint8_t endpoint, EP_STAT stat, bool toggle = false);
    void epr_set_stat_tx(uint8_t endpoint, EP_STAT stat, bool toggle = false);

    uint8_t device_address_ = 0;
    uint16_t interface_ = 0;
    volatile bool new_rx_data_[3] = {};
    volatile bool tx_data_ack_[3] = {};
    uint8_t count_rx_[3] = {};
    uint8_t rx_buffer_[3][64] = {};
    uint32_t error_count_ = 0;
    friend class System;
    uint32_t reset_count_ = 0;
    USB_FS_device_Type &regs_;
};

typedef struct { // up to 1024 bytes, 16 bit access only, first table is 64 bytes, reception buffers need two additional bytes for CRC
    struct {
    uint16_t ADDR_TX;
    uint16_t COUNT_TX;
    uint16_t ADDR_RX;
    uint16_t COUNT_RX;
    } btable[8];

    // below buffer is user specified
    struct {
    uint16_t EP_TX[32];
    uint16_t EP_RX[48];
    } buffer[3];
} USBPMA_TypeDef;
#define USBPMA ((USBPMA_TypeDef *) USB_PMAADDR)

typedef struct {
    union {
        decltype(USB_FS_device_Type::EP0R) EPR[8];
        decltype(USB_FS_device_Type::EP0R_b) EP[8];
    };
} USBEPR_TypeDef;
#define USBEPR ((USBEPR_TypeDef *) &(regs_.EP0R))

union EPReg {
    EPReg() : word(0) {}
    constexpr EPReg(decltype(USB_FS_device_Type::EP0R_b) &epr) : word(std::bit_cast<uint32_t>(epr)) {}
    uint32_t word;                        /*!< (@ 0x00000018) USB endpoint n register                                    */
    
    struct {
        uint32_t EA         : 4;            /*!< [3..0] EA                                                                 */
        uint32_t STAT_TX    : 2;            /*!< [5..4] STAT_TX                                                            */
        uint32_t DTOG_TX    : 1;            /*!< [6..6] DTOG_TX                                                            */
        uint32_t CTR_TX     : 1;            /*!< [7..7] CTR_TX                                                             */
        uint32_t EP_KIND    : 1;            /*!< [8..8] EP_KIND                                                            */
        uint32_t EP_TYPE    : 2;            /*!< [10..9] EP_TYPE                                                           */
        uint32_t SETUP      : 1;            /*!< [11..11] SETUP                                                            */
        uint32_t STAT_RX    : 2;            /*!< [13..12] STAT_RX                                                          */
        uint32_t DTOG_RX    : 1;            /*!< [14..14] DTOG_RX                                                          */
        uint32_t CTR_RX     : 1;            /*!< [15..15] CTR_RX                                                           */
        uint32_t            : 16;
    };
};



#define LOBYTE(x)  ((uint8_t)(x & 0x00FF))
#define HIBYTE(x)  ((uint8_t)((x & 0xFF00) >>8))
#define  USB_DESC_TYPE_DEVICE                              1
#define  USB_DESC_TYPE_CONFIGURATION                       2
#define  USB_DESC_TYPE_STRING                              3
#define  USB_DESC_TYPE_INTERFACE                           4
#define  USB_DESC_TYPE_ENDPOINT                            5
#define  USB_DESC_TYPE_DEVICE_QUALIFIER                    6
#define  USB_DESC_TYPE_OTHER_SPEED_CONFIGURATION           7
#define  USB_DESC_TYPE_BOS                                 0x0F
#define  USBD_IDX_LANGID_STR                            0x00 
#define  USBD_IDX_MFC_STR                               0x01 
#define  USBD_IDX_PRODUCT_STR                           0x02
#define  USBD_IDX_SERIAL_STR                            0x03 
#define  USBD_IDX_CONFIG_STR                            0x04 
#define  USBD_IDX_INTERFACE_STR                         0x05 
#define USBD_BULK_SIZE                                  64
#define DFU_INTERFACE_NUMBER                            0x01

#define         DEVICE_ID1          (UID_BASE) //(0x1FFF7A10)
#define         DEVICE_ID2          (UID_BASE + 4) 
#define         DEVICE_ID3          (UID_BASE + 8) 

#define USBD_VID     0x3293 // Unhuman VID
#define USBD_PID     0x0100

static const uint8_t USB_DEVICE_DESCIPTOR[]=
{
  0x12,                       /*bLength */
  USB_DESC_TYPE_DEVICE,       /*bDescriptorType*/
  0x00,                       /*bcdUSB */
  0x02,
  0xFF,                       /*bDeviceClass*/
  0x00,                       /*bDeviceSubClass*/
  0x00,                       /*bDeviceProtocol*/
  64,           /*bMaxPacketSize*/
  LOBYTE(USBD_VID),           /*idVendor*/
  HIBYTE(USBD_VID),           /*idVendor*/
  LOBYTE(USBD_PID),        /*idProduct*/
  HIBYTE(USBD_PID),        /*idProduct*/
  0x00,                       /*bcdDevice rel. 2.00*/
  0x02,
  USBD_IDX_MFC_STR,           /*Index of manufacturer  string*/
  USBD_IDX_PRODUCT_STR,       /*Index of product string*/
  USBD_IDX_SERIAL_STR,        /*Index of serial number string*/
  1  /*bNumConfigurations*/
};

static const uint8_t USB_CONFIGURATION_DESCRIPTOR[] =
{
  /*Configuration Descriptor*/
  0x09,   /* bLength: Configuration Descriptor size */
  USB_DESC_TYPE_CONFIGURATION,      /* bDescriptorType: Configuration */
  9+9+7+7+7+7+9+9,                /* wTotalLength:no of returned bytes */
  0x00,
  0x02,   /* bNumInterfaces: 2 interface */
  0x01,   /* bConfigurationValue: Configuration value */
  0x04,   /* iConfiguration: Index of string descriptor describing the configuration */
  0xC0,   /* bmAttributes: self powered */
  0x32,   /* MaxPower 100 mA */
  
  /*---------------------------------------------------------------------------*/
  
  /*Interface Descriptor */
  0x09,   /* bLength: Interface Descriptor size */
  USB_DESC_TYPE_INTERFACE,  /* bDescriptorType: Interface */
  /* Interface descriptor type */
  0x00,   /* bInterfaceNumber: Number of Interface */
  0x00,   /* bAlternateSetting: Alternate setting */
  0x04,   /* bNumEndpoints: 4 endpoints used */
  0x00,   /* bInterfaceClass: 0 */
  0x00,   /* bInterfaceSubClass: 0 */
  0x00,   /* bInterfaceProtocol:0 */
  0x05,   /* iInterface: */
  
  // realtime interface
  /*Endpoint 2 Descriptor*/
  0x07,                           /* bLength: Endpoint Descriptor size */
  USB_DESC_TYPE_ENDPOINT,   /* bDescriptorType: Endpoint */
  0x82,                     /* bEndpointAddress */
  0x02,                           /* bmAttributes: Bulk */
  LOBYTE(USBD_BULK_SIZE),     /* wMaxPacketSize: */
  HIBYTE(USBD_BULK_SIZE),
  0x0,                           /* bInterval: */ 

    /*Endpoint 2 Descriptor*/
  0x07,                           /* bLength: Endpoint Descriptor size */
  USB_DESC_TYPE_ENDPOINT,   /* bDescriptorType: Endpoint */
  2,                     /* bEndpointAddress */
  0x02,                           /* bmAttributes: Bulk */
  LOBYTE(USBD_BULK_SIZE),     /* wMaxPacketSize: */
  HIBYTE(USBD_BULK_SIZE),
  0x0,                           /* bInterval: */ 
  /*---------------------------------------------------------------------------*/
  
  // text interface
  /*Endpoint 1 Descriptor*/
  0x07,                           /* bLength: Endpoint Descriptor size */
  USB_DESC_TYPE_ENDPOINT,   /* bDescriptorType: Endpoint */
  0x81,                     /* bEndpointAddress */
  0x02,                           /* bmAttributes: Bulk */
  LOBYTE(USBD_BULK_SIZE),     /* wMaxPacketSize: */
  HIBYTE(USBD_BULK_SIZE),
  0x10,                           /* bInterval: */ 

    /*Endpoint 1 Descriptor*/
  0x07,                           /* bLength: Endpoint Descriptor size */
  USB_DESC_TYPE_ENDPOINT,   /* bDescriptorType: Endpoint */
  0x01,                     /* bEndpointAddress */
  0x02,                           /* bmAttributes: Bulk */
  LOBYTE(USBD_BULK_SIZE),     /* wMaxPacketSize: */
  HIBYTE(USBD_BULK_SIZE),
  0x10,                           /* bInterval: */ 
  /*---------------------------------------------------------------------------*/

// DFU taken from the st dfu mode descriptor change interface protocol 2 to 1
  0x09,
  USB_DESC_TYPE_INTERFACE,
  DFU_INTERFACE_NUMBER,
  0x00,
  0x00,
  0xfe,
  0x01,
  0x01,
  0x06,

  0x09,
  0x21,
  0x0b, 
  0xff,
  0x00,
  0x00,
  0x08,
  0x1a,
  0x01
};

struct usb_control_request {
    uint8_t bRequestType;
    uint8_t bRequest;
    uint16_t wValue;    // little endian
    uint16_t wIndex;
    uint16_t wLength;
} __attribute__ ((packed));

static void read_pma(uint8_t byte_count, uint16_t * pma_address, uint8_t *buffer_out);

static void _send_data(uint8_t endpoint, const uint8_t *data, uint8_t length);

bool USB::tx_active(uint8_t endpoint) {
    return USBEPR->EP[endpoint].STAT_TX == static_cast<uint32_t>(EP_STAT::VALID);
}

USB::USB() : regs_{*USB_FS_Device} {
    regs_.CNTR_b = decltype(USB_FS_device_Type::CNTR_b) { .L1REQM = 1, .RESETM = 1, .WKUPM = 1, .ERRM = 1, .CTRM = 1 };
}

void USB::connect() {
   // regs_.BCDR_b = 1; // device pull up
}

// true if successful cancel
bool USB::cancel_transfer(uint8_t endpoint, uint32_t timeout_ns) {
    // set NAK but it doesn't cancel any transfer in progress right now
    uint16_t nak_count = 0;
    while(USBEPR->EP[endpoint].STAT_TX != static_cast<uint32_t>(EP_STAT::NAK)) {
        epr_set_stat_tx(endpoint, EP_STAT::NAK);
        nak_count++;
        if (nak_count > 100) {
            // give up
            break;
        }
    }

    // wait for any current transfer to complete, checking for activity on an EXTI pin 
    // and checking for CTR_TX
    // timeout of 5000 ns
   // EXTI->PR1 = EXTI_PR1_PIF10;
    uint32_t t_start = get_clock();
    uint16_t idle_count = 0;
    while((get_clock() - t_start) < timeout_ns/(uint16_t) (1e9/CPU_FREQUENCY_HZ)) {
        // if (EXTI->PR1 & EXTI_PR1_PIF10) {
        //     EXTI->PR1 = EXTI_PR1_PIF10;
        //     idle_count = 0;
        // } else {
            idle_count++;
        //}
        if (idle_count > 3) { 
            // 3 gives about 2 us right now, usb pins must transition within 7 bits/.58 us
            return true;
        }
        if (USBEPR->EP[endpoint].CTR_TX) {
            return true;
        }
    }
    return false;
    // after making it through the endpoint may still be active (NAK was set, but a completed 
    // tranfer will toggle a bit to thus reenable TX_VALID) so return to while(tx_active(endpoint)) 
}

// Wait will pause until last packet has been received, If wait is false, then a buffered packet
// will be discarded. For wait being false the maximum transmission is USBD_BULK_SIZE (64) bytes.
void USB::send_data(uint8_t endpoint, const uint8_t *data, uint16_t length, bool wait, uint32_t wait_timeout_us) {
    auto t_start_wait = get_clock();
    while (tx_active(endpoint)) {
        if (wait && get_clock() - t_start_wait <= US_TO_CPU(wait_timeout_us)) {
            // it wil force cancel on timeout
            continue;
        } else {
            bool successful_cancel = cancel_transfer(endpoint);
            if (!successful_cancel && wait) {
                continue;
            } else {
                return;
            }
        }
    }
    
    if (wait && (length >= USBD_BULK_SIZE)) {
        _send_data(endpoint, data, USBD_BULK_SIZE);
        send_data(endpoint, data+USBD_BULK_SIZE, length-USBD_BULK_SIZE, wait, wait_timeout_us);
    } else {
        _send_data(endpoint, data, length);
    }
}

void USB::_send_data(uint8_t endpoint, const uint8_t *data, uint8_t length) {
    uint8_t length16 = (length+1)>>1;
    volatile uint16_t * pma_address = USBPMA->buffer[endpoint].EP_TX;
    for(int i=0; i<length16; i++) {
        pma_address[i] = ((const uint16_t *) data)[i];
    }
    USBPMA->btable[endpoint].COUNT_TX = length;
    epr_set_stat_tx(endpoint, EP_STAT::VALID);
}

// todo protect
int USB::receive_data(uint8_t endpoint, uint8_t * const data, uint8_t length) {
    if (new_rx_data_[endpoint]) {
        new_rx_data_[endpoint] = false;
        asm("nop"); // addresses unknown bug
        length = std::min(length,count_rx_[endpoint]);
        for (int i=0; i<length; i++) {
            data[i] = rx_buffer_[endpoint][i];
        }
        return length;
    } else {
        return 0;
    }
}

void USB::send_string(uint8_t endpoint, const char *str, uint8_t length) {
    uint16_t str_out[length+1];
    uint8_t length_total = 2 + 2*length;
    str_out[0] = length_total | (3 << 8); // header
    for (int i=0; i<length; i++) {
        str_out[i+1] = str[i];
    }
    send_data(endpoint, reinterpret_cast<const uint8_t *>(str_out), length_total);
}

void USB::send_stall(uint8_t endpoint) {
    epr_set_stat_tx(endpoint, EP_STAT::STALL);
}

void read_pma(uint8_t byte_count, volatile uint16_t * pma_address, uint8_t *buffer_out) {
    int count_received_16 = (byte_count + 1) >> 1;
    for(int i=0; i<count_received_16; i++) {
        ((uint16_t *) buffer_out)[i] = pma_address[i];
    }
}

void USB::epr_set_stat_rx(uint8_t endpoint, EP_STAT stat, bool toggle) {
    EPReg epr { USBEPR->EP[endpoint] };
    epr.STAT_RX ^= static_cast<uint32_t>(stat);
    epr.STAT_TX = 0; // don't toggle
    epr.CTR_TX = 1; // write 1 to not clear CTR
    epr.CTR_RX = 1;
    epr.DTOG_RX ^= toggle;
    USBEPR->EPR[endpoint]= epr.word;
}

void USB::epr_set_stat_tx(uint8_t endpoint, EP_STAT stat, bool toggle) {
    EPReg epr { USBEPR->EP[endpoint] };
    epr.STAT_TX ^= static_cast<uint32_t>(stat);
    epr.STAT_RX = 0; // don't toggle
    epr.CTR_TX = 1; // write 1 to not clear CTR
    epr.CTR_RX = 1;
    epr.DTOG_RX ^= toggle;
    USBEPR->EPR[endpoint] = epr.word;
}

void USB::interrupt() {
    // Reading regs_.ISTR seems to take many clock cyles. Read just once where possible
    auto istr = regs_.ISTR_b;
    /* Handle Reset Interrupt */
    if (istr.RESET)
    {
        reset_count_++;
        error_count_ = 0;
        // todo bring back logger in isr safe way
        // logger.log("usb reset");
        // Set up endpoint 0
        EPReg epr;
        epr.EP_TYPE = static_cast<uint32_t>(EP_TYPE::CONTROL);
        regs_.EP0R = std::bit_cast<decltype(regs_.EP0R)>(epr);
        USBPMA->btable[0].ADDR_TX = offsetof(USBPMA_TypeDef, buffer[0].EP_TX);
        epr_set_stat_tx(0, EP_STAT::NAK, true);
            // sets the toggle only bits to DIS and clears DTOG, hardware better not change EPR during operation
        USBPMA->btable[0].ADDR_RX = offsetof(USBPMA_TypeDef, buffer[0].EP_RX);
        USBPMA->btable[0].COUNT_RX =  (1 << USB_COUNT_RX_BLSIZE_Pos) | (2 << USB_COUNT_RX_NUM_BLOCK_Pos); // 1:2 -> 96 byte allocation
        epr_set_stat_rx(0, EP_STAT::VALID, true);
            // similar to above TX

        // enable interrupts

        regs_.DADDR_b.EF = 1; // device address 0 is effective

        regs_.ISTR_b.RESET = 0;
    }

    // Endpoint correct transfer interrupt
    if(istr.CTR)
    {
        switch (istr.EP_ID) {
            case 0:
                if (istr.DIR) { // RX
                    if (regs_.EP0R_b.SETUP) {
                        uint8_t buffer[64];
                        uint8_t byte_count = USBPMA->btable[0].COUNT_RX & USB_COUNT_RX_COUNT_RX;
                        read_pma(byte_count, USBPMA->buffer[0].EP_RX, buffer);
                        handle_setup_packet(reinterpret_cast<usb_control_request *>(buffer));
                    }
                    // clear CTR
                    regs_.EP0R_b.CTR_RX = 0; // = (USB_EP_CTR_TX | (regs_.EP0R & USB_EPREG_MASK)) & ~USB_EP_CTR_RX;
                    // renable rx on ep0
                    epr_set_stat_rx(0, EP_STAT::VALID);
                }
                if (regs_.EP0R_b.CTR_TX) {
                    // clear CTR_TX
                    regs_.EP0R_b.CTR_TX = 0;
                }
                break;
            case 2:
                if (istr.DIR) { // RX
                    // clear CTR_RX
                    regs_.EP2R_b.CTR_RX = 0;
                    count_rx_[2] = (USBPMA->btable[2].COUNT_RX & USB_COUNT_RX_COUNT_RX);
                    read_pma(count_rx_[2], USBPMA->buffer[2].EP_RX, rx_buffer_[2]);
                    new_rx_data_[2] = true;
                    epr_set_stat_rx(2, EP_STAT::VALID);
                }
                if (regs_.EP2R_b.CTR_TX) {
                    tx_data_ack_[2] = true;
                    // clear CTR_TX
                    regs_.EP2R_b.CTR_TX = 0;
                }
                break;
            case 1:
                if (istr.DIR) { // RX
                    // clear CTR_RX
                    regs_.EP1R_b.CTR_RX = 0;
                    count_rx_[1] = (USBPMA->btable[1].COUNT_RX & USB_COUNT_RX_COUNT_RX);
                    read_pma(count_rx_[1], USBPMA->buffer[1].EP_RX, rx_buffer_[1]);
                    new_rx_data_[1] = true;
                    epr_set_stat_rx(1, EP_STAT::VALID);
                }
                if (regs_.EP1R_b.CTR_TX) {
                    tx_data_ack_[1] = true;
                     // clear CTR_TX
                    regs_.EP1R_b.CTR_TX = 0;
                }
                break;
        }
    }

    if (istr.ERR) {
        error_count_++;
        regs_.ISTR_b.ERR = 0;
    }

    // clear anything remaining
    //regs_.ISTR = 0;
}

 void USB::handle_setup_packet(usb_control_request *setup_data) {
    switch(setup_data->bRequestType) {
        case 0x80:  // standard request get
            switch (setup_data->bRequest) {
                case 0x00:  // get status
                    send_data(0, reinterpret_cast<const uint8_t *>("\x0\x0"), 2);  // not self powered or remote wakeup
                    break;
                case 0x06:  // get descriptor
                    switch (setup_data->wValue >> 8) {
                        case 0x01:   // device descriptor
                            send_data(0, USB_DEVICE_DESCIPTOR, std::min(static_cast<size_t>(setup_data->wLength),sizeof(USB_DEVICE_DESCIPTOR)));
                            break;
                        case 0x02:   // configuration descriptor
                            send_data(0, USB_CONFIGURATION_DESCRIPTOR, std::min(static_cast<size_t>(setup_data->wLength),sizeof(USB_CONFIGURATION_DESCRIPTOR)));
                            break;
                        case 0x03:  // string descriptor
                            switch (setup_data->wValue & 0xFF) {
                                case 0x00: // language descriptor
                                    send_data(0, reinterpret_cast<const uint8_t *>("\x4\x3\x9\x4"), 4); // english
                                    break;
                                // case 0x01:
                                //     send_string(0, MANUFACTURER_STRING, std::strlen(MANUFACTURER_STRING));
                                //     break;
                                // case 0x02:
                                //     send_string(0, PRODUCT_STRING, std::strlen(PRODUCT_STRING));
                                //     break;
                                // case 0x03:
                                // { 
                                //     send_string(0, get_serial_number(), std::strlen(get_serial_number()));
                                //     break;
                                // }
                                // case 0x04:
                                //     send_string(0, OBOT_VERSION " " BUILD_DATETIME, std::strlen(OBOT_VERSION " " BUILD_DATETIME));
                                //     break;
                                // case 0x05:
                                //     send_string(0, const_cast<const char*>(name), std::strlen(const_cast<const char*>(name)));
                                //     break;
                                case 0x06:
                                    send_string(0, "ST DFU mode", std::strlen("ST DFU mode"));
                                    break;
                                default:
                                    send_string(0, "default", std::strlen("default"));
                                    break;
                            }
                            break;
                        default: 
                            send_stall(0);
                            break;
                    }
                    break;
                default:
                    send_stall(0);
                    break;
            }
            break;
        case 0x00:  // standard request set
            switch (setup_data->bRequest) {
                case 0x05:  // set address
                    device_address_ = setup_data->wValue;
                    send_data(0,0,0);
                    // set device address after acknowledge
                    while(regs_.EP0R_b.STAT_TX == 3); // wait for packet to go through (3 == ep tx valid)
                    regs_.DADDR_b.ADD = device_address_;
                    break;
                case 0x09: // set configuration
                    // enable endpoint 2 IN (TX)
                    regs_.EP2R = 0x002; // Bulk on 2 
                    USBPMA->btable[2].ADDR_TX = offsetof(USBPMA_TypeDef, buffer[2].EP_TX);
                    epr_set_stat_tx(2, EP_STAT::NAK);
                    // sets the toggle only bits to NAK, hardware better not change EPR during operation
                    
                    regs_.EP2R = 2;
                    // enable endpoint 2 OUT (RX)
                    USBPMA->btable[2].ADDR_RX = offsetof(USBPMA_TypeDef, buffer[2].EP_RX);
                    USBPMA->btable[2].COUNT_RX = (1 << USB_COUNT_RX_BLSIZE_Pos) | (2 << USB_COUNT_RX_NUM_BLOCK_Pos); // 1:2 -> 96 byte allocation
                    epr_set_stat_rx(2, EP_STAT::VALID); // as above with TX

                    // enable endpoint 1 IN (TX)
                    regs_.EP1R = 1; // Bulk on 1
                    USBPMA->btable[1].ADDR_TX = offsetof(USBPMA_TypeDef, buffer[1].EP_TX);
                    epr_set_stat_tx(1, EP_STAT::NAK);
                        // sets the toggle only bits to NAK, hardware better not change EPR during operation
                    
                    // enable endpoint 2 OUT (RX)
                    USBPMA->btable[1].ADDR_RX = offsetof(USBPMA_TypeDef, buffer[1].EP_RX);
                    USBPMA->btable[1].COUNT_RX = (1 << USB_COUNT_RX_BLSIZE_Pos) | (2 << USB_COUNT_RX_NUM_BLOCK_Pos); // 1:2 -> 96 byte allocation
                    epr_set_stat_rx(1, EP_STAT::VALID);
                    
                    // setup status phase    
                    send_data(0,0,0);
                    break;
                default:
                    send_stall(0);
                    break;
            }
            break;
        case 0x01:  // interface request set
            if (setup_data->bRequest == 11) { // set inteface request
                interface_ = setup_data->wIndex;
                send_data(0,0,0);
            } else {
                send_stall(0);
            }
            break;
        case 0xa1:  // interface class get
            if ((setup_data->bRequest == 3) && (interface_ == DFU_INTERFACE_NUMBER)) { // dfu get_status
                send_data(0,reinterpret_cast<const uint8_t *>("\x00\x00\x00\x00\x00\x00"), 6);
            } else {
                send_stall(0);
            }
            break;
        case 0x21:  // interface class request
            if ((setup_data->bRequest == 0) && (interface_ == DFU_INTERFACE_NUMBER)) { // dfu detach
                send_data(0,0,0);
                while (regs_.EP0R_b.STAT_TX == 3); // wait for packet to go through (3 == ep tx valid)
                cpu::wait_ms(10);
                go_to_bootloader = 0xB007;
                NVIC_SystemReset();
            } else {
                send_stall(0);
            }
            break;
        default:
            send_stall(0);
            break;
    }
}
