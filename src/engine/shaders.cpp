#include <algorithm>
#include <fstream>
#include <iostream>
#include <span>
#include <sstream>
#include <string>

#include <glad/glad.h>
#include <spdlog/spdlog.h>

#include "engine/shaders.h"

unsigned int engine::shader::gl_type_conv(const type type)
{
    if (type == type::vertex)
    {
        return GL_VERTEX_SHADER;
    }

    if (type == type::fragment)
    {
        return GL_FRAGMENT_SHADER;
    }

    return 0;
}

void engine::shader::retrive_file(const std::string &path)
{
    std::ifstream file{path};
    if (!file)
    {
        return;
    }

    std::stringstream buffer;
    buffer << file.rdbuf();

    this->_source = std::move(buffer.str());

    file.close();
}

unsigned int engine::shader::get() const
{
    return this->_shader;
}

engine::shader::shader(type type, const std::string &path) : _type(type)
{
    this->_shader = glCreateShader(shader::gl_type_conv(this->_type));

    this->retrive_file(path);

    const char *source = this->_source.c_str();
    glShaderSource(this->_shader, 1, &source, nullptr);
    glCompileShader(this->_shader);

    // check for shader compile errors
    int  success;
    char log[512];
    glGetShaderiv(this->_shader, GL_COMPILE_STATUS, &success);
    if (success == 0)
    {
        glGetShaderInfoLog(this->_shader, sizeof(log), nullptr, log);
        std::cout << "shader compilation_failed: \n" << log << "\n";
    }
}

int engine::program::get_uniform_location(const std::string &name)
{
    const auto cached = this->_uniform_cache.find(name);
    if (cached != this->_uniform_cache.end())
    {
        return cached->second;
    }

    const int location = glGetUniformLocation(this->_id, name.c_str());
    if (location != -1)
    {
        spdlog::info(
            "program::get_uniform_location(\"{}\"): _id:{:0d} -> cache @ {:0x}",
            name, this->_id, location);
        this->_uniform_cache[name] = location;
    }

    return location;
}

unsigned int engine::program::get() const
{
    return this->_id;
}

void engine::program::use() const
{
    glUseProgram(this->_id);
}

engine::program::program(std::span<shader> &&shaders) : _shaders(shaders)
{
    this->_id = glCreateProgram();
    std::ranges::for_each(this->_shaders, [this](shader &shader) {
        glAttachShader(this->_id, shader.get());
    });

    glLinkProgram(this->_id);

    // check for linking errors
    int  success;
    char log[512];
    glGetProgramiv(this->_id, GL_LINK_STATUS, &success);
    if (success == 0)
    {
        glGetProgramInfoLog(this->_id, sizeof(log), nullptr, log);
        std::cout << "shader program linking failed:\n" << log << "\n";
    }

    std::ranges::for_each(this->_shaders, [this](shader &shader) {
        glDeleteShader(shader.get());
    });
}

engine::program::~program()
{
    glDeleteProgram(this->_id);
}
