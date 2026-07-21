#define STB_IMAGE_IMPLEMENTATION
#include "Texture.h"
#include <glad/glad.h>
#include <stb/stb_image.h>
#include <iostream>
Texture::Texture(const std::string& path)
    : ID(0), m_file_path(path), m_width(0), m_height(0), m_bpp(0)
{
    stbi_set_flip_vertically_on_load(0);
    unsigned char* data = stbi_load(path.c_str(), &m_width, &m_height, &m_bpp, 0);
    if (!data)
    {
        std::cerr << "Texture: failed to load image: " << path << '\n';
        return;
    }

    GLenum internalFormat = GL_RGB;
    GLenum dataFormat = GL_RGB;
    if (m_bpp == 1) { internalFormat = dataFormat = GL_RED; }
    else if (m_bpp == 3) { internalFormat = GL_RGB; dataFormat = GL_RGB; }
    else if (m_bpp == 4) { internalFormat = GL_RGBA; dataFormat = GL_RGBA; }

    glGenTextures(1, &ID);
    glBindTexture(GL_TEXTURE_2D, ID);

    glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, m_width, m_height, 0, dataFormat, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    stbi_image_free(data);
    glBindTexture(GL_TEXTURE_2D, 0);

    std::cerr << "Loaded texture '" << path << "' ("
        << m_width << "x" << m_height << ", " << m_bpp << " channels) -> ID=" << ID << '\n';

}
Texture::~Texture()
{
    if (ID != 0) {
        glDeleteTextures(1, &ID);
    }
}
void Texture::bind(unsigned int slot) const 
{
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, ID);
}
void Texture::unbind() 
{
    glBindTexture(GL_TEXTURE_2D, 0);
}
int Texture::getWidth() const { return m_width; }
int Texture::getHeight() const { return m_height; }

bool Texture::isValid() const { return ID != 0; }
