#ifndef ENGINE_WINDOW_H
#define ENGINE_WINDOW_H

#include <GLFW/glfw3.h>

#include "engine/keyboard.h"

#include <glm/glm.hpp>

namespace engine
{
class window
{
    public:
    // settings
    static constexpr unsigned int WIDTH  = 800;
    static constexpr unsigned int HEIGHT = 600;

    private:
    GLFWwindow *_win = nullptr;

    float _last_mouse_x = WIDTH / 2.0f;
    float _last_mouse_y = HEIGHT / 2.0f;

    typedef std::function<void(uint32_t)>             kb_fn;
    typedef std::function<void(glm::vec2, glm::vec2)> mouse_fn;

    std::vector<kb_fn>    _keypad_callback;
    std::vector<mouse_fn> _mouse_callback;

    void mouse_callback(float x_pos, float y_pos);

    inline static window *self = nullptr;

    public:
    void update();

    void add_keypad_callback(kb_fn &&func);
    void add_mouse_callback(mouse_fn &&func);

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
