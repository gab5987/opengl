#include <array>
#include <memory>
#include <numeric>
#include <span>

#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/vector_float3.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <imgui.h>
#include <spdlog/spdlog.h>

#include "engine/buffer.h"
#include "engine/camera.h"
#include "engine/render.h"
#include "engine/shaders.h"
#include "engine/texture.h"
#include "engine/window.h"
#include "osd/osd.h"
#include "osd/scene.h"

int main(int _argc, char *_argv[])
{
    engine::render render{};
    engine::window window{};

    osd::osd   osd{window};
    osd::scene scene{osd};

    engine::camera camera{window};

    using shader_type = engine::shader::type;

    std::array shaders{
        engine::shader{shader_type::vertex, "data/shaders/basic_vert.glsl"},
        engine::shader{shader_type::fragment, "data/shaders/basic_frag.glsl"}};

    engine::program prog{std::span{shaders}};

    struct [[gnu::packed]] vertex
    {
        const glm::vec3 texel_pos;
        glm::vec2       texture_pos;
    };

    // vertex vertices[]{
    //     {{0.5f, 0.5f, 0.0f}, {1.0f, 1.0f}},
    //     {{0.5f, -0.5f, 0.0f}, {1.0f, 0.0f}},
    //     {{-0.5f, -0.5f, 0.0f}, {0.0f, 0.0f}},
    //     {{-0.5f, 0.5f, 0.0f}, {0.0f, 1.0f}},
    // };

    vertex vertices[] = {
        {{-0.5f, -0.5f, -0.5f}, {0.0f, 0.0f}},
        {{0.5f, -0.5f, -0.5f}, {1.0f, 0.0f}},
        {{0.5f, 0.5f, -0.5f}, {1.0f, 1.0f}},
        {{0.5f, 0.5f, -0.5f}, {1.0f, 1.0f}},
        {{-0.5f, 0.5f, -0.5f}, {0.0f, 1.0f}},
        {{-0.5f, -0.5f, -0.5f}, {0.0f, 0.0f}},

        {{-0.5f, -0.5f, 0.5f}, {0.0f, 0.0f}},
        {{0.5f, -0.5f, 0.5f}, {1.0f, 0.0f}},
        {{0.5f, 0.5f, 0.5f}, {1.0f, 1.0f}},
        {{0.5f, 0.5f, 0.5f}, {1.0f, 1.0f}},
        {{-0.5f, 0.5f, 0.5f}, {0.0f, 1.0f}},
        {{-0.5f, -0.5f, 0.5f}, {0.0f, 0.0f}},

        {{-0.5f, 0.5f, 0.5f}, {1.0f, 0.0f}},
        {{-0.5f, 0.5f, -0.5f}, {1.0f, 1.0f}},
        {{-0.5f, -0.5f, -0.5f}, {0.0f, 1.0f}},
        {{-0.5f, -0.5f, -0.5f}, {0.0f, 1.0f}},
        {{-0.5f, -0.5f, 0.5f}, {0.0f, 0.0f}},
        {{-0.5f, 0.5f, 0.5f}, {1.0f, 0.0f}},

        {{0.5f, 0.5f, 0.5f}, {1.0f, 0.0f}},
        {{0.5f, 0.5f, -0.5f}, {1.0f, 1.0f}},
        {{0.5f, -0.5f, -0.5f}, {0.0f, 1.0f}},
        {{0.5f, -0.5f, -0.5f}, {0.0f, 1.0f}},
        {{0.5f, -0.5f, 0.5f}, {0.0f, 0.0f}},
        {{0.5f, 0.5f, 0.5f}, {1.0f, 0.0f}},

        {{-0.5f, -0.5f, -0.5f}, {0.0f, 1.0f}},
        {{0.5f, -0.5f, -0.5f}, {1.0f, 1.0f}},
        {{0.5f, -0.5f, 0.5f}, {1.0f, 0.0f}},
        {{0.5f, -0.5f, 0.5f}, {1.0f, 0.0f}},
        {{-0.5f, -0.5f, 0.5f}, {0.0f, 0.0f}},
        {{-0.5f, -0.5f, -0.5f}, {0.0f, 1.0f}},

        {{-0.5f, 0.5f, -0.5f}, {0.0f, 1.0f}},
        {{0.5f, 0.5f, -0.5f}, {1.0f, 1.0f}},
        {{0.5f, 0.5f, 0.5f}, {1.0f, 0.0f}},
        {{0.5f, 0.5f, 0.5f}, {1.0f, 0.0f}},
        {{-0.5f, 0.5f, 0.5f}, {0.0f, 0.0f}},
        {{-0.5f, 0.5f, -0.5f}, {0.0f, 1.0f}},
    };

    glm::vec3 cube_positions[] = {
        glm::vec3(2.0f, 5.0f, -15.0f),   glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f), glm::vec3(2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f, 3.0f, -7.5f),   glm::vec3(1.3f, -2.0f, -2.5f),
        glm::vec3(1.5f, 2.0f, -2.5f),    glm::vec3(1.5f, 0.2f, -1.5f),
        glm::vec3(-1.3f, 1.0f, -1.5f)};

    unsigned int indices[std::size(vertices)]{};
    std::iota(std::begin(indices), std::end(indices), 0);

    engine::varr  vao{};
    engine::vbuff vbo{vertices, sizeof(vertices)};
    engine::ibuff ebo{indices, std::size(indices)};

    engine::layout layout;
    layout.push<float>(3);
    layout.push<float>(2);

    vao.add_layout(vbo, layout);

    const engine::texture texture{"data/textures/box.png"};
    texture.bind();

    while (!window.should_close())
    {
        render.clear();
        camera.update();

        prog.use();
        prog.set_uniform("u_texture", 0);

        // create transformations
        glm::mat4 model{1.0f};
        glm::mat4 view = camera.get_view_matrix();
        glm::mat4 projection;

        projection = glm::perspective(
            glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

        for (unsigned int i = 0; i < std::size(cube_positions); i++)
        {
            model = glm::mat4(1.0f);
            model = glm::translate(model, cube_positions[i]);

            float angle =
                static_cast<float>(glfwGetTime()) * (i % 2 ? -1.0f : 1.0f);
            model = glm::rotate(model, angle, glm::vec3(0.5f, 1.0f, 0.0f));

            angle = 20.0f * i;
            model = glm::rotate(
                model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));

            prog.set_uniform("u_view", view);
            prog.set_uniform("u_proj", projection);
            prog.set_uniform("u_model", model);

            render.draw(vao, ebo, prog);
        }

        osd.draw();
        window.update();
    }

    return 0;
}
