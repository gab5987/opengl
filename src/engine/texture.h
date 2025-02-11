#ifndef ENGINE_TEXTURE_H
#define ENGINE_TEXTURE_H

#include <string>
namespace engine
{
class texture
{
    unsigned int _id;
    std::string  _file_path;

    unsigned char *_buffer;
    int            _width;
    int            _height;
    int            _bpp;

    public:
    explicit texture(const std::string &path);
    ~texture();

    void bind(unsigned int slot = 0) const;
    void unbind() const;
};
}; // namespace engine

#endif

