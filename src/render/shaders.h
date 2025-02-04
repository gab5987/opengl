#ifndef RENDER_SHADERS_H
#define RENDER_SHADERS_H

#include "glm/ext/matrix_float4x4.hpp"
#include <span>
#include <string>
#include <unordered_map>

#include <glad/glad.h>
#include <glm/ext/vector_float3.hpp>
#include <glm/vec3.hpp>

namespace engine
{
class shader
{
    public:
    enum class type : uint8_t
    {
        vertex,
        fragment,
    };

    private:
    const type  _type;
    std::string _source;

    unsigned int _shader;

    void retrive_file(const std::string &path);

    static unsigned int gl_type_conv(type type);

    public:
    unsigned int get() const;

    explicit shader(type type, const std::string &path);
};

template <typename T> struct un_transf
{
    static_assert(false, "type deduction failed for uniform transfer");
};

template <> struct un_transf<int>
{
    inline static auto fun = [](int loc, int data) -> void {
        glUniform1i(loc, data);
    };
};

template <> struct un_transf<glm::vec3>
{
    inline static auto fun = [](int loc, const glm::vec3 &vec) -> void {
        glUniform3f(loc, vec.x, vec.y, vec.z);
    };
};

template <> struct un_transf<glm::mat4>
{
    inline static auto fun = [](int loc, const glm::mat4 &mat) -> void {
        glUniformMatrix4fv(loc, 1, GL_FALSE, &(mat[0][0]));
    };
};

class program
{
    std::span<shader> _shaders;
    unsigned int      _id;

    std::unordered_map<std::string, int> _uniform_cache;

    int get_uniform_location(const std::string &name);

    public:
    template <typename T>
    void set_uniform(const std::string &name, const T &data)
    {
        const int location = this->get_uniform_location(name);
        if (location == -1)
        {
            return;
        }

        un_transf<T>::fun(location, data);
    }

    unsigned int get() const;
    void         use() const;

    explicit program(std::span<shader> &&shaders);
    ~program();
};
}; // namespace engine

#endif

