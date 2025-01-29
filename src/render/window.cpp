#include <cstdlib>
#include <iostream>

// clang-format off
#include <glad/glad.h>
#include <GLFW/glfw3.h>
// clang-format on

#include "render/window.h"

void engine::window::swap() const
{
    glfwSwapBuffers(this->_win);
}

void engine::window::wireframe()
{
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
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

    this->_win = glfwCreateWindow(WIDTH, HEIGHT, "OpenGL", nullptr, nullptr);
    if (this->_win == nullptr)
    {
        std::cout << "Failed to create GLFW window\n";

        glfwTerminate();
        exit(-1);
    }

    glfwMakeContextCurrent(this->_win);
    glfwSetFramebufferSizeCallback(
        this->_win, [](GLFWwindow *_win, int width, int height) {
            // make sure the viewport matches the new window dimensions;
            // that width and height will be significantly larger than
            // specified on high res displays.
            glViewport(0, 0, width, height);
        });

    // glad: load all OpenGL function pointers
    if (gladLoadGLLoader((GLADloadproc)glfwGetProcAddress) == 0)
    {
        std::cout << "Failed to initialize GLAD\n";
        exit(-1);
    }
}

engine::window::~window()
{
    glfwTerminate();
}
