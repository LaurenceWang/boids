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

    void createVBO();
    void createVAO();

public:
    Object(Program& program, std::vector<glimac::ShapeVertex> vertices);
    Object(Program& program, Model vertices);
    ~Object() = default;

    void draw(const FreeflyCamera& ViewMatrixCamera, GLuint& texture, p6::Context& ctx, glm::vec3 position, float scaleSize, int nb);
    void deleteVBO_VAO();
};