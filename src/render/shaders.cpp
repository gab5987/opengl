#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "glad/glad.h"
#include "render/shaders.h"

unsigned int render::shader::gl_type_conv(const type type)
{
    switch (type)
    {
        case type::vertex: {
            return GL_VERTEX_SHADER;
        }

        case type::fragment: {
            return GL_FRAGMENT_SHADER;
        }

        default: {
            return 0;
        }
    }
}

render::shader::type render::shader::check_type(const std::string &path)
{
    if (path.ends_with("vsh"))
    {
        return type::vertex;
    }

    if (path.ends_with("fsh"))
    {
        return type::fragment;
    }

    return type::unknown;
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

render::shader::shader(const std::string &path)
{
    this->_type = render::shader::check_type(path);
    if (this->_type == type::unknown)
    {
        std::cout << "invalid shader type\n";
        return;
    }

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

render::program::program(std::vector<shader> &&shaders)
    : _shaders(std::move(shaders))
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
