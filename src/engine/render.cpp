#include <glad/glad.h>

#include "engine/buffer.h"
#include "engine/render.h"
#include "engine/shaders.h"

void engine::render::clear() const
{
    glClearColor(0.2F, 0.3F, 0.3F, 1.0F);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
};

void engine::render::draw(
    const varr &vao, const ibuff &ebo, const program &prog) const
{
    prog.use();
    vao.bind();
    ebo.bind();
    glDrawElements(
        GL_TRIANGLES, static_cast<int>(ebo.count()), GL_UNSIGNED_INT, nullptr);
}
