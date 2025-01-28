#include <array>
#include <memory>
#include <span>

#include <spdlog/spdlog.h>

#include "render/buffer.h"
#include "render/shaders.h"
#include "render/window.h"

int main(int _argc, char *_argv[])
{
    spdlog::info(
        "spdlog version {}.{}.{}", SPDLOG_VER_MAJOR, SPDLOG_VER_MINOR,
        SPDLOG_VER_PATCH);

    auto window = std::make_unique<render::window>();

    const char *gl_version = (char *)(glGetString(GL_VERSION));
    spdlog::info("OpenGL {}", gl_version);

    using shader_type = render::shader::type;

    std::array<render::shader, 2> shaders{
        render::shader{shader_type::vertex, "data/shaders/basic_vert.glsl"},
        render::shader{shader_type::fragment, "data/shaders/basic_frag.glsl"}};

    auto prog = std::make_unique<render::program>(std::span{shaders});

    float vertices[]{
        0.0F,  0.0F,  0.0F, // Middle point

        -0.25, 0.25,  0.0F, // left top
        0.25,  0.25,  0.0F, // right top

        -0.25, -0.25, 0.0F, // left bottom
        0.25,  -0.25, 0.0F, // right bottm
    };

    unsigned int indices[]{0, 1, 2, 0, 3, 4};

    render::varr  vao{};
    render::vbuff vbo{vertices, sizeof(vertices)};
    render::ibuff ebo{indices, sizeof(indices) / sizeof(indices[0])};

    render::layout layout;
    layout.push<float>(3);

    vao.add_layout(vbo, layout);

    // to draw in wireframe polygons.
    render::window::wireframe();

    prog->use();

    const int location = glGetUniformLocation(prog->get(), "un_color");
    glUniform3f(location, 1.0F, 1.0F, 1.0F);

    float red = 0.0F;
    float inc = 0.005F;

    while (glfwWindowShouldClose(window->get()) == 0)
    {
        // input
        // -----
        if (glfwGetKey(window->get(), GLFW_KEY_ESCAPE) == GLFW_PRESS)
        {
            glfwSetWindowShouldClose(window->get(), 1);
        }

        glClearColor(0.2F, 0.3F, 0.3F, 1.0F);
        glClear(GL_COLOR_BUFFER_BIT);

        prog->use();
        glUniform3f(location, red, 0, 0);

        vao.bind();
        ebo.bind();

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        if (red >= 0.99F || red <= -0.99F)
        {
            inc = -1 * inc;
        }
        red += inc;

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse
        // moved etc.)
        window->swap();
        glfwPollEvents();
    }

    return 0;
}

