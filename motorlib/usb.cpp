#include "usb.h"
#include <algorithm>
#include <cstring>


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

#define USBD_VID     1155
#define USBD_PID_FS     (22336+1)

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
  LOBYTE(USBD_PID_FS),        /*idProduct*/
  HIBYTE(USBD_PID_FS),        /*idProduct*/
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
  2,                     /* bEndpointAddress */
  0x02,                           /* bmAttributes: Interrupt */
  LOBYTE(64),     /* wMaxPacketSize: */
  HIBYTE(64),
  0x10,                           /* bInterval: */ 

    /*Endpoint 2 Descriptor*/
  0x07,                           /* bLength: Endpoint Descriptor size */
  USB_DESC_TYPE_ENDPOINT,   /* bDescriptorType: Endpoint */
  2,                     /* bEndpointAddress */
  0x02,                           /* bmAttributes: Interrupt */
  LOBYTE(64),     /* wMaxPacketSize: */
  HIBYTE(64),
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
                    USB->DADDR &= ~USB_DADDR_ADD; 
                    USB->DADDR |= device_address_;
                    send_data(0,0,0); // core seems to know to still send this as address 0, todo check
                    break;
                case 0x09: // set configuration
                    // enable endpoint 2 IN (TX)
                    USB->EP2R = 2; // Bulk on 2
                    USBPMA->btable[2].ADDR_TX = offsetof(USBPMA_TypeDef, buffer[2].EP_TX);
                    epr_set_toggle(2, USB_EP_TX_NAK, USB_EPTX_STAT);
                        // sets the toggle only bits to NAK, hardware better not change EPR during operation
                    // tx interrupt ?
                    
                    // enable endpoint 2 OUT (RX)
                    USBPMA->btable[2].ADDR_RX = offsetof(USBPMA_TypeDef, buffer[2].EP_RX);
                    USBPMA->btable[2].COUNT_RX = (1 << USB_COUNT2_RX_BLSIZE_Pos) | (2 << USB_COUNT2_RX_NUM_BLOCK_Pos); // 1:2 -> 96 byte allocation
                    epr_set_toggle(2, USB_EP_RX_VALID, USB_EPRX_STAT); // as above with TX
                    // rx interrupt?
                    
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
                go_to_bootloader = true;
                send_data(0,0,0);
            } else {
                send_stall(0);
            }
            break;
        default:
            send_stall(0);
            break;
    }
}
    