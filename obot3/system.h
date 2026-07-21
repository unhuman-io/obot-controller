template<typename Cfg>
struct System {
    using MainLoop = typename Cfg::MainLoopType<System>;
    using FastLoop = typename Cfg::FastLoopType<System>;
};
