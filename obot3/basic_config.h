#include "loops.h"
#include "basic_foc_controller.h"

struct BasicConfig {
    static constexpr int fast_loop_frequency = 1000;
    static constexpr int main_loop_frequency = 100;
    
    struct MainLoopConfig {
        static constexpr int frequency = main_loop_frequency;
    };
    
    template <typename System>
    using MainLoopType = MainLoop<System, MainLoopConfig>;

    struct FastLoopConfig {
        static constexpr int frequency = fast_loop_frequency;
        using ControllerType = BasicFOCController;
        static constexpr int kp = 3;
    };
    
    template <typename System>
    using FastLoopType = FastLoop<System, FastLoopConfig{}>;

};
