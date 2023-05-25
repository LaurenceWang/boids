#pragma once
#include "Texture.hpp"

struct Skybox {
    p6::Shader         _shader;
    std::vector<float> _vertices;
    GLuint             _textureID;
    GLuint             _vbo;
    GLuint             _vao;

    Skybox(const std::vector<float>& vertices)
        : _shader(p6::load_shader("Shaders/skybox.vs.glsl", "Shaders/skybox.fs.glsl"))
        , _vertices(vertices)
    {
        createVBO_VAO(_vertices.data(), _vertices.size());
        getTextureID();
    }

    void createVBO_VAO(const float* skyboxVertices, size_t numVertices);
    void getTextureID();
    void renderSkybox(glm::mat4 ViewMatrix, p6::Context& ctx);
    void deleteVBO_VAO();
};

std::vector<float> returnVertices();
