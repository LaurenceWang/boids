#pragma once
#include "ProgramEntity.hpp"

struct shadowProgram : public ProgramEntity {
    p6::Shader _Program;

    GLint  uModel;
    GLint  lightSpaceMatrix;
    GLint  DepthBiasMVP;
    GLint  shadowMap;
    GLuint lightInvDirID;

    shadowProgram()
        : _Program(p6::load_shader("Shaders/simpleDepthShader.vs.glsl", "Shaders/empty.fs.glsl"))
    {
        initializeVariables();
    }

    shadowProgram(std::string shaderPath)
        : _Program(p6::load_shader("Shaders/depth.vs.glsl", shaderPath))
    {
        initializeVariables();
    }

    void initializeVariables()
    {
        uModel           = glGetUniformLocation(_Program.id(), "uModel");
        lightSpaceMatrix = glGetUniformLocation(_Program.id(), "lightSpaceMatrix");
        DepthBiasMVP     = glGetUniformLocation(_Program.id(), "DepthBiasMVP");
        shadowMap        = glGetUniformLocation(_Program.id(), "shadowMap");
        lightInvDirID    = glGetUniformLocation(_Program.id(), "LightInvDirection_worldspace");
    }
};