#include "st_device.h"
#include <fcntl.h>
#include <cstring>
#include "../../motorlib/logger.h"
#include "../../motorlib/peripheral/stm32g4/pin_config.h"
#include "../../motorlib/control_fun.h"
#include "../../motorlib/util.h"
Logger::CIndex log_index;
char log_queue[LOGGING_MAX_SIZE];
__attribute__ ((init_priority(LOGGER_INIT_PRIORITY))) Logger logger(log_index, log_queue);

extern "C" {
void SystemClock_Config();
void board_init() {
    SystemClock_Config();
}
void system_loop_interrupt() {}
void fast_loop_interrupt() {}
void main_loop_interrupt() {}
void usb_interrupt() {}

uint32_t adc_value;

TrajectoryGenerator traj;

void system_init() {
    RCC->AHB2ENR |= RCC_AHB2ENR_GPIOCEN | RCC_AHB2ENR_ADC12EN | RCC_AHB2ENR_DAC3EN;

    RCC->AHB1ENR |= RCC_AHB1ENR_DMA1EN | RCC_AHB1ENR_DMAMUX1EN;
    RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;
    

    GPIO_SETL(C, 0, GPIO_MODE::ANALOG, GPIO_SPEED::LOW, 0); // adc1 ch6

    ADC2->CR = ADC_CR_ADVREGEN;
    ns_delay(20000);

    ADC12_COMMON->CCR = ADC_CCR_VREFEN | 3 << ADC_CCR_CKMODE_Pos; // hclk/4 (42.5 MHz)
    ADC2->SQR1 = 16 << ADC_SQR1_SQ1_Pos;
    ADC2->CFGR = ADC_CFGR_OVRMOD | ADC_CFGR_CONT | ADC_CFGR_DMAEN | ADC_CFGR_DMACFG;
    ADC2->CFGR2 = 0;
    ADC2->SMPR2 = 0 << ADC_SMPR2_SMP16_Pos; // 2.5 cycles
    ADC2->CR |= ADC_CR_ADEN;
    while(!(ADC2->ISR & ADC_ISR_ADRDY));
    ADC2->CR |= ADC_CR_ADSTART;

    DMAMUX1_Channel0->CCR =  36; // ADC2
    DMA1_Channel1->CPAR = (uint32_t) &ADC2->DR;
    DMA1_Channel1->CMAR = (uint32_t) &DAC3->DHR12R1;
    DMA1_Channel1->CNDTR = 1;
    DMA1_Channel1->CCR = DMA_CCR_MINC | DMA_CCR_CIRC | 2 << DMA_CCR_MSIZE_Pos | 2 << DMA_CCR_PSIZE_Pos | DMA_CCR_EN;
    DMA1_Channel1->CCR = 0;

    OPAMP1->CSR = 3 << OPAMP_CSR_VPSEL_Pos | 3 << OPAMP_CSR_VMSEL_Pos | OPAMP_CSR_HIGHSPEEDEN | OPAMP_CSR_OPAMPxEN; // follower on dac3 ch1 out to pa2
    OPAMP2->CSR = 2 << OPAMP_CSR_VPSEL_Pos | 3 << OPAMP_CSR_VMSEL_Pos | OPAMP_CSR_HIGHSPEEDEN | OPAMP_CSR_OPAMPINTEN | OPAMP_CSR_OPAMPxEN; // follower on pb0 to adc2_ch16
    DAC3->MCR = 3 << DAC_MCR_MODE1_Pos;
    DAC3->CR = DAC_CR_EN1;

    traj.set_amplitude(1.5);
    traj.set_frequency(1000);
}

void system_run() {
    uint32_t now = get_clock();
    static uint32_t last = now;
    float dt = (float) (now - last) * (1.0 / CPU_FREQUENCY_HZ);
    last = now;
    auto t = traj.step(dt);
    DAC3->DHR12R1 = (t.value+1.65)*(1/3.3)*4096;  
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


} // extern "C"

void config_init() {}

void config_maintenance() {}

