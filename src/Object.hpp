#pragma once

#include "Model.hpp"
#include "Program.hpp"
#include "Texture.hpp"
#include "glimac/FreeflyCamera.hpp"
#include "glimac/common.hpp"
#include "p6/p6.h"

struct objectParameters {
    glm::vec3 position;
    float     degRotate;
    float     scaleSize;
};

class Object {
private:
protected:
    Program&                         _program;
    GLuint                           _vbo;
    GLuint                           _vao;
    std::vector<glimac::ShapeVertex> _vertices;
    glm::mat4                        _ProjMatrix;
    Texture                          _texture;

    void createVBO();
    void createVAO();

public:
    Object(Object&& obj);
    Object(Program& program, Model vertices);
    Object(Program& program, Model vertices, Texture texture);
    Object(Program& program, std::vector<glimac::ShapeVertex> vertices);
    Object(Program& program, std::vector<glimac::ShapeVertex> vertices, Texture texture);

    ~Object() = default;

    void createDrawEnvironment(p6::Context& ctx);
    void draw(const glm::mat4& ViewMatrixCamera, glm::vec3 position, float degRotate, float scaleSize, glm::vec3 arpPos);
    void draw(const glm::mat4& ViewMatrixCamera, objectParameters parameters);
    void debindVAO();
    void deleteVBO_VAO();
};