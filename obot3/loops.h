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
    typename T::ControllerType;
};

template <typename System, IsFastLoopConfig auto config>
struct FastLoop {
    using Controller = typename decltype(config)::ControllerType;
    static inline Controller controller{config.kp};
    static void update() {
        System::MainLoop::get_command();
        controller.update();
    }
    static void get_status() {}
};