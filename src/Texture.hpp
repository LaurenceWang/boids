#pragma once

#include "p6/p6.h"

class Texture {
private:
    GLuint _textureID;
    int    _textureOrder;

public:
    explicit Texture(const std::filesystem::path& texturePath, GLuint textureID);

    Texture(std::vector<std::filesystem::path>& faces, GLuint textureID);

    Texture() = default;
    void bind() const;
    void unBind() const;
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