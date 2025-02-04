#include <array>
#include <memory>
#include <span>

#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/vector_float3.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <imgui.h>
#include <spdlog/spdlog.h>

#include "global.h"
#include "osd/osd.h"
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

    engine::render render{};
    engine::window window{};

    osd::osd osd{window};

    gstate = new global{&render, &window};

    const char *gl_version = (char *)(glGetString(GL_VERSION));
    spdlog::info("OpenGL {}", gl_version);

    using shader_type = engine::shader::type;

    std::array<engine::shader, 2> shaders{
        engine::shader{shader_type::vertex, "data/shaders/basic_vert.glsl"},
        engine::shader{shader_type::fragment, "data/shaders/basic_frag.glsl"}};

    engine::program prog{std::span{shaders}};

    osd.add_ctx("FrameRate", [](auto &_io) {
        ImGui::Text(
            "Application average %.3f ms/frame (%.1f FPS)",
            1000.0F / _io.Framerate, _io.Framerate);
    });

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

    glm::mat4 ortho = glm::ortho(-2.0F, 2.0F, -1.5F, 1.5F, -1.0F, 1.0F);

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
    prog.set_uniform("u_MVP", ortho);

    // float red = 0.0F;
    // float inc = 0.005F;

    while (glfwWindowShouldClose(window.get()) == 0)
    {
        // input
        // -----
        if (glfwGetKey(window.get(), GLFW_KEY_ESCAPE) == GLFW_PRESS)
        {
            glfwSetWindowShouldClose(window.get(), 1);
        }

        glClearColor(0.2F, 0.3F, 0.3F, 1.0F);
        glClear(GL_COLOR_BUFFER_BIT);

        prog.use();
        // prog.set_uniform("un_color", glm::vec3{red, 0, 0});

        render.draw(vao, ebo, prog);

        // if (red >= 0.99F || red <= -0.99F)
        // {
        //     inc = -1 * inc;
        // }
        // red += inc;

        glm::mat4 trans = ortho;
        // trans           = glm::translate(trans, glm::vec3{0.5F, -0.5F,
        // 0.0F});
        trans = glm::rotate(
            trans, (float)glfwGetTime(), glm::vec3{0.0F, 0.0F, 1.0F});

        prog.set_uniform("u_MVP", trans);

        osd.draw();

        // glfw: swap buffers and poll IO events (keys pressed/released,
        // mouse moved etc.)
        window.swap();
        glfwPollEvents();
    }

    return 0;
}

