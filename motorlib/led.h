

#ifndef LED_H
#define LED_H

#include <cstdint>
// A tricolor led for status, pwm compare registers required
class LED {
 public:
    LED(uint16_t *const red_reg, uint16_t *const green_reg, uint16_t *const blue_reg, float brightness=1.0)
        : red_reg_(red_reg), green_reg_(green_reg), blue_reg_(blue_reg) {
            brightness_ = brightness;
        }
    enum Mode {OFF, ON, BLINKING, PULSING};
    enum Color {RED, ORANGE, YELLOW, CHARTREUSE, GREEN, SPRING, CYAN, AZURE, BLUE, VIOLET, MAGENTA, ROSE, WHITE};
    void set_mode(Mode mode) {}
    void set_color(Color color) {
        intensity_red_ = intensity_color_[color][0];
        intensity_green_ = intensity_color_[color][1];
        intensity_blue_ = intensity_color_[color][2];
    }
    void set_rate(float frequency) {}
    void update() {
        // TODO figure out where to put 10000
        if (i++ >= 10000) {
            i = 0;
        }
        float t = i*(1.f/10000.f);
        float intensity = t < 0.5 ? 2*t : 2-2*t;
        uint16_t intensity_period = 65535*brightness_*intensity*intensity*intensity;
        *red_reg_ = intensity_red_* intensity_period;
        *green_reg_ = intensity_green_ * intensity_period;
        *blue_reg_ = intensity_blue_ * intensity_period;
    }
 private:
    uint16_t i = 0;
    float brightness_;
    uint16_t *const red_reg_;
    uint16_t *const green_reg_;
    uint16_t *const blue_reg_;
    float intensity_color_[WHITE+1][3] = {{1,0,0}, {1,.5,0}, {1,1,0}, {0.5,1,0}, {0,1,0}, {0,1,.5}, {0,1,1}, {0,.5,1}, {0,0,1}, {.5,0,1}, {1,0,1}, {1,0,.5}, {1,1,1}};
    float intensity_red_ = 1;
    float intensity_green_ = 1;
    float intensity_blue_ = 1;
};

#endif