#pragma once

#include "p6/p6.h"

struct Program {
    p6::Shader _Program;

    GLint uMVPMatrix;
    GLint uMVMatrix;
    GLint uNormalMatrix;

    GLint uTexture;

    GLint uKd;
    GLint uKs;
    GLint uShininess;

    GLint uLightPos_vs;
    GLint uLightDir_vs;
    GLint uLightIntensity;

    // ObstaclesProgram() : m_Program(p6::load_shader("shaders/3D.vs.glsl", "shaders/pointlight.fs.glsl"))
    Program()
        : _Program(p6::load_shader("Shaders/3D.vs.glsl", "Shaders/directionalLight.fs.glsl"))
    {
        initializeVariables();
    }

    Program(std::string shaderPath)
        : _Program(p6::load_shader("Shaders/3D.vs.glsl", shaderPath))
    {
        initializeVariables();
    }

    void initializeVariables()
    {
        uMVPMatrix    = glGetUniformLocation(_Program.id(), "uMVPMatrix");
        uMVMatrix     = glGetUniformLocation(_Program.id(), "uMVMatrix");
        uNormalMatrix = glGetUniformLocation(_Program.id(), "uNormalMatrix");

        uTexture = glGetUniformLocation(_Program.id(), "uTexture");

        uKd        = glGetUniformLocation(_Program.id(), "uKd");
        uKs        = glGetUniformLocation(_Program.id(), "uKs");
        uShininess = glGetUniformLocation(_Program.id(), "uShininess");

        uLightPos_vs    = glGetUniformLocation(_Program.id(), "uLightPos_vs");
        uLightDir_vs    = glGetUniformLocation(_Program.id(), "uLightDir_vs");
        uLightIntensity = glGetUniformLocation(_Program.id(), "uLightIntensity");
    }
};