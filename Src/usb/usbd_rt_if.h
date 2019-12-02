#ifndef __USBD_CDC_IF_H__
#define __USBD_CDC_IF_H__

#ifdef __cplusplus
 extern "C" {
#endif

#include "usbd_rt.h"


uint8_t USB_send(uint8_t* Buf, uint16_t Len);


#ifdef __cplusplus
}
#endif

#endif 