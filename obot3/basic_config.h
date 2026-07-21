#include "loops.h"
#include "basic_foc_controller.h"

struct BasicLoopsConfig {
    int fast_loop_frequency = 100'000;
    int main_loop_frequency = 10'000;
};

template<BasicLoopsConfig loop_config = BasicLoopsConfig{}>
struct BasicConfig {
    static constexpr int fast_loop_frequency = loop_config.fast_loop_frequency;
    static constexpr int main_loop_frequency = loop_config.main_loop_frequency;
    
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
