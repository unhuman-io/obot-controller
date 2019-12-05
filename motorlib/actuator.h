template <class PWM, class Encoder>
class FastLoop;
class MainLoop;

template <class PWM, class Encoder>
class Actuator {
 public:
    Actuator(FastLoop<PWM, Encoder> &fast_loop, MainLoop &main_loop) : fast_loop_(fast_loop), main_loop_(main_loop) {}
    void run();
 private:
    FastLoop<PWM, Encoder> &fast_loop_;
    MainLoop &main_loop_;
};