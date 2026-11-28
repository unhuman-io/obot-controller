// What the radio firmware of the power board and of the remote need from the
// processor and from the radio, as plain C functions. port.c implements them
// on ST's HAL and on ST's Bluetooth LE link layer; the applications (../radio,
// ../remote) are C++ and see nothing of either.
//
// The radio is used without connections. A node advertises a frame of up to
// 26 bytes again and again (as "manufacturer data" of a non-connectable
// advertisement) and listens for those of the one other node it is paired
// with. Who may say what is in the frames (power.messages), not in Bluetooth.
#ifndef POWER_PORT_H
#define POWER_PORT_H
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#ifdef __cplusplus
extern "C" {
#endif

// ---- implemented by the application -----------------------------------------
void app_init(void);            // once, with the radio stack up
void app_tick(void);            // every port_tick_period(), and after a pin with port_pin_wake() changed
// a frame from the node port_radio_listen() was given, in the radio's task
void app_radio_received(const uint8_t* frame, uint8_t length, int8_t rssi);

// ---- time, sleep -------------------------------------------------------------
uint32_t port_ms(void);                 // since start; goes on through sleep
void port_tick_period(uint32_t ms);
// false: the processor's clocks stay on between events (UART, SPI and ADC in use: some mA).
// true: Stop mode between events (tens of uA with the radio's own sleep).
void port_allow_stop(bool allow);
uint32_t port_random(void);

// ---- pins: port 'A'.., pin 0..15 ------------------------------------------------------
void port_pin_output(char port, int pin, bool open_drain, bool high);
void port_pin_input(char port, int pin, int pull);      // pull: 0 none, 1 up, -1 down
void port_pin_write(char port, int pin, bool high);
bool port_pin_read(char port, int pin);
// An input whose every change wakes the processor and runs app_tick(). One pin for each
// pin number (PB13 and PC13 cannot both); numbers 1, 2, 6 and 13 have a handler here.
void port_pin_wake(char port, int pin, int pull);

// ---- the display's SPI (SPI3: PA0 clock, PB8 data, least significant bit first, 1 MHz) --
void port_lcd_init(char cs_port, int cs_pin);           // chip select is active HIGH
void port_lcd_send(const uint8_t* data, size_t length);

// ---- analog: millivolts at PA1 (ADC4 channel 8), 3.0 V full scale ---------------------
uint16_t port_adc_mv(void);

// ---- the link to the main controller (power board only): LPUART1, PB5 out, PA10 in ----
void port_link_init(void);
void port_link_write(const uint8_t* data, size_t length);
bool port_link_read(uint8_t* byte);

// ---- the dock's data contact: one wire, both ways, 2400 baud (USART1 on PB12) ---------
// Nothing here waits: what is written goes out a few bytes at each port_dock_poll(), which
// the application calls from its tick (20 ms or faster while the dock is in use), and the
// other end reads at the same rate without losing any.
void port_dock_init(void);
void port_dock_write(const uint8_t* data, size_t length);   // up to 64 bytes waiting; its own echo is thrown away
void port_dock_poll(void);
bool port_dock_read(uint8_t* byte);

// ---- radio ------------------------------------------------------------------------------
void port_radio_address(uint8_t address[6]);            // this node's
// The frame to send from now on, every interval_ms (20 at least). length 0: stop sending.
void port_radio_send(const uint8_t* frame, uint8_t length, uint16_t interval_ms);
// Listen for frames of `peer` for window_ms out of every interval_ms (equal: all the time).
// window_ms 0 or peer NULL: stop listening.
void port_radio_listen(const uint8_t peer[6], uint16_t interval_ms, uint16_t window_ms);

// ---- what survives power off: one small record in the last page of the flash ------------
// (written with the radio stopped by the caller: the flash and the radio do not share time well)
bool port_store_read(uint8_t* data, size_t length);
bool port_store_write(const uint8_t* data, size_t length);

#ifdef __cplusplus
}
#endif
#endif
