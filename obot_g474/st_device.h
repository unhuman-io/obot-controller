#ifndef ST_DEVICE_H
#define ST_DEVICE_H

#include "stm32g474xx.h"
//#include "stm32g4xx_hal.h"
#define CPU_FREQUENCY_HZ    170e6

#define FLASH_OTP_BASE      0x1FFF7000

// from HAL
#define DMA_REQUEST_SPI1_RX           10U
#define DMA_REQUEST_SPI1_TX           11U
#define DMA_REQUEST_SPI2_RX           12U
#define DMA_REQUEST_SPI2_TX           13U
#define DMA_REQUEST_SPI3_RX           14U
#define DMA_REQUEST_SPI3_TX           15U

#define DMA_REQUEST_I2C1_RX           16U
#define DMA_REQUEST_I2C1_TX           17U
#define DMA_REQUEST_I2C2_RX           18U
#define DMA_REQUEST_I2C2_TX           19U
#define DMA_REQUEST_I2C3_RX           20U
#define DMA_REQUEST_I2C3_TX           21U
#define DMA_REQUEST_I2C4_RX           22U
#define DMA_REQUEST_I2C4_TX           23U

#define DMA_REQUEST_USART1_RX         24U
#define DMA_REQUEST_USART1_TX         25U
#define DMA_REQUEST_USART2_RX         26U
#define DMA_REQUEST_USART2_TX         27U
#define DMA_REQUEST_USART3_RX         28U
#define DMA_REQUEST_USART3_TX         29U

#define DMA_REQUEST_UART4_RX          30U
#define DMA_REQUEST_UART4_TX          31U
#define DMA_REQUEST_UART5_RX          32U
#define DMA_REQUEST_UART5_TX          33U

#define DMA_REQUEST_LPUART1_RX        34U
#define DMA_REQUEST_LPUART1_TX        35U

#define VREFINT_CAL_ADDR                   ((uint16_t*) (0x1FFF75AAUL)) /* Internal voltage reference, address of parameter VREFINT_CAL: VrefInt ADC raw data acquired at temperature 30 DegC (tolerance: +-5 DegC), Vref+ = 3.0 V (tolerance: +-10 mV). */
#define VREFINT_CAL_VREF                   (3000UL)                     /* Analog voltage reference (Vref+) value with which temperature sensor has been calibrated in production (tolerance: +-10 mV) (unit: mV). */



#endif
