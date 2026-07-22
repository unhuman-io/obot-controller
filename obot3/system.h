template<typename Cfg>
struct System {
    using MainLoop = typename Cfg::template MainLoopType<System>;
    using FastLoop = typename Cfg::template FastLoopType<System>;
};
