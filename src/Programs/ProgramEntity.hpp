#pragma once
#include "p6/p6.h"

struct ProgramEntity {
    p6::Shader _Program;

    GLint uMVPMatrix;
    GLint uMVMatrix;
    GLint uNormalMatrix;

    ProgramEntity()
        : _Program(p6::load_shader("Shaders/3D.vs.glsl", "Shaders/directionalLight.fs.glsl"))
    {
        initializeVariables();
    }

    ProgramEntity(std::string shaderVSPath, std::string shaderFSPath)
        : _Program(p6::load_shader(shaderVSPath, shaderFSPath))
    {
        initializeVariables();
    }

    void initializeVariables()
    {
        uMVPMatrix    = glGetUniformLocation(_Program.id(), "uMVPMatrix");
        uMVMatrix     = glGetUniformLocation(_Program.id(), "uMVMatrix");
        uNormalMatrix = glGetUniformLocation(_Program.id(), "uNormalMatrix");
    }
};