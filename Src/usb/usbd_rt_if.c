#include "usbd_rt_if.h"

extern USBD_HandleTypeDef hUsbDeviceFS;


uint8_t USB_send(uint8_t* Buf, uint16_t Len)
{
  uint8_t result = USBD_OK;

  usb_send(&hUsbDeviceFS, Buf, Len);

  return result;
}
