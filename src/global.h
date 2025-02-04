#ifndef GLOBAL_H
#define GLOBAL_H

namespace engine
{
class render;
class window;
}; // namespace engine

struct global
{
    engine::render *render;
    engine::window *window;

    float fps = 0.0F;

    ~global()               = default;
    auto operator=(auto &)  = delete;
    bool operator==(auto &) = delete;
};

inline static global *gstate = nullptr;

#endif

