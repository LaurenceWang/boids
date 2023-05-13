#pragma once

#include "Model.hpp"
#include "Program.hpp"
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

    void createVBO();
    void createVAO();

public:
    Object(Program& program, std::vector<glimac::ShapeVertex> vertices);
    Object(Program& program, Model vertices);
    ~Object() = default;

    void createDrawEnvironment(GLuint texture, p6::Context& ctx);
    void draw(const FreeflyCamera& ViewMatrixCamera, p6::Context& ctx, glm::vec3 position, float scaleSize);
    void debindVAO();
    void deleteVBO_VAO();
};