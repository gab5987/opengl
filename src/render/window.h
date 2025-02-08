#ifndef RENDER_WINDOW_H
#define RENDER_WINDOW_H

#include <GLFW/glfw3.h>

namespace engine
{
class window
{
    GLFWwindow *_win = nullptr;

    public:
    // settings
    static constexpr unsigned int WIDTH  = 800;
    static constexpr unsigned int HEIGHT = 600;

    void swap() const;

    static void normal_mode();
    static void wireframe_mode();

    [[nodiscard]] bool should_close() const;

    [[nodiscard]] float get_dpi_scale() const;

    [[nodiscard]] GLFWwindow *get() const;

    window();
    ~window();
};
}; // namespace engine

#endif
