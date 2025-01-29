#include <array>
#include <memory>
#include <span>

#include <glm/ext/vector_float3.hpp>
#include <spdlog/spdlog.h>

#include "render/buffer.h"
#include "render/render.h"
#include "render/shaders.h"
#include "render/texture.h"
#include "render/window.h"

int main(int _argc, char *_argv[])
{
    spdlog::info(
        "spdlog version {}.{}.{}", SPDLOG_VER_MAJOR, SPDLOG_VER_MINOR,
        SPDLOG_VER_PATCH);

    auto window = std::make_unique<engine::window>();

    const char *gl_version = (char *)(glGetString(GL_VERSION));
    spdlog::info("OpenGL {}", gl_version);

    using shader_type = engine::shader::type;

    std::array<engine::shader, 2> shaders{
        engine::shader{shader_type::vertex, "data/shaders/basic_vert.glsl"},
        engine::shader{shader_type::fragment, "data/shaders/basic_frag.glsl"}};

    engine::program prog{std::span{shaders}};

    // glm::vec3 vertices[]{
    //     {0.0F, 0.0F, 0.0F}, // Middle point
    //
    //     {-0.25, 0.25, 0.0F}, // left top
    //     {0.25, 0.25, 0.0F},  // right top
    //
    //     {-0.25, -0.25, 0.0F}, // left bottom
    //     {0.25, -0.25, 0.0F},  // right bottm
    // };
    //
    // unsigned int indices[]{0, 1, 2, 0, 3, 4};

    float vertices[] = {
        -0.5F, -0.5F, 0.0F, 0.0F, // 0
        0.5F,  -0.5F, 1.0F, 0.0F, // 1
        0.5F,  0.5F,  1.0F, 1.0F, // 2
        -0.5F, 0.5F,  0.0F, 1.0F  // 3
    };

    unsigned int indices[] = {0, 1, 2, 2, 3, 0};

    engine::varr  vao{};
    engine::vbuff vbo{vertices, sizeof(vertices)};
    engine::ibuff ebo{indices, sizeof(indices) / sizeof(indices[0])};

    engine::layout layout;
    // layout.push<float>(3);
    layout.push<float>(2);
    layout.push<float>(2);

    vao.add_layout(vbo, layout);

    // to draw in wireframe polygons.
    // engine::window::wireframe();

    prog.use();

    engine::texture texture{"data/textures/phone.png"};
    texture.bind();

    prog.set_uniform("u_Texture", 0);

    float red = 0.0F;
    float inc = 0.005F;

    engine::render render{};

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

        prog.use();
        prog.set_uniform("un_color", glm::vec3{red, 0, 0});

        render.draw(vao, ebo, prog);

        if (red >= 0.99F || red <= -0.99F)
        {
            inc = -1 * inc;
        }
        red += inc;

        // glfw: swap buffers and poll IO events (keys pressed/released,
        // mouse moved etc.)
        window->swap();
        glfwPollEvents();
    }

    return 0;
}

