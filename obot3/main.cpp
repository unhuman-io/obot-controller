#include <concepts>

struct BasicFOCController {
    static void update() {}
};

template <typename System, typename Config>
struct MainLoop {
    static void update() {
        System::FastLoop::get_status();
    }
    static void get_command() {}
};


template <typename T>
concept IsFastLoopConfig = requires(T config) {
    { config.frequency } -> std::convertible_to<int>;
    { config.controller.update() };
};

template <typename System, IsFastLoopConfig auto config>
struct FastLoop {
    static void update() {
        System::MainLoop::get_command();
        config.controller.update();
    }
    static void get_status() {}
};

struct Config {
    static constexpr int fast_loop_frequency = 1000;
    static constexpr int main_loop_frequency = 100;
    
    struct MainLoopConfig {
        static constexpr int frequency = main_loop_frequency;
    };
    
    template <typename System>
    using MainLoopType = MainLoop<System, MainLoopConfig>;

    struct FastLoopConfig {
        static constexpr int frequency = fast_loop_frequency;
        BasicFOCController controller;
    };
    
    template <typename System>
    using FastLoopType = FastLoop<System, FastLoopConfig{}>;

};


template<typename Cfg>
struct System {
    using MainLoop = typename Cfg::MainLoopType<System>;
    using FastLoop = typename Cfg::FastLoopType<System>;
};

using MySystem = System<Config>;

int main() {

    while(1) {
        MySystem::FastLoop::update();
        MySystem::MainLoop::update();
    }
    return 0;
}

extern "C" {
void _close_r() {}
void _getpid_r() {}
void _kill_r() {}
void _lseek_r() {}
void _read_r() {}
void _write_r() {}
}
