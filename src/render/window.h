#ifndef RENDER_WINDOW_H
#define RENDER_WINDOW_H

// clang-format off
#include <glad/glad.h>
#include <GLFW/glfw3.h>
// clang-format on

namespace engine
{
class window
{
    GLFWwindow *_win = nullptr;

    public:
    // settings
    static constexpr unsigned int WIDTH  = 800 * 2;
    static constexpr unsigned int HEIGHT = 600 * 2;

    void        swap() const;
    static void wireframe();

    GLFWwindow *get() const;

    window();
    ~window();
};
}; // namespace engine

#endif

