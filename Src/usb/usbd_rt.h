#ifndef __USB_CDC_H
#define __USB_CDC_H

#ifdef __cplusplus
extern "C" {
#endif


#include  "usbd_ioreq.h"


#define CDC_IN_EP                                   0x81U  /* EP1 for data IN */
#define CDC_OUT_EP                                  0x01U  /* EP1 for data OUT */



#define CDC_DATA_FS_MAX_PACKET_SIZE                 64U  /* Endpoint IN & OUT Packet size */


#define USB_CDC_CONFIG_DESC_SIZ                     (9+9+7+7)


#define CDC_DATA_FS_IN_PACKET_SIZE                  CDC_DATA_FS_MAX_PACKET_SIZE
#define CDC_DATA_FS_OUT_PACKET_SIZE                 CDC_DATA_FS_MAX_PACKET_SIZE

extern USBD_ClassTypeDef  USBD_RT;

void usb_send(USBD_HandleTypeDef *pdev, uint8_t *pbuff, uint16_t length);

#ifdef __cplusplus
}
#endif

#endif  