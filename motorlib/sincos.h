struct Sincos {
    float sin, cos;
};

Sincos sincos1(float x)  __attribute__((section (".ccmram")));;