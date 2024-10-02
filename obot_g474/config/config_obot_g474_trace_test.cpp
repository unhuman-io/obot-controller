#include "st_device.h"
#include <fcntl.h>
#include <cstring>
#include "../../motorlib/logger.h"
#include "../../motorlib/peripheral/stm32g4/pin_config.h"
__attribute__ ((init_priority(LOGGER_INIT_PRIORITY))) Logger logger;

extern "C" {
void SystemClock_Config();
void board_init() {
    SystemClock_Config();
}
void system_loop_interrupt() {}
void fast_loop_interrupt() {}
void main_loop_interrupt() {}
void usb_interrupt() {}
uint32_t gpio_a_bsrr[10] = {2, 2<<16, 0, 2<<16, 0, 0, 2, 0, 0, 2<<16};
uint8_t dr;
void system_init() {
  RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;
  GPIO_SETL(A, 0, GPIO_MODE::OUTPUT, GPIO_SPEED::VERY_HIGH, 0);
  GPIO_SETL(A, 1, GPIO_MODE::OUTPUT, GPIO_SPEED::VERY_HIGH, 0);
  GPIO_SETL(A, 2, GPIO_MODE::OUTPUT, GPIO_SPEED::VERY_HIGH, 0);
  GPIO_SETL(A, 4, GPIO_MODE::ALT_FUN, GPIO_SPEED::VERY_HIGH, 5); // spi1 nss
  GPIO_SETL(A, 5, GPIO_MODE::ALT_FUN, GPIO_SPEED::HIGH, 5); // spi1
  GPIO_SETL(A, 6, GPIO_MODE::ALT_FUN, GPIO_SPEED::HIGH, 5); // spi1
  GPIO_SETL(A, 7, GPIO_MODE::ALT_FUN, GPIO_SPEED::HIGH, 5); // spi1 
  RCC->AHB1ENR = RCC_AHB1ENR_DMA1EN | RCC_AHB1ENR_DMAMUX1EN;
  RCC->APB2ENR |= RCC_APB2ENR_SPI1EN | RCC_APB2ENR_SPI4EN;
  RCC->APB1ENR1 |= RCC_APB1ENR1_SPI2EN;
  MASK_SET(SYSCFG->EXTICR[0], SYSCFG_EXTICR1_EXTI0, 0); // EXTI PA0
  DMAMUX1_Channel0->CCR =  1; // req gen 0
  DMAMUX1_RequestGenerator0->RGCR = 1 << DMAMUX_RGxCR_GPOL_Pos | 0 << DMAMUX_RGxCR_SIG_ID_Pos | 31 << DMAMUX_RGxCR_GNBREQ_Pos | DMAMUX_RGxCR_GE;
  DMA1_Channel1->CMAR = (uint32_t)&gpio_a_bsrr;
  DMA1_Channel1->CPAR = (uint32_t)&GPIOA->BSRR;
  DMA1_Channel1->CNDTR = 2;
  DMA1_Channel1->CCR = DMA_CCR_CIRC | DMA_CCR_DIR | DMA_CCR_EN | DMA_CCR_MINC | DMA_CCR_MSIZE_1 | DMA_CCR_PSIZE_1;

  DMAMUX1_Channel1->CCR = DMA_REQUEST_SPI1_RX;
  DMA1_Channel2->CMAR = (uint32_t)&dr;
  DMA1_Channel2->CPAR = (uint32_t)&SPI1->DR;
  DMA1_Channel2->CNDTR = 1;
  DMA1_Channel2->CCR = DMA_CCR_CIRC | DMA_CCR_EN | DMA_CCR_MINC;

  DMAMUX1_Channel2->CCR = DMA_REQUEST_SPI4_RX;
  DMA1_Channel3->CMAR = (uint32_t)&dr;
  DMA1_Channel3->CPAR = (uint32_t)&SPI4->DR;
  DMA1_Channel3->CNDTR = 1;
  DMA1_Channel3->CCR = DMA_CCR_CIRC | DMA_CCR_EN | DMA_CCR_MINC;

  DMAMUX1_Channel3->CCR = DMA_REQUEST_SPI2_RX;
  DMA1_Channel4->CMAR = (uint32_t)&dr;
  DMA1_Channel4->CPAR = (uint32_t)&SPI2->DR;
  DMA1_Channel4->CNDTR = 1;
  DMA1_Channel4->CCR = DMA_CCR_CIRC | DMA_CCR_EN | DMA_CCR_MINC;

  DMAMUX1_Channel4->CCR = 0 << DMAMUX_CxCR_SYNC_ID_Pos | 1 << DMAMUX_CxCR_SPOL_Pos | 9 << DMAMUX_CxCR_NBREQ_Pos | DMAMUX_CxCR_SE | DMA_REQUEST_SPI2_TX;
  DMA1_Channel5->CMAR = (uint32_t)&gpio_a_bsrr;
  DMA1_Channel5->CPAR = (uint32_t)&SPI2->DR;
  DMA1_Channel5->CNDTR = 1;
  DMA1_Channel5->CCR = DMA_CCR_CIRC | DMA_CCR_DIR | DMA_CCR_EN | DMA_CCR_MINC;

  DMAMUX1_Channel5->CCR = 0 << DMAMUX_CxCR_SYNC_ID_Pos | 1 << DMAMUX_CxCR_SPOL_Pos | 9 << DMAMUX_CxCR_NBREQ_Pos | DMAMUX_CxCR_SE | DMA_REQUEST_SPI4_TX;
  DMA1_Channel6->CMAR = (uint32_t)&gpio_a_bsrr;
  DMA1_Channel6->CPAR = (uint32_t)&SPI4->DR;
  DMA1_Channel6->CNDTR = 1;
  DMA1_Channel6->CCR = DMA_CCR_CIRC | DMA_CCR_DIR | DMA_CCR_EN | DMA_CCR_MINC;

  DMAMUX1_Channel6->CCR = 0 << DMAMUX_CxCR_SYNC_ID_Pos | 1 << DMAMUX_CxCR_SPOL_Pos | 9 << DMAMUX_CxCR_NBREQ_Pos | DMAMUX_CxCR_SE | DMA_REQUEST_SPI1_TX;
  DMA1_Channel7->CMAR = (uint32_t)&gpio_a_bsrr;
  DMA1_Channel7->CPAR = (uint32_t)&SPI1->DR;
  DMA1_Channel7->CNDTR = 1;
  DMA1_Channel7->CCR = DMA_CCR_CIRC | DMA_CCR_DIR | DMA_CCR_EN | DMA_CCR_MINC;


  SPI1->CR2 = (7 << SPI_CR2_DS_Pos) | SPI_CR2_TXDMAEN | SPI_CR2_RXDMAEN | SPI_CR2_FRXTH | SPI_CR2_SSOE;   // 8 bit
  SPI1->CR1 = 3 << SPI_CR1_BR_Pos | SPI_CR1_MSTR  | SPI_CR1_SPE;
  SPI2->CR2 = (7 << SPI_CR2_DS_Pos) | SPI_CR2_TXDMAEN | SPI_CR2_RXDMAEN | SPI_CR2_FRXTH | SPI_CR2_SSOE;   // 8 bit
  SPI2->CR1 = 3 << SPI_CR1_BR_Pos | SPI_CR1_MSTR  | SPI_CR1_SPE;
  SPI4->CR2 = (7 << SPI_CR2_DS_Pos) | SPI_CR2_TXDMAEN | SPI_CR2_RXDMAEN | SPI_CR2_FRXTH | SPI_CR2_SSOE;   // 8 bit
  SPI4->CR1 = 3 << SPI_CR1_BR_Pos | SPI_CR1_MSTR  | SPI_CR1_SPE;

  NVIC_SetPriority(PendSV_IRQn,
        NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 2, 0));
  NVIC_EnableIRQ(PendSV_IRQn);
}
volatile float j;
volatile uint16_t pma;
void system_run() {
    for(volatile int i=0; i<100000; i++) {
      if (i % 1000 == 0) {
        j = i;
      }
    }
    GPIOA->ODR ^= 1 << 0;
    
    
    //SPI1->DR = (uint8_t) 0x55;
    GPIOA->BSRR = 4;
    GPIOA->BSRR = 4<<16;
    for (volatile int i = 0; i < 100; i++) {
        pma = *(uint16_t *) USB_PMAADDR;
        pma = *(uint16_t *) USB_PMAADDR;
        pma = *(uint16_t *) USB_PMAADDR;
        pma = *(uint16_t *) USB_PMAADDR;



        volatile uint32_t *icsr = (uint32_t *)0xE000ED04;
        // Pend a PendSV exception using by writing 1 to PENDSVSET at bit 28
        *icsr = 0x1 << 28;

    }

    GPIOA->BSRR = 4;
    GPIOA->BSRR = 4<<16;
    GPIOA->BSRR = 4;
    GPIOA->BSRR = 4<<16;
    GPIOA->BSRR = 4;
    GPIOA->BSRR = 4<<16;
    GPIOA->BSRR = 4;
    GPIOA->BSRR = 4<<16;
    GPIOA->BSRR = 4;
    GPIOA->BSRR = 4<<16;
    GPIOA->BSRR = 4;
    GPIOA->BSRR = 4<<16;
    GPIOA->BSRR = 4;
    GPIOA->BSRR = 4<<16;
    GPIOA->BSRR = 4;
    GPIOA->BSRR = 4<<16;
    GPIOA->BSRR = 4;
    GPIOA->BSRR = 4<<16;
    GPIOA->BSRR = 4;
    GPIOA->BSRR = 4<<16;
    GPIOA->BSRR = 4;
    GPIOA->BSRR = 4<<16;
    GPIOA->BSRR = 4;
    GPIOA->BSRR = 4<<16;
    GPIOA->BSRR = 4;
    GPIOA->BSRR = 4<<16;
    GPIOA->BSRR = 4;
    GPIOA->BSRR = 4<<16;
    GPIOA->BSRR = 4;
    GPIOA->BSRR = 4<<16;
    GPIOA->BSRR = 4;
    GPIOA->BSRR = 4<<16;
    GPIOA->BSRR = 4;
    GPIOA->BSRR = 4<<16;
    GPIOA->BSRR = 4;
    GPIOA->BSRR = 4<<16;
    GPIOA->BSRR = 4;
    GPIOA->BSRR = 4<<16;
        GPIOA->BSRR = 4;
    GPIOA->BSRR = 4<<16;
    GPIOA->BSRR = 4;
    GPIOA->BSRR = 4<<16;
    GPIOA->BSRR = 4;
    GPIOA->BSRR = 4<<16;
    GPIOA->BSRR = 4;
    GPIOA->BSRR = 4<<16;
    GPIOA->BSRR = 4;
    GPIOA->BSRR = 4<<16;
    GPIOA->BSRR = 4;
    GPIOA->BSRR = 4<<16;
    GPIOA->BSRR = 4;
    GPIOA->BSRR = 4<<16;
    GPIOA->BSRR = 4;
    GPIOA->BSRR = 4<<16;
    GPIOA->BSRR = 4;
    GPIOA->BSRR = 4<<16;
    GPIOA->BSRR = 4;
    GPIOA->BSRR = 4<<16;
        GPIOA->BSRR = 4;
    GPIOA->BSRR = 4<<16;
    GPIOA->BSRR = 4;
    GPIOA->BSRR = 4<<16;
    GPIOA->BSRR = 4;
    GPIOA->BSRR = 4<<16;
    GPIOA->BSRR = 4;
    GPIOA->BSRR = 4<<16;
    GPIOA->BSRR = 4;
    GPIOA->BSRR = 4<<16;
    GPIOA->BSRR = 4;
    GPIOA->BSRR = 4<<16;
    GPIOA->BSRR = 4;
    GPIOA->BSRR = 4<<16;
    GPIOA->BSRR = 4;
    GPIOA->BSRR = 4<<16;
    GPIOA->BSRR = 4;
    GPIOA->BSRR = 4<<16;
    GPIOA->BSRR = 4;
    GPIOA->BSRR = 4<<16;
        GPIOA->BSRR = 4;
    GPIOA->BSRR = 4<<16;
    GPIOA->BSRR = 4;
    GPIOA->BSRR = 4<<16;
    GPIOA->BSRR = 4;
    GPIOA->BSRR = 4<<16;
    GPIOA->BSRR = 4;
    GPIOA->BSRR = 4<<16;
    GPIOA->BSRR = 4;
    GPIOA->BSRR = 4<<16;
    GPIOA->BSRR = 4;
    GPIOA->BSRR = 4<<16;
    GPIOA->BSRR = 4;
    GPIOA->BSRR = 4<<16;
    GPIOA->BSRR = 4;
    GPIOA->BSRR = 4<<16;
    GPIOA->BSRR = 4;
    GPIOA->BSRR = 4<<16;
    GPIOA->BSRR = 4;
    GPIOA->BSRR = 4<<16;
    //SPI1->CR1 = 3 << SPI_CR1_BR_Pos | SPI_CR1_MSTR ;// | SPI_CR1_SPE;

    asm("nop; nop; nop; nop; nop; nop; nop; nop; nop; nop;");
    asm("nop; nop; nop; nop; nop; nop; nop; nop; nop; nop;");
    asm("nop; nop; nop; nop; nop; nop; nop; nop; nop; nop;");
    asm("nop; nop; nop; nop; nop; nop; nop; nop; nop; nop;");
    IWDG->KR = 0xAAAA;
}
void _write(int fd, const char *buf, size_t count) {
    logger.log(std::string_view(buf, count));
}
int _fstat (int fd, struct stat * st) {
    memset (st, 0, sizeof (* st));
    st->st_mode = S_IFCHR;
    setbuf(stdout, NULL); // Disable buffering for stdout
    setbuf(stderr, NULL);
    return (0);
}
int _isatty (int fd) {
    return (1);
}

void PendSV_Handler() {
    GPIOA->ODR ^= 1 << 2;
    pma = *(uint16_t *) USB_PMAADDR;
    pma = *(uint16_t *) USB_PMAADDR;
    pma = *(uint16_t *) USB_PMAADDR;
    pma = *(uint16_t *) USB_PMAADDR;
    pma = *(uint16_t *) USB_PMAADDR;
}
} // extern "C"

void config_init() {}

void config_maintenance() {}

