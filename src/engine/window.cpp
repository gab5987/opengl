#include <cstdlib>

#include <spdlog/spdlog.h>

// clang-format off
#include <glad/glad.h>
#include <GLFW/glfw3.h>
// clang-format on

#include "engine/window.h"

#include <bits/ranges_algo.h>

#define WINDOW_SET_IF_PRESSED(GL_KEY, KEY)         \
    (glfwGetKey(this->_win, GL_KEY) == GLFW_PRESS) \
        ? static_cast<uint32_t>(KEY)               \
        : 0

void engine::window::mouse_callback(float x_pos, float y_pos)
{
    const float delta_x = x_pos - this->_last_mouse_x;
    const float delta_y = this->_last_mouse_y - y_pos;

    this->_last_mouse_x = x_pos;
    this->_last_mouse_y = y_pos;

    const glm::vec2 deltas{delta_x, delta_y};
    const glm::vec2 absolute{x_pos, y_pos};

    auto iter = [&](const mouse_fn &callback) -> void {
        callback(deltas, absolute);
    };
    std::ranges::for_each(this->_mouse_callback, iter);
}

void engine::window::update()
{
    glfwPollEvents();

    uint32_t pressed = static_cast<uint32_t>(key::none);
    pressed |= WINDOW_SET_IF_PRESSED(GLFW_KEY_W, key::front);
    pressed |= WINDOW_SET_IF_PRESSED(GLFW_KEY_S, key::back);
    pressed |= WINDOW_SET_IF_PRESSED(GLFW_KEY_A, key::left);
    pressed |= WINDOW_SET_IF_PRESSED(GLFW_KEY_D, key::right);

    if (glfwGetKey(this->_win, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(this->_win, true);
    }

    auto iter = [pressed](const kb_fn &callback) -> void { callback(pressed); };
    std::ranges::for_each(this->_keypad_callback, iter);

    glfwSwapBuffers(this->_win);
}

void engine::window::add_keypad_callback(kb_fn &&func)
{
    this->_keypad_callback.emplace_back(std::move(func));
}

void engine::window::add_mouse_callback(mouse_fn &&func)
{
    this->_mouse_callback.emplace_back(std::move(func));
}

void engine::window::normal_mode()
{
    spdlog::info("window::normal_mode()");
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void engine::window::wireframe_mode()
{
    spdlog::info("window::wireframe_mode()");
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}
bool engine::window::should_close() const
{
    return glfwWindowShouldClose(this->_win) != 0;
}

float engine::window::get_dpi_scale() const
{
    float xscale = 0.0F;
    float yscale = 0.0F;

    glfwGetWindowContentScale(this->_win, &xscale, &yscale);

    spdlog::info("window::get_dpi_scale(): xscale:{}", xscale);

    return xscale;
}

GLFWwindow *engine::window::get() const
{
    return this->_win;
}

engine::window::window()
{
    self = this;

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_SCALE_TO_MONITOR, GLFW_TRUE);

    this->_win = glfwCreateWindow(WIDTH, HEIGHT, "OpenGL", nullptr, nullptr);
    if (this->_win == nullptr)
    {
        spdlog::error("Failed to create GLFW window");

        glfwTerminate();
        exit(-1);
    }

    glfwMakeContextCurrent(this->_win);
    glfwSwapInterval(1); // Enable vsync

    glfwSetFramebufferSizeCallback(
        this->_win, [](GLFWwindow *_win, int width, int height) {
            // make sure the viewport matches the new window dimensions;
            // that width and height will be significantly larger than
            // specified on high-res displays.
            glViewport(0, 0, width, height);
            spdlog::info(
                "window::glfwSetFramebufferSizeCallback(_win, <lambda>): "
                "viewport resized: (x:{} y:{})",
                width, height);
        });

    // glad: load all OpenGL function pointers
    if (gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)) ==
        0)
    {
        spdlog::error("Failed to initialize GLAD");
        exit(-1);
    }

    const char *gl_version =
        reinterpret_cast<const char *>(glGetString(GL_VERSION));
    spdlog::info("OpenGL {}", gl_version);

    glfwSetInputMode(this->_win, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glfwSetCursorPosCallback(
        this->_win, [](auto *_, const double x, const double y) -> void {
            self->mouse_callback(x, y);
        });
}

engine::window::~window()
{
    glfwTerminate();
    spdlog::warn("Window terminated");
}
