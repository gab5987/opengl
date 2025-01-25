#ifndef RENDER_SHADERS_H
#define RENDER_SHADERS_H

#include <string>
#include <vector>

class shader
{
    public:
    enum class type : uint8_t
    {
        unknown,
        vertex,
        fragment,
    };

    private:
    type        _type{type::unknown};
    std::string _source;

    unsigned int _shader;

    void retrive_file(const std::string &path);

    static unsigned int gl_type_conv(type type);
    static type         check_type(const std::string &path);

    public:
    unsigned int get() const;

    explicit shader(const std::string &path);
};

class program
{
    std::vector<shader> _shaders;

    unsigned int _program;

    public:
    unsigned int get() const;

    explicit program(std::vector<shader> &&shaders);
    ~program();
};

#endif

