#pragma once

#include "Model.hpp"
#include "Program.hpp"
#include "Texture.hpp"
#include "glimac/FreeflyCamera.hpp"
#include "glimac/common.hpp"
#include "p6/p6.h"

class Object {
private:
    GLuint                           _vbo;
    GLuint                           _vao;
    Program&                         _program;
    std::vector<glimac::ShapeVertex> _vertices;
    glm::mat4                        _ProjMatrix;
    Texture                          _texture;

    void createVBO();
    void createVAO();

public:
    Object(Program& program, std::vector<glimac::ShapeVertex> vertices, Texture texture);
    Object(Program& program, Model vertices, Texture texture);
    Object(Program& program, std::vector<glimac::ShapeVertex> vertices);
    Object(Program& program, Model vertices);
    ~Object() = default;

    void createDrawEnvironment(p6::Context& ctx);
    void draw(const FreeflyCamera& ViewMatrixCamera, glm::vec3 position, float scaleSize);
    void debindVAO();
    void deleteVBO_VAO();
};