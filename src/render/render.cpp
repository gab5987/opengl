#include <glad/glad.h>

#include "render/buffer.h"
#include "render/render.h"
#include "render/shaders.h"

void engine::render::clear() const
{
    glClear(GL_COLOR_BUFFER_BIT);
};

void engine::render::draw(
    const varr &vao, const ibuff &ebo, const program &prog) const
{
    prog.use();
    vao.bind();
    ebo.bind();
    glDrawElements(GL_TRIANGLES, (int)ebo.count(), GL_UNSIGNED_INT, nullptr);
}
