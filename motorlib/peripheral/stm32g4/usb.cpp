#include "../usb.h"
#include <algorithm>
#include <cstring>
#include "../../param.h"
#include "../../otp.h"
#include "../../../version.h"

#include "stm32g4xx.h"
#include "../../util.h"

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

#define USBD_VID     1155

static const uint8_t USB_DEVICE_DESCIPTOR[]=
{
  0x12,                       /*bLength */
  USB_DESC_TYPE_DEVICE,       /*bDescriptorType*/
  0x00,                       /*bcdUSB */
  0x02,
  0xFF,                       /*bDeviceClass*/
  0x02,                       /*bDeviceSubClass*/
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
  9+9+7+7+9+9,                /* wTotalLength:no of returned bytes */
  0x00,
  0x02,   /* bNumInterfaces: 2 interface */
  0x01,   /* bConfigurationValue: Configuration value */
  0x04,   /* iConfiguration: Index of string descriptor describing the configuration */
  0xC0,   /* bmAttributes: self powered */
  0x32,   /* MaxPower 0 mA */
  
  /*---------------------------------------------------------------------------*/
  
  /*Interface Descriptor */
  0x09,   /* bLength: Interface Descriptor size */
  USB_DESC_TYPE_INTERFACE,  /* bDescriptorType: Interface */
  /* Interface descriptor type */
  0x00,   /* bInterfaceNumber: Number of Interface */
  0x00,   /* bAlternateSetting: Alternate setting */
  0x02,   /* bNumEndpoints: One endpoints used */
  0x00,   /* bInterfaceClass: Communication Interface Class */
  0x00,   /* bInterfaceSubClass: Abstract Control Model */
  0x00,   /* bInterfaceProtocol: Common AT commands */
  0x05,   /* iInterface: */
  
  /*Endpoint 2 Descriptor*/
  0x07,                           /* bLength: Endpoint Descriptor size */
  USB_DESC_TYPE_ENDPOINT,   /* bDescriptorType: Endpoint */
  0x82,                     /* bEndpointAddress */
  0x02,                           /* bmAttributes: Interrupt */
  LOBYTE(USBD_BULK_SIZE),     /* wMaxPacketSize: */
  HIBYTE(USBD_BULK_SIZE),
  0x10,                           /* bInterval: */ 

    /*Endpoint 2 Descriptor*/
  0x07,                           /* bLength: Endpoint Descriptor size */
  USB_DESC_TYPE_ENDPOINT,   /* bDescriptorType: Endpoint */
  2,                     /* bEndpointAddress */
  0x02,                           /* bmAttributes: Interrupt */
  LOBYTE(USBD_BULK_SIZE),     /* wMaxPacketSize: */
  HIBYTE(USBD_BULK_SIZE),
  0x10,                           /* bInterval: */ 
  /*---------------------------------------------------------------------------*/

// DFU taken from the st dfu mode descriptor change interface protocol 2 to 1
  0x09,
  0x04,
  0x01,
  0x00,
  0x00,
  0xfe,
  0x01,
  0x01,
  0x00,

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

// special function due to difficulty of toggle bits and clear bits;
// hopefully hardware doesn't change values during this function
static void epr_set_toggle(uint8_t endpoint, uint16_t set_bits, uint16_t set_mask);

static void read_pma(uint8_t byte_count, __IO uint16_t * pma_address, uint8_t *buffer_out);

static void _send_data(uint8_t endpoint, const uint8_t *data, uint8_t length);

// Wait will pause until last packet has been received, If wait is false, then a buffered packet
// will be discarded. For wait being false the maximum transmission is USBD_BULK_SIZE (64) bytes.
void USB1::send_data(uint8_t endpoint, const uint8_t *data, uint8_t length, bool wait) {
    while ((USBEPR->EP[endpoint].EPR & USB_EPTX_STAT) == USB_EP_TX_VALID) {
        if (wait) {
            continue;
        } else {
            epr_set_toggle(endpoint, USB_EP_TX_NAK, USB_EPTX_STAT);
        }
    }
    
    if (wait && (length > USBD_BULK_SIZE)) {
        _send_data(endpoint, data, USBD_BULK_SIZE);
        send_data(endpoint, data+USBD_BULK_SIZE, length-USBD_BULK_SIZE);
    } else {
        _send_data(endpoint, data, length);
    }
}

void _send_data(uint8_t endpoint, const uint8_t *data, uint8_t length) {
    uint8_t length16 = (length+1)>>1;
    __IO uint16_t * pma_address = USBPMA->buffer[endpoint].EP_TX;
    for(int i=0; i<length16; i++) {
        pma_address[i] = ((const uint16_t *) data)[i];
    }
    USBPMA->btable[endpoint].COUNT_TX = length;
    epr_set_toggle(endpoint, USB_EP_TX_VALID, USB_EPTX_STAT);
}

// todo protect
int USB1::receive_data(uint8_t endpoint, uint8_t * const data, uint8_t length) {
    if (new_rx_data_) {
        new_rx_data_ = false;
        length = std::min(length,count_rx_);
        for (int i=0; i<length; i++) {
            data[i] = rx_buffer_[i];
        }
        return length;
    } else {
        return 0;
    }
}

void USB1::send_string(uint8_t endpoint, const char *str, uint8_t length) {
    uint16_t str_out[length+1] = {};
    uint8_t length_total = 2 + 2*length;
    str_out[0] = length_total | (3 << 8); // header
    for (int i=0; i<length; i++) {
        str_out[i+1] = str[i];
    }
    send_data(endpoint, reinterpret_cast<const uint8_t *>(str_out), length_total);
}

void USB1::send_stall(uint8_t endpoint) {
    epr_set_toggle(endpoint, USB_EP_TX_STALL, USB_EPTX_STAT);
}

void read_pma(uint8_t byte_count, __IO uint16_t * pma_address, uint8_t *buffer_out) {
    int count_received_16 = (byte_count + 1) >> 1;
    for(int i=0; i<count_received_16; i++) {
        ((uint16_t *) buffer_out)[i] = pma_address[i];
    }
}

void epr_set_toggle(uint8_t endpoint, uint16_t set_bits, uint16_t set_mask) {
    uint16_t epr = USBEPR->EP[endpoint].EPR;
    uint16_t epr_toggle = (epr & set_mask) ^ set_bits;
    uint16_t epr_normal = epr & USB_EPREG_MASK;
    uint16_t epr_total = epr_normal | epr_toggle | USB_EP_CTR_TX | USB_EP_CTR_RX; // always write 1 to not clear CTR
    USBEPR->EP[endpoint].EPR = epr_total;
}

static void Get_SerialNum(void)
{
  uint32_t deviceserial0, deviceserial1, deviceserial2;

  //deviceserial0 = *(uint32_t *)DEVICE_ID1;
  //deviceserial1 = *(uint32_t *)DEVICE_ID2;
  //deviceserial2 = *(uint32_t *)DEVICE_ID3;

  deviceserial0 += deviceserial2;

//   if (deviceserial0 != 0U)
//   {
//     IntToUnicode(deviceserial0, &USBD_StringSerial[2], 8U);
//     IntToUnicode(deviceserial1, &USBD_StringSerial[18], 4U);
//   }
}

/**
  * @brief  Convert Hex 32Bits value into char
  * @param  value: value to convert
  * @param  pbuf: pointer to the buffer
  * @param  len: buffer length
  * @retval None
  */
static void IntToUnicode(uint32_t value, uint8_t *pbuf, uint8_t len)
{
  uint8_t idx = 0U;

  for (idx = 0U ; idx < len ; idx ++)
  {
    if (((value >> 28)) < 0xAU)
    {
      pbuf[ 2U * idx] = (value >> 28) + '0';
    }
    else
    {
      pbuf[2U * idx] = (value >> 28) + 'A' - 10U;
    }

    value = value << 4;

    pbuf[2U * idx + 1] = 0U;
  }
}

void USB1::interrupt() {
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

    // clear anything remaining
    USB->ISTR = 0;
}

 void USB1::handle_setup_packet(uint8_t *setup_data) {
    switch(setup_data[0]) {
        case 0x80:  // standard request get
            switch (setup_data[1]) {
                case 0x00:  // get status
                    send_data(0, reinterpret_cast<const uint8_t *>("\x1\x0"), 2);  // self powered
                    break;
                case 0x06:  // get descriptor
                    switch (setup_data[3]) {
                        case 0x01:   // device descriptor
                            send_data(0, USB_DEVICE_DESCIPTOR, std::min(static_cast<size_t>(setup_data[6]),sizeof(USB_DEVICE_DESCIPTOR)));
                            break;
                        case 0x02:   // configuration descriptor
                            send_data(0, USB_CONFIGURATION_DESCRIPTOR, std::min(static_cast<size_t>(setup_data[6]),sizeof(USB_CONFIGURATION_DESCRIPTOR)));
                            break;
                        case 0x03:  // string descriptor
                            switch (setup_data[2]) {
                                case 0x00: // language descriptor
                                    send_data(0, reinterpret_cast<const uint8_t *>("\x4\x3\x9\x4"), 4); // english
                                    break;
                                case 0x01:
                                    send_string(0, MANUFACTURER_STRING, std::strlen(MANUFACTURER_STRING));
                                    break;
                                case 0x02:
                                    send_string(0, PRODUCT_STRING, std::strlen(PRODUCT_STRING));
                                    break;
                                case 0x03:
                                    Get_SerialNum();
                                    send_string(0, "abc", std::strlen("abc"));
                                    break;
                                case 0x04:
                                    send_string(0, "abc", std::strlen("abc"));
                                  //  send_string(0, VERSION " " GIT_HASH " " BUILD_DATETIME, std::strlen(VERSION " " GIT_HASH " " BUILD_DATETIME));
                                    break;
                                case 0x05:
                                    send_string(0, param()->name, std::strlen(param()->name));
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
            switch (setup_data[1]) {
                case 0x05:  // set address
                    device_address_ = setup_data[2];
                    send_data(0,0,0);
                    // set device address after acknowledge
                    while((USB->EP0R & USB_EPTX_STAT) == USB_EP_TX_VALID);
                    USB->DADDR &= ~USB_DADDR_ADD; 
                    USB->DADDR |= device_address_;
                    break;
                case 0x09: // set configuration
                    // enable endpoint 2 IN (TX)
                    USB->EP2R = 2; // Bulk on 2
                    USBPMA->btable[2].ADDR_TX = offsetof(USBPMA_TypeDef, buffer[2].EP_TX);
                    epr_set_toggle(2, USB_EP_TX_NAK, USB_EPTX_STAT);
                        // sets the toggle only bits to NAK, hardware better not change EPR during operation
                    
                    // enable endpoint 2 OUT (RX)
                    USBPMA->btable[2].ADDR_RX = offsetof(USBPMA_TypeDef, buffer[2].EP_RX);
                    USBPMA->btable[2].COUNT_RX = (1 << USB_COUNT2_RX_BLSIZE_Pos) | (2 << USB_COUNT2_RX_NUM_BLOCK_Pos); // 1:2 -> 96 byte allocation
                    epr_set_toggle(2, USB_EP_RX_VALID, USB_EPRX_STAT); // as above with TX
                    
                    // setup status phase    
                    send_data(0,0,0);
                    break;
                default:
                    send_stall(0);
                    break;
            }
            break;
        case 0x01:  // interface request set
            if (setup_data[1] == 11) { // set inteface request
                interface_ = setup_data[4];
                send_data(0,0,0);
            } else {
                send_stall(0);
            }
            break;
        case 0xa1:  // interface class get
            if ((setup_data[1] == 3) && (interface_ == 1)) { // dfu get_status
                send_data(0,reinterpret_cast<const uint8_t *>("\x00\x00\x00\x00\x00\x00"), 6);
            } else {
                send_stall(0);
            }
            break;
        case 0x21:  // interface class request
            if ((setup_data[1] == 0) && (interface_ == 1)) { // dfu detach
                send_data(0,0,0);
                while ((USB->EP0R & USB_EPTX_STAT) == USB_EP_TX_VALID); // wait for packet to go through
                ms_delay(10);
                go_to_bootloader = 0xB0;
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
    