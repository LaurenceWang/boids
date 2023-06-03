#pragma once
#include "rendering/Texture.hpp"

struct Skybox {
    p6::Shader         _shader;
    std::vector<float> _vertices;
    GLuint             _textureID;
    GLuint             _vbo;
    GLuint             _vao;

    Skybox(const std::vector<float>& vertices);

    void createVBO_VAO(const float* skyboxVertices, size_t numVertices);
    void getTextureID();
    void renderSkybox(glm::mat4 ViewMatrix, p6::Context& ctx);
    void deleteRessources();
};

std::vector<float> returnVertices();
