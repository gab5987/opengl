#include <glad/glad.h>
#include <stb_image.h>

#include "render/texture.h"

engine::texture::texture(const std::string &path) : _file_path(path)
{
    stbi_set_flip_vertically_on_load(1);
    this->_buffer =
        stbi_load(path.c_str(), &this->_width, &this->_height, &this->_bpp, 4);

    glGenTextures(1, &this->_id);
    glBindTexture(GL_TEXTURE_2D, this->_id); // Bind without slot selection

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glTexImage2D(
        GL_TEXTURE_2D, 0, GL_RGBA8, this->_width, this->_height, 0, GL_RGBA,
        GL_UNSIGNED_BYTE, this->_buffer);

    this->unbind();
};

engine::texture::~texture()
{
    if (this->_buffer != nullptr)
    {
        stbi_image_free(this->_buffer);
    }

    glDeleteTextures(1, &this->_id);
}

void engine::texture::bind(unsigned int slot) const
{
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, this->_id);
}

void engine::texture::unbind() const
{
    glBindTexture(GL_TEXTURE_2D, 0);
}
