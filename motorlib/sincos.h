#pragma once

struct Sincos {
    float sin, cos;
};

// input x in radians
Sincos sincos1(float x)  __attribute__((section (".ccmram")));;