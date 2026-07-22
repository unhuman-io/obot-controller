template<typename Cfg>
struct System {
    using MainLoop = typename Cfg::template MainLoopType<System>;
    using FastLoop = typename Cfg::template FastLoopType<System>;
    
    static inline constinit MainLoop main_loop{};
    static inline constinit FastLoop fast_loop{};
};
