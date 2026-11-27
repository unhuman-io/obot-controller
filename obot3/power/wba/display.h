#pragma once
// The Sharp memory LCD on port.h's SPI, for both radio firmwares: the status
// screen, only the lines that changed, and the polarity flip the display wants
// at least once a second (see power.frame).
#include <cstddef>
#include <cstdint>
#include "port.h"
import power.frame;
import power.screen;

struct Display {
    Frame frame;
    LcdShadow shadow;
    bool vcom = false;
    uint32_t flipped_ms = 0;

    // DISP is pulled low on the board: the glass is blank until this
    void start(char cs_port, int cs_pin, char disp_port, int disp_pin) {
        uint8_t bytes[2];
        port_lcd_init(cs_port, cs_pin);
        port_lcd_send(bytes, lcd_clear(vcom, bytes));
        port_pin_output(disp_port, disp_pin, false, true);
        frame.clear();
        shadow.sent(frame, {0, lcd_height - 1});
        shadow.known = true;
    }

    void show(const ScreenData& data, uint32_t now_ms) {
        static uint8_t bytes[lcd_transfer_max];
        draw_screen(frame, data);
        flip(now_ms);
        bool sent = false;
        for (LcdShadow::Run run = shadow.next(frame); !run.empty(); run = shadow.next(frame, run.last + 1)) {
            port_lcd_send(bytes, lcd_lines(frame, run.first, run.last, vcom, bytes));
            shadow.sent(frame, run);
            sent = true;
        }
        if (!sent && flipped_ms == now_ms) port_lcd_send(bytes, lcd_hold(vcom, bytes));     // nothing to draw: the flip alone
    }

 private:
    void flip(uint32_t now_ms) {
        if (now_ms - flipped_ms < 500) return;
        vcom = !vcom;
        flipped_ms = now_ms;
    }
};
