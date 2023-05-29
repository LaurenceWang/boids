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
    Program&                                      _program;
    int                                           _lod = 0;
    std::vector<std::vector<glimac::ShapeVertex>> _lodVertices;
    GLuint                                        _vbo;
    GLuint                                        _vao;
    std::vector<glimac::ShapeVertex>              _vertices;
    glm::mat4                                     _ProjMatrix;
    Texture                                       _texture;
    void                                          createVBO();
    void                                          createVAO();
    void                                          createVBO2();
    void                                          createVAO2();

public:
    Object(Object& obj);
    Object(Program& program, Model vertices, Model lowVertices, Texture texture);
    Object(Program& program, Model vertices);
    Object(Program& program, Model vertices, Texture texture);
    Object(Program& program, std::vector<glimac::ShapeVertex> vertices);
    Object(Program& program, std::vector<glimac::ShapeVertex> vertices, Texture texture);

    ~Object() = default;

    void createDrawEnvironment(p6::Context& ctx);
    void draw(const glm::mat4& ViewMatrixCamera, glm::vec3 position, float degRotate, float scaleSize, glm::vec3 arpPos);
    void draw(const glm::mat4& ViewMatrixCamera, objectParameters parameters);
    void draw3(const glm::mat4& ViewMatrixCamera, glm::vec3 position, float degRotate, float scaleSize, glm::vec3 arpPos);
    void adjustLOD(glm::vec3 arpenteurPos, glm::vec3 objPos, int choice);
    void distanceLOD(glm::vec3 arpenteurPos, glm::vec3 objPos);
    void debindVAO();
    void deleteVBO_VAO();
};