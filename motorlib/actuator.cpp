#include "actuator.h"

void Actuator::run() {
    int i = 0;
    while(1) {
        i++;
        fast_loop_.maintenance();
    }
}