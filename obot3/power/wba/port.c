// port.h on an STM32WBA5MMG module, on top of ST's HAL and of ST's example
// BLE_BeaconHCI from STM32CubeWBA, whose files are compiled as they are (see
// ../README.md): its start-up brings up clocks, RTC, timer server, sequencer,
// low power manager and the Bluetooth LE link layer, and at the end of that
// calls IBeacon_Process(), which is here and starts our application.
#include <string.h>

#include "main.h"
#include "app_conf.h"
#include "app_ble.h"
#include "ble_core.h"
#include "svc_ctl.h"
#include "stm32_seq.h"
#include "stm32_timer.h"
#include "stm32_lpm.h"
#include "stm32_lpm_if.h"
#include "adc_ctrl.h"
#include "stm32wbaxx_ll_adc.h"
#include "port.h"

// the sequencer task of a beacon kind we do not build: ours now
#define TASK_TICK (1U << CFG_TASK_EDDYSTONE_TLML_REQ_ID)

// ---- start, tick, sleep -------------------------------------------------------
static UTIL_TIMER_Object_t tick_timer;
static SVCCTL_EvtAckStatus_t radio_event(void* packet);

static void tick_task(void) { app_tick(); }
static void tick_timer_callback(void* argument) {
    (void)argument;
    UTIL_SEQ_SetTask(TASK_TICK, CFG_SEQ_PRIO_0);
}

tBleStatus IBeacon_Process(void) {
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();
    SVCCTL_RegisterHandler(radio_event);
    UTIL_SEQ_RegTask(TASK_TICK, UTIL_SEQ_RFU, tick_task);
    UTIL_TIMER_Create(&tick_timer, 1000, UTIL_TIMER_PERIODIC, tick_timer_callback, NULL);
    app_init();
    UTIL_TIMER_Start(&tick_timer);
    return BLE_STATUS_SUCCESS;
}

uint32_t port_ms(void) { return UTIL_TIMER_GetCurrentTime(); }

void port_tick_period(uint32_t ms) {
    static uint32_t period = 1000;
    if (ms == period) return;
    period = ms;
    UTIL_TIMER_StartWithPeriod(&tick_timer, ms);
}

void port_allow_stop(bool allow) {
    UTIL_LPM_SetMaxMode(1U << CFG_LPM_APP, allow ? UTIL_LPM_MAX_MODE : UTIL_LPM_SLEEP_MODE);
}

uint32_t port_random(void) {
    uint8_t bytes[8] = {0};
    uint32_t value;
    hci_le_rand(bytes);
    memcpy(&value, bytes, sizeof value);
    return value;
}

// With -nostartfiles (the llvm build) the C library's start-up still calls these
void __attribute__((weak)) _init(void) {}
void __attribute__((weak)) _fini(void) {}

// ---- pins -------------------------------------------------------------------------
static GPIO_TypeDef* gpio(char port) {
    return port == 'A' ? GPIOA : port == 'B' ? GPIOB
                                             : GPIOC;
}

void port_pin_output(char port, int pin, bool open_drain, bool high) {
    GPIO_InitTypeDef init = {0};
    HAL_GPIO_WritePin(gpio(port), (uint16_t)(1U << pin), high ? GPIO_PIN_SET : GPIO_PIN_RESET);   // the level first
    init.Pin = 1U << pin;
    init.Mode = open_drain ? GPIO_MODE_OUTPUT_OD : GPIO_MODE_OUTPUT_PP;
    init.Pull = GPIO_NOPULL;
    init.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(gpio(port), &init);
}

static uint32_t pull_of(int pull) {
    return pull > 0 ? GPIO_PULLUP : pull < 0 ? GPIO_PULLDOWN
                                             : GPIO_NOPULL;
}

void port_pin_input(char port, int pin, int pull) {
    GPIO_InitTypeDef init = {0};
    init.Pin = 1U << pin;
    init.Mode = GPIO_MODE_INPUT;
    init.Pull = pull_of(pull);
    HAL_GPIO_Init(gpio(port), &init);
}

void port_pin_write(char port, int pin, bool high) {
    HAL_GPIO_WritePin(gpio(port), (uint16_t)(1U << pin), high ? GPIO_PIN_SET : GPIO_PIN_RESET);
}

bool port_pin_read(char port, int pin) {
    return HAL_GPIO_ReadPin(gpio(port), (uint16_t)(1U << pin)) == GPIO_PIN_SET;
}

void port_pin_wake(char port, int pin, int pull) {
    static const IRQn_Type line[16] = {EXTI0_IRQn, EXTI1_IRQn, EXTI2_IRQn, EXTI3_IRQn, EXTI4_IRQn, EXTI5_IRQn, EXTI6_IRQn, EXTI7_IRQn,
                                       EXTI8_IRQn, EXTI9_IRQn, EXTI10_IRQn, EXTI11_IRQn, EXTI12_IRQn, EXTI13_IRQn, EXTI14_IRQn, EXTI15_IRQn};
    GPIO_InitTypeDef init = {0};
    init.Pin = 1U << pin;
    init.Mode = GPIO_MODE_IT_RISING_FALLING;
    init.Pull = pull_of(pull);
    HAL_GPIO_Init(gpio(port), &init);
    HAL_NVIC_SetPriority(line[pin], 14, 0);
    HAL_NVIC_EnableIRQ(line[pin]);
}

void EXTI1_IRQHandler(void) { HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_1); }
void EXTI2_IRQHandler(void) { HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_2); }
void EXTI6_IRQHandler(void) { HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_6); }
void EXTI13_IRQHandler(void) { HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_13); }
void HAL_GPIO_EXTI_Rising_Callback(uint16_t pin) {
    (void)pin;
    UTIL_SEQ_SetTask(TASK_TICK, CFG_SEQ_PRIO_0);
}
void HAL_GPIO_EXTI_Falling_Callback(uint16_t pin) {
    (void)pin;
    UTIL_SEQ_SetTask(TASK_TICK, CFG_SEQ_PRIO_0);
}

// ---- the display ----------------------------------------------------------------------
static SPI_HandleTypeDef lcd_spi;
static char lcd_cs_port;
static int lcd_cs_pin;

void HAL_SPI_MspInit(SPI_HandleTypeDef* spi) {
    GPIO_InitTypeDef init = {0};
    if (spi->Instance != SPI3) return;
    __HAL_RCC_SPI3_CLK_ENABLE();
    init.Mode = GPIO_MODE_AF_PP;
    init.Pull = GPIO_NOPULL;
    init.Speed = GPIO_SPEED_FREQ_LOW;
    init.Alternate = GPIO_AF6_SPI3;
    init.Pin = GPIO_PIN_0;              // PA0 SPI3_SCK
    HAL_GPIO_Init(GPIOA, &init);
    init.Pin = GPIO_PIN_8;              // PB8 SPI3_MOSI
    HAL_GPIO_Init(GPIOB, &init);
}

void port_lcd_init(char cs_port, int cs_pin) {
    lcd_cs_port = cs_port;
    lcd_cs_pin = cs_pin;
    port_pin_output(cs_port, cs_pin, false, false);
    lcd_spi.Instance = SPI3;
    lcd_spi.Init.Mode = SPI_MODE_MASTER;
    lcd_spi.Init.Direction = SPI_DIRECTION_2LINES_TXONLY;
    lcd_spi.Init.DataSize = SPI_DATASIZE_8BIT;
    lcd_spi.Init.CLKPolarity = SPI_POLARITY_LOW;        // idle low, data taken on the rising edge
    lcd_spi.Init.CLKPhase = SPI_PHASE_1EDGE;
    lcd_spi.Init.NSS = SPI_NSS_SOFT;
    lcd_spi.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_32;     // 1 MHz from 32 MHz (0.5 when the radio has the clock at 16)
    lcd_spi.Init.FirstBit = SPI_FIRSTBIT_LSB;
    lcd_spi.Init.TIMode = SPI_TIMODE_DISABLE;
    lcd_spi.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
    lcd_spi.Init.MasterKeepIOState = SPI_MASTER_KEEP_IO_STATE_ENABLE;     // the clock stays low between transfers
    HAL_SPI_Init(&lcd_spi);
}

static void short_wait(void) {
    for (volatile int k = 0; k < 100; k++) {}     // more than 6 us at 32 MHz
}

void port_lcd_send(const uint8_t* data, size_t length) {
    port_pin_write(lcd_cs_port, lcd_cs_pin, true);
    short_wait();
    HAL_SPI_Transmit(&lcd_spi, (uint8_t*)data, (uint16_t)length, 100);
    short_wait();
    port_pin_write(lcd_cs_port, lcd_cs_pin, false);
}

// ---- analog -----------------------------------------------------------------------------
// The ADC is shared with the radio's temperature measurement through ST's
// ADC controller; this is our request, as theirs but for channel 8 (PA1).
static ADCCTRL_Handle_t adc_request = {
    .Uid = 0x00,
    .State = ADCCTRL_HANDLE_NOT_REG,
    .InitConf = {.ConvParams = {.TriggerFrequencyMode = LL_ADC_TRIGGER_FREQ_LOW,
                                .Resolution = LL_ADC_RESOLUTION_12B,
                                .DataAlign = LL_ADC_DATA_ALIGN_RIGHT,
                                .TriggerStart = LL_ADC_REG_TRIG_SOFTWARE,
                                .TriggerEdge = LL_ADC_REG_TRIG_EXT_RISING,
                                .ConversionMode = LL_ADC_REG_CONV_SINGLE,
                                .DmaTransfer = LL_ADC_REG_DMA_TRANSFER_NONE,
                                .Overrun = LL_ADC_REG_OVR_DATA_OVERWRITTEN,
                                .SamplingTimeCommon1 = LL_ADC_SAMPLINGTIME_814CYCLES_5,    // megohm dividers
                                .SamplingTimeCommon2 = LL_ADC_SAMPLINGTIME_1CYCLE_5},
                 .SeqParams = {.Setup = LL_ADC_REG_SEQ_CONFIGURABLE, .Length = LL_ADC_REG_SEQ_SCAN_DISABLE, .DiscMode = LL_ADC_REG_SEQ_DISCONT_1RANK},
                 .LowPowerParams = {.AutoPowerOff = DISABLE, .AutonomousDPD = LL_ADC_LP_AUTONOMOUS_DPD_DISABLE}},
    .ChannelConf = {{.Channel = LL_ADC_CHANNEL_8, .Rank = LL_ADC_REG_RANK_1, .SamplingTime = LL_ADC_SAMPLINGTIME_COMMON_1}},
};

uint16_t port_adc_mv(void) {
    int16_t raw = 0;
    if (adc_request.State == ADCCTRL_HANDLE_NOT_REG && ADCCTRL_RegisterHandle(&adc_request) != ADCCTRL_OK) return 0;
    if (ADCCTRL_RequestIpState(&adc_request, ADC_ON) != ADCCTRL_OK) return 0;
    if (ADCCTRL_RequestValues(&adc_request, &raw) != ADCCTRL_OK) raw = 0;
    ADCCTRL_RequestIpState(&adc_request, ADC_OFF);
    return (uint16_t)((uint32_t)(raw < 0 ? 0 : raw) * 3000U / 4095U);
}

// ---- the two UARTs, by their registers (ST's start-up owns the HAL's USART1 for its log) --
// Both run from the 16 MHz internal oscillator: the system clock changes
// between 16 and 32 MHz as the radio needs it.
#define UART_ERRORS (USART_ISR_PE | USART_ISR_FE | USART_ISR_NE | USART_ISR_ORE)

static volatile uint8_t link_queue[64];
static volatile uint32_t link_head, link_tail;

void port_link_init(void) {
    GPIO_InitTypeDef init = {0};
    RCC->CR |= RCC_CR_HSION;
    __HAL_RCC_LPUART1_CONFIG(RCC_LPUART1CLKSOURCE_HSI);
    __HAL_RCC_LPUART1_CLK_ENABLE();
    init.Mode = GPIO_MODE_AF_PP;
    init.Pull = GPIO_NOPULL;
    init.Speed = GPIO_SPEED_FREQ_LOW;
    init.Alternate = GPIO_AF8_LPUART1;
    init.Pin = GPIO_PIN_5;              // PB5 LPUART1_TX
    HAL_GPIO_Init(GPIOB, &init);
    init.Pin = GPIO_PIN_10;             // PA10 LPUART1_RX (pulled down on the board while the main controller is off)
    HAL_GPIO_Init(GPIOA, &init);
    LPUART1->CR1 = 0;
    LPUART1->BRR = (uint32_t)((256ULL * 16000000ULL + 115200U / 2U) / 115200U);
    LPUART1->CR1 = USART_CR1_FIFOEN | USART_CR1_RXNEIE_RXFNEIE | USART_CR1_TE | USART_CR1_RE;
    LPUART1->CR1 |= USART_CR1_UE;
    HAL_NVIC_SetPriority(LPUART1_IRQn, 13, 0);
    HAL_NVIC_EnableIRQ(LPUART1_IRQn);
}

void LPUART1_IRQHandler(void) {
    uint32_t status = LPUART1->ISR;
    if (status & UART_ERRORS) LPUART1->ICR = status & UART_ERRORS;
    while (LPUART1->ISR & USART_ISR_RXNE_RXFNE) {
        const uint8_t byte = (uint8_t)LPUART1->RDR;
        if (link_head - link_tail < sizeof link_queue) {
            link_queue[link_head % sizeof link_queue] = byte;
            link_head++;
        }
    }
}

bool port_link_read(uint8_t* byte) {
    if (link_tail == link_head) return false;
    *byte = link_queue[link_tail % sizeof link_queue];
    link_tail++;
    return true;
}

void port_link_write(const uint8_t* data, size_t length) {
    for (size_t k = 0; k < length; k++) {
        for (uint32_t tries = 0; !(LPUART1->ISR & USART_ISR_TXE_TXFNF) && tries < 100000U; tries++) {}
        LPUART1->TDR = data[k];
    }
}

static uint8_t dock_queue[64];
static uint32_t dock_head, dock_tail, dock_echo;

void port_dock_init(void) {
    GPIO_InitTypeDef init = {0};
    RCC->CR |= RCC_CR_HSION;
    __HAL_RCC_USART1_CONFIG(RCC_USART1CLKSOURCE_HSI);
    __HAL_RCC_USART1_CLK_ENABLE();
    init.Mode = GPIO_MODE_AF_OD;        // one wire for both directions: only ever pulled low
    init.Pull = GPIO_PULLUP;
    init.Speed = GPIO_SPEED_FREQ_LOW;
    init.Alternate = GPIO_AF7_USART1;
    init.Pin = GPIO_PIN_12;             // PB12 USART1_TX
    HAL_GPIO_Init(GPIOB, &init);
    USART1->CR1 = 0;
    USART1->BRR = 16000000U / 2400U;
    USART1->CR3 = USART_CR3_HDSEL;      // the receiver listens on the same pin
    USART1->CR1 = USART_CR1_FIFOEN | USART_CR1_TE | USART_CR1_RE;
    USART1->CR1 |= USART_CR1_UE;
}

void port_dock_write(const uint8_t* data, size_t length) {
    for (size_t k = 0; k < length && dock_head - dock_tail < sizeof dock_queue; k++) {
        dock_queue[dock_head % sizeof dock_queue] = data[k];
        dock_head++;
    }
}

// Two bytes a call: at 2400 baud a byte takes 4 ms, so with a call every 20 ms
// or faster neither this end's fifo (8 bytes) nor the other's ever fills.
void port_dock_poll(void) {
    for (int k = 0; k < 2 && dock_tail != dock_head && (USART1->ISR & USART_ISR_TXE_TXFNF); k++) {
        USART1->TDR = dock_queue[dock_tail % sizeof dock_queue];
        dock_tail++;
        dock_echo++;
    }
}

bool port_dock_read(uint8_t* byte) {
    for (;;) {
        const uint32_t status = USART1->ISR;
        if (status & UART_ERRORS) USART1->ICR = status & UART_ERRORS;
        if (!(status & USART_ISR_RXNE_RXFNE)) return false;
        *byte = (uint8_t)USART1->RDR;
        if (dock_echo == 0) return true;
        dock_echo--;                    // what we sent ourselves
    }
}

// ---- radio --------------------------------------------------------------------------------
#define COMPANY 0xFFFFU     // "manufacturer data" of nobody: the Bluetooth number for tests and in-house use

void port_radio_address(uint8_t address[6]) { memcpy(address, BleGetBdAddress(), 6); }

static uint16_t radio_units(uint32_t ms) {      // of 0.625 ms
    const uint32_t units = ms * 8U / 5U;
    return (uint16_t)(units < 0x20U ? 0x20U : units > 0x4000U ? 0x4000U
                                                              : units);
}

void port_radio_send(const uint8_t* frame, uint8_t length, uint16_t interval_ms) {
    static bool sending;
    static uint16_t interval;
    static const uint8_t nobody[6] = {0};
    uint8_t data[31];
    if (length == 0 || length > 24) {
        if (sending) hci_le_set_advertising_enable(0);
        sending = false;
        return;
    }
    data[0] = 2;            // flags: discoverable for ever, Bluetooth LE only
    data[1] = 0x01;
    data[2] = 0x06;
    data[3] = (uint8_t)(length + 3);
    data[4] = 0xFF;         // manufacturer data
    data[5] = (uint8_t)(COMPANY & 0xFFU);
    data[6] = (uint8_t)(COMPANY >> 8);
    memcpy(data + 7, frame, length);
    if (sending && interval != interval_ms) {
        hci_le_set_advertising_enable(0);
        sending = false;
    }
    if (!sending) {
        const uint16_t units = radio_units(interval_ms);
        // not connectable, not scannable (3), our own public address (0), all three channels (7), from anybody (0)
        hci_le_set_advertising_parameters(units, units, 3, 0, 0, nobody, 7, 0);
    }
    hci_le_set_advertising_data((uint8_t)(7 + length), data);
    if (!sending) hci_le_set_advertising_enable(1);
    sending = true;
    interval = interval_ms;
}

void port_radio_listen(const uint8_t peer[6], uint16_t interval_ms, uint16_t window_ms) {
    static bool listening;
    if (listening) hci_le_set_scan_enable(0, 0);
    listening = false;
    if (peer == NULL || window_ms == 0) return;
    hci_le_clear_filter_accept_list();
    hci_le_add_device_to_filter_accept_list(0, peer);
    // passive (0), our public address (0), only what is on the list (1)
    hci_le_set_scan_parameters(0, radio_units(interval_ms), radio_units(window_ms > interval_ms ? interval_ms : window_ms), 0, 1);
    hci_le_set_scan_enable(1, 0);       // every packet, repeats too: each one is news
    listening = true;
}

// An advertising report, as the link layer sends it up:
//   0x04, 0x3E (LE meta event), length, 0x02 (advertising report), number of reports,
//   then for each: kind, address type, address (6), data length, data, rssi
static SVCCTL_EvtAckStatus_t radio_event(void* packet) {
    const uint8_t* p = (const uint8_t*)packet;
    if (p[1] != HCI_LE_META_EVT_CODE || p[3] != HCI_LE_ADVERTISING_REPORT_SUBEVT_CODE) return SVCCTL_EvtNotAck;
    const uint8_t* end = p + 3 + p[2];
    const uint8_t* report = p + 5;
    for (uint8_t n = p[4]; n > 0 && report + 9 <= end; n--) {
        const uint8_t length = report[8];
        const uint8_t* data = report + 9;
        if (data + length + 1 > end) break;
        // the parts of the advertisement: length, kind, content
        for (uint8_t at = 0; at + 1 < length && data[at] != 0 && at + data[at] < length; at = (uint8_t)(at + data[at] + 1)) {
            if (data[at + 1] == 0xFF && data[at] >= 3 && data[at + 2] == (COMPANY & 0xFFU) && data[at + 3] == (COMPANY >> 8)) {
                app_radio_received(data + at + 4, (uint8_t)(data[at] - 3), (int8_t)data[length]);
            }
        }
        report = data + length + 1;
    }
    return SVCCTL_EvtAckFlowEnable;
}

// ---- the record in the flash ---------------------------------------------------------------
#define STORE_PAGE    (FLASH_PAGE_NB - 1U)
#define STORE_ADDRESS (FLASH_BASE + STORE_PAGE * FLASH_PAGE_SIZE)
#define STORE_MAGIC   0x3150424FU     // "OBP1"
#define STORE_MAX     96U

static uint16_t store_check(const uint8_t* data, size_t length) {
    uint16_t crc = 0xFFFF;
    for (size_t k = 0; k < length; k++) {
        crc ^= (uint16_t)(data[k] << 8);
        for (int bit = 0; bit < 8; bit++) crc = (uint16_t)((crc & 0x8000U) ? (crc << 1) ^ 0x1021U : crc << 1);
    }
    return crc;
}

// magic (4), length (2), check (2), data
bool port_store_read(uint8_t* data, size_t length) {
    const uint8_t* stored = (const uint8_t*)STORE_ADDRESS;
    uint32_t magic;
    uint16_t said_length, said_check;
    memcpy(&magic, stored, 4);
    memcpy(&said_length, stored + 4, 2);
    memcpy(&said_check, stored + 6, 2);
    if (magic != STORE_MAGIC || said_length != length || length > STORE_MAX) return false;
    if (store_check(stored + 8, length) != said_check) return false;
    memcpy(data, stored + 8, length);
    return true;
}

bool port_store_write(const uint8_t* data, size_t length) {
    static uint32_t record[(8U + STORE_MAX + 15U) / 16U * 4U];     // whole words of 128 bits, as the flash is written
    FLASH_EraseInitTypeDef erase = {0};
    uint32_t page_error = 0;
    const uint32_t magic = STORE_MAGIC;
    const uint16_t said_length = (uint16_t)length, check = store_check(data, length);
    bool ok;
    if (length > STORE_MAX) return false;
    memset(record, 0xFF, sizeof record);
    memcpy((uint8_t*)record, &magic, 4);
    memcpy((uint8_t*)record + 4, &said_length, 2);
    memcpy((uint8_t*)record + 6, &check, 2);
    memcpy((uint8_t*)record + 8, data, length);
    HAL_FLASH_Unlock();
    erase.TypeErase = FLASH_TYPEERASE_PAGES;
    erase.Page = STORE_PAGE;
    erase.NbPages = 1;
    ok = HAL_FLASHEx_Erase(&erase, &page_error) == HAL_OK;
    for (uint32_t k = 0; ok && k < (8U + length + 15U) / 16U; k++) {
        ok = HAL_FLASH_Program(FLASH_TYPEPROGRAM_QUADWORD, STORE_ADDRESS + 16U * k, (uint32_t)&record[4U * k]) == HAL_OK;
    }
    HAL_FLASH_Lock();
    return ok;
}
