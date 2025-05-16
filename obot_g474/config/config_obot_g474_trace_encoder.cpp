#include "../../motorlib/logger.h"
#include "../../motorlib/peripheral/stm32g4/pin_config.h"
import trace_board;

constexpr uint8_t FMAC_Read = 110;
constexpr uint8_t FMAC_Write = 111;

extern "C" void board_init() {
    TraceBoard::board_init();
    RCC->AHB1ENR |= RCC_AHB1ENR_DMA1EN | RCC_AHB1ENR_DMAMUX1EN | RCC_AHB1ENR_FMACEN;
    RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;
    RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;
    
   
    GPIO_SETL(A, 4, GPIO_MODE::ALT_FUN, GPIO_SPEED::VERY_HIGH, 5);   // SPI1 CS
    GPIO_SETL(A, 5, GPIO_MODE::ALT_FUN, GPIO_SPEED::VERY_HIGH, 5);   // SPI1 CLK
    GPIO_SETL(A, 6, GPIO_MODE::ALT_FUN, GPIO_SPEED::MEDIUM, 5);   // SPI1 MISO
    GPIO_SETL(A, 7, GPIO_MODE::ALT_FUN, GPIO_SPEED::VERY_HIGH, 5);   // SPI1 MOSI
}

Logger::CIndex log_index __attribute__((section(".noload")));
char log_queue[LOGGING_MAX_SIZE] __attribute__((section(".noload")));
__attribute__ ((init_priority(LOGGER_INIT_PRIORITY))) Logger logger(log_index, log_queue);
extern const char * const name = "obot_g474_trace2";

extern "C" void system_run(){
    IWDG->KR = 0xAAAA;
    //FMAC->RDATA;
}

uint8_t data_in[6];
uint32_t fmac_x = 0x1;
constexpr uint16_t fmac_b = 0x100;
constexpr uint16_t fmac_a = 0x100;
uint8_t data_out[6] = {0x12, 0x34, 0x56, 0x78, 0x9a, 0xbc};
uint8_t data_out2[6] = {0x11, 0x22, 0x33, 0x44, 0x55, 0x66};
extern "C" void system_init(){
    SPI1->CR2 = (7 << SPI_CR2_DS_Pos) | SPI_CR2_FRXTH | SPI_CR2_TXDMAEN | SPI_CR2_RXDMAEN;   // 8 bit
    DMAMUX1_Channel0->CCR =  DMA_REQUEST_SPI1_TX;
    DMAMUX1_Channel1->CCR =  DMA_REQUEST_SPI1_RX | 4 << DMAMUX_CxCR_NBREQ_Pos | DMAMUX_CxCR_EGE;
    DMA1_Channel1->CNDTR = 5;
    DMA1_Channel1->CPAR = (uint32_t) &SPI1->DR;
    DMA1_Channel1->CMAR = (uint32_t) data_out;
    DMA1_Channel1->CCR = DMA_CCR_CIRC | DMA_CCR_DIR | DMA_CCR_EN | DMA_CCR_MINC; // read from memory
    DMA1_Channel2->CNDTR = 5;
    DMA1_Channel2->CPAR = (uint32_t) &SPI1->DR;
    DMA1_Channel2->CMAR = (uint32_t) data_in;
    DMA1_Channel2->CCR = DMA_CCR_CIRC | DMA_CCR_EN | DMA_CCR_MINC; // write to memory

    //FMAC->CR = FMAC_CR_DMAREN;

    FMAC->X1BUFCFG = 1 << FMAC_X1BUFCFG_X1_BUF_SIZE_Pos;
    FMAC->X2BUFCFG = 3 << FMAC_X2BUFCFG_X2_BUF_SIZE_Pos | 1 << FMAC_X2BUFCFG_X2_BASE_Pos;
    FMAC->YBUFCFG = 1 << FMAC_YBUFCFG_Y_BUF_SIZE_Pos | 4 << FMAC_YBUFCFG_Y_BASE_Pos;
    FMAC->PARAM = FMAC_PARAM_START | 2 << FMAC_PARAM_FUNC_Pos | 1 << FMAC_PARAM_P_Pos | 1 << FMAC_PARAM_Q_Pos; // load 1 a and 1 b
    FMAC->WDATA = fmac_b;
    //FMAC->WDATA = 0;
    FMAC->WDATA = fmac_a;
    asm("nop");
    FMAC->PARAM =  9 << FMAC_PARAM_FUNC_Pos | 1 << FMAC_PARAM_P_Pos | 1 << FMAC_PARAM_Q_Pos | 7 << FMAC_PARAM_R_Pos; // IIR filter
    FMAC->CR = FMAC_CR_DMAREN;
    FMAC->PARAM |= FMAC_PARAM_START;


    DMAMUX1_Channel2->CCR = 1; // Req gen 0
    DMAMUX1_RequestGenerator0->RGCR = 1 << DMAMUX_RGxCR_GPOL_Pos | DMAMUX_RGxCR_GE | 17 << DMAMUX_RGxCR_SIG_ID_Pos; // dmamux_ch1 event
    DMA1_Channel3->CNDTR = 1;
    DMA1_Channel3->CPAR = (uint32_t) &FMAC->WDATA;
    DMA1_Channel3->CMAR = (uint32_t) &fmac_x;
    DMA1_Channel3->CCR = DMA_CCR_CIRC | DMA_CCR_DIR | DMA_CCR_MINC | DMA_CCR_EN | 2 << DMA_CCR_MSIZE_Pos | 2 << DMA_CCR_PSIZE_Pos ; // read from memory

    DMAMUX1_Channel3->CCR = FMAC_Read;
    DMA1_Channel4->CNDTR = 1;
    DMA1_Channel4->CPAR = (uint32_t) &FMAC->RDATA;
    DMA1_Channel4->CMAR = (uint32_t) data_out;
    DMA1_Channel4->CCR = DMA_CCR_CIRC | DMA_CCR_EN | DMA_CCR_MINC | 2 << DMA_CCR_MSIZE_Pos | 2 << DMA_CCR_PSIZE_Pos ; // write to memory

    SPI1->CR1 |= SPI_CR1_SPE;
}
extern "C" void usb_interrupt(){}
extern "C" void fast_loop_interrupt(){}
extern "C" void main_loop_interrupt(){}
extern "C" void system_loop_interrupt(){}
