#pragma once

#include "p6/p6.h"

class Texture {
private:
public:
    GLuint _textureID;
    int    _textureOrder;
    explicit Texture(const std::filesystem::path& texturePath, GLuint textureID);
    void Bind() const;
    void UnBind() const;
    void deleteTex() const;

    GLuint getTextureID()
    {
        return _textureID;
    }

    GLuint getTextureOrder() const
    {
        return _textureOrder;
    }
};