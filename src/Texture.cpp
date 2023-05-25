#include "Texture.hpp"

Texture::Texture(const std::filesystem::path& texturePath, GLuint textureID)
    : _textureID(textureID)
{
    const auto img = p6::load_image_buffer(texturePath);

    glGenTextures(1, &_textureID);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, _textureID);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img.width(), img.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, img.data());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, 0);
}

Texture::Texture(std::vector<std::filesystem::path>& faces, GLuint textureID)
    : _textureID(textureID)
{
    glGenTextures(1, &_textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, _textureID);

    for (unsigned int i = 0; i < faces.size(); i++)
    {
        const auto img = p6::load_image_buffer(faces[i]);

        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, img.width(), img.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, img.data());
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}

void Texture::bind() const
{
    glActiveTexture(GL_TEXTURE0 + _textureOrder);
    glBindTexture(GL_TEXTURE_2D, _textureID);
}

void Texture::unBind() const
{
    glActiveTexture(GL_TEXTURE0 + _textureOrder);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::deleteTex() const
{
    glDeleteTextures(1, &_textureID);
}
