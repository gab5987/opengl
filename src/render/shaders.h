#ifndef RENDER_SHADERS_H
#define RENDER_SHADERS_H

#include <span>
#include <string>

namespace render
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
        void add_uniform(
            const std::string &name, const void *data, unsigned int size);
        unsigned int uniform_index() const;

        unsigned int get() const;

        explicit shader(type type, const std::string &path);
    };

    class program
    {
        std::span<shader> _shaders;

        unsigned int _program;

        public:
        unsigned int get() const;
        void         use() const;

        explicit program(std::span<shader> &&shaders);
        ~program();
    };
}; // namespace render

#endif

