#ifndef ENGINE_BUFFER_H
#define ENGINE_BUFFER_H

#include <cstdint>
#include <iterator>
#include <type_traits>
#include <vector>

#include <glad/glad.h>
#include <glm/vec3.hpp>
#include <spdlog/spdlog.h>

namespace engine
{
template <unsigned int TYPE> class buffer
{
    protected:
    unsigned int _id;

    public:
    void bind() const
    {
        glBindBuffer(TYPE, this->_id);
    };

    void unbind() const
    {
        glBindBuffer(TYPE, 0);
    }

    buffer(const void *data, const unsigned int size)
    {
        glGenBuffers(1, &this->_id);
        glBindBuffer(TYPE, this->_id);
        glBufferData(TYPE, size, data, GL_STATIC_DRAW);

        spdlog::info(
            "buffer<{:0x}>(void*, {:0d}): _id:{:0x}", TYPE, size, this->_id);
    }

    ~buffer()
    {
        spdlog::info("~buffer<{:0x}>(): _id:{:0x}", TYPE, this->_id);
        glDeleteBuffers(1, &this->_id);
    }
};

class vbuff : public buffer<GL_ARRAY_BUFFER>
{
    public:
    vbuff(const void *data, unsigned int size);
    ~vbuff() = default;
};

class ibuff : public buffer<GL_ELEMENT_ARRAY_BUFFER>
{
    unsigned int _count;

    public:
    unsigned int count() const;

    ibuff(const unsigned *idx, unsigned int count);
    ~ibuff() = default;
};

struct element
{
    const GLenum       type;
    const int          count;
    const unsigned int normalize;

    unsigned int        type_size() const;
    static unsigned int type_size(GLenum type);
};

#define ELM_TYPE_DEF(type, gl)              \
    template <> struct gltype<type>         \
    {                                       \
        static constexpr GLenum VALUE = gl; \
    };

template <typename T> struct gltype
{
    static constexpr std::unreachable_sentinel_t VALUE{};
};

ELM_TYPE_DEF(float, GL_FLOAT);
ELM_TYPE_DEF(char, GL_BYTE);
ELM_TYPE_DEF(unsigned char, GL_UNSIGNED_BYTE);
ELM_TYPE_DEF(int16_t, GL_SHORT);
ELM_TYPE_DEF(uint16_t, GL_UNSIGNED_SHORT);
ELM_TYPE_DEF(int, GL_INT);
ELM_TYPE_DEF(unsigned int, GL_UNSIGNED_INT);

class layout
{
    int                  _stride = 0;
    std::vector<element> _elements;

    template <typename T> static consteval GLenum form_type()
    {
        constexpr bool INVALID = std::is_same_v<
            decltype(gltype<T>::VALUE), std::unreachable_sentinel_t>;

        static_assert(!INVALID, "Buffer type could not be resolved");

        return gltype<T>::VALUE;
    };

    public:
    [[nodiscard]] int  stride() const;
    [[nodiscard]] auto elements() const -> const decltype(_elements) &;

    template <typename T> void push(int count = 1)
    {
        const GLenum       type = form_type<T>();
        const unsigned int norm =
            (type == GL_BYTE || type == GL_UNSIGNED_BYTE) ? GL_TRUE : GL_FALSE;

        this->_elements.push_back({type, count, norm});
        this->_stride += sizeof(type) * count;
        spdlog::info(
            "layout::push<{:0x}>({:0d}): _stride:{:0d}", type, count,
            this->_stride);
    }

    layout()  = default;
    ~layout() = default;
}; // namespace render

class varr
{
    unsigned int _id;

    public:
    void add_layout(const vbuff &vbo, const layout &layout);

    void bind() const;
    void unbind() const;

    varr();
    ~varr();
};
}; // namespace engine

#endif
