#include <cstddef>
#include <spdlog/spdlog.h>

#include "engine/buffer.h"

engine::vbuff::vbuff(const void *data, const unsigned int size)
    : buffer<GL_ARRAY_BUFFER>(data, size)
{
}

engine::ibuff::ibuff(const unsigned *idx, const unsigned int count)
    : buffer<GL_ELEMENT_ARRAY_BUFFER>(idx, count * sizeof(unsigned int)),
      _count(count)
{
}

unsigned int engine::ibuff::count() const
{
    return this->_count;
}

unsigned int engine::element::type_size(GLenum type)
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

unsigned int engine::element::type_size() const
{
    return type_size(this->type);
}

auto engine::layout::elements() const -> const decltype(_elements) &
{
    return this->_elements;
}

int engine::layout::stride() const
{
    return this->_stride;
}

void engine::varr::add_layout(const vbuff &vbo, const layout &layout)
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

engine::varr::varr()
{
    glGenVertexArrays(1, &this->_id);
    spdlog::info("varr(): _id:{:0d}", this->_id);

    glBindVertexArray(this->_id);
}

engine::varr::~varr()
{
    spdlog::info("~varr(): _id:{:0d}", this->_id);
    glDeleteVertexArrays(1, &this->_id);
}

void engine::varr::bind() const
{
    glBindVertexArray(this->_id);
}

void engine::varr::unbind() const
{
    glBindVertexArray(0);
}
