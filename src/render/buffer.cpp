#include <cstddef>
#include <spdlog/spdlog.h>

#include "render/buffer.h"

render::vbuff::vbuff(const void *data, const unsigned int size)
    : buffer<GL_ARRAY_BUFFER>(data, size)
{
}

render::ibuff::ibuff(const unsigned *idx, const unsigned int count)
    : buffer<GL_ELEMENT_ARRAY_BUFFER>(idx, count * sizeof(unsigned int))
{
}

unsigned int render::element::type_size(GLenum type)
{
#define ELM_TYPE_SIZE(T, I) \
    case T: {               \
        return sizeof(I);   \
    }

    switch (type)
    {
        ELM_TYPE_SIZE(GL_FLOAT, GLfloat)
        ELM_TYPE_SIZE(GL_BYTE, GLbyte)
        ELM_TYPE_SIZE(GL_UNSIGNED_BYTE, GLubyte)
        ELM_TYPE_SIZE(GL_SHORT, GLshort)
        ELM_TYPE_SIZE(GL_UNSIGNED_SHORT, GLushort)
        ELM_TYPE_SIZE(GL_INT, GLint)
        ELM_TYPE_SIZE(GL_UNSIGNED_INT, GLuint)

        default:
            break;
    }

    return 0;
}

unsigned int render::element::type_size() const
{
    return type_size(this->type);
}

auto render::layout::elements() const -> const decltype(_elements) &
{
    return this->_elements;
}

int render::layout::stride() const
{
    return this->_stride;
}

void render::varr::add_layout(const vbuff &vbo, const layout &layout)
{
    this->bind();
    vbo.bind();

    const auto &elements = layout.elements();

    unsigned int offset = 0;
    for (int i = 0; i < elements.size(); ++i)
    {
        const element &elm = elements.at(i);
        glEnableVertexAttribArray(i);
        glVertexAttribPointer(
            i, elm.count, elm.type, elm.normalize, layout.stride(),
            (void *)offset);

        offset += elm.count * elm.type_size();
    }
}

render::varr::varr()
{
    glGenVertexArrays(1, &this->_id);
    spdlog::info("varr(): _id:{:0d}", this->_id);

    glBindVertexArray(this->_id);
}

render::varr::~varr()
{
    spdlog::info("~varr(): _id:{:0d}", this->_id);
    glDeleteVertexArrays(1, &this->_id);
}

void render::varr::bind() const
{
    glBindVertexArray(this->_id);
};

void render::varr::unbind() const
{
    glBindVertexArray(0);
}
