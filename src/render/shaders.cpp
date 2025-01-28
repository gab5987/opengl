#include <algorithm>
#include <fstream>
#include <iostream>
#include <span>
#include <sstream>
#include <string>
#include <vector>

#include "glad/glad.h"
#include "render/shaders.h"

unsigned int render::shader::gl_type_conv(const type type)
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

void render::shader::retrive_file(const std::string &path)
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

unsigned int render::shader::get() const
{
    return this->_shader;
}

render::shader::shader(type type, const std::string &path) : _type(type)
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
};

unsigned int render::program::get() const
{
    return this->_program;
}

void render::program::use() const
{
    glUseProgram(this->_program);
}

render::program::program(std::span<shader> &&shaders) : _shaders(shaders)
{
    this->_program = glCreateProgram();
    std::ranges::for_each(this->_shaders, [this](shader &shader) {
        glAttachShader(this->_program, shader.get());
    });

    glLinkProgram(this->_program);

    // check for linking errors
    int  success;
    char log[512];
    glGetProgramiv(this->_program, GL_LINK_STATUS, &success);
    if (success == 0)
    {
        glGetProgramInfoLog(this->_program, sizeof(log), nullptr, log);
        std::cout << "shader program linking failed:\n" << log << "\n";
    }

    std::ranges::for_each(this->_shaders, [this](shader &shader) {
        glDeleteShader(shader.get());
    });
};

render::program::~program()
{
    glDeleteProgram(this->_program);
}
