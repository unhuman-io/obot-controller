#include "fast_loop.h"

class Actuator {
 public:
    Actuator(FastLoop &fast_loop) : fast_loop_(fast_loop) {}
    void run();
 private:
    FastLoop &fast_loop_;
};