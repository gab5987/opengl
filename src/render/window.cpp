#include <cstdlib>

#include <spdlog/spdlog.h>

// clang-format off
#include <glad/glad.h>
#include <GLFW/glfw3.h>
// clang-format on

#include "render/window.h"

void engine::window::swap() const
{
    glfwSwapBuffers(this->_win);
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
            // specified on high res displays.
            glViewport(0, 0, width, height);
            spdlog::info(
                "window::glfwSetFramebufferSizeCallback(_win, <lambda>): "
                "viewport resized: (x:{} y:{})",
                width, height);
        });

    // glad: load all OpenGL function pointers
    if (gladLoadGLLoader((GLADloadproc)glfwGetProcAddress) == 0)
    {
        spdlog::error("Failed to initialize GLAD");
        exit(-1);
    }

    const char *gl_version = (char *)(glGetString(GL_VERSION));
    spdlog::info("OpenGL {}", gl_version);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

engine::window::~window()
{
    glfwTerminate();
    spdlog::warn("Window terminated");
}
