#pragma once
#include "ProgramEntity.hpp"

struct defaultProgram : public ProgramEntity {
    p6::Shader _Program;

    GLint uTexture;

    GLint uKd;
    GLint uKs;
    GLint uShininess;

    GLint uLightPos_vs;
    GLint uLightDir_vs;
    GLint uLightIntensity;

    defaultProgram()
        : _Program(p6::load_shader("Shaders/3D.vs.glsl", "Shaders/directionalLight.fs.glsl"))
    {
        initializeVariables();
    }

    defaultProgram(std::string shaderPath)
        : _Program(p6::load_shader("Shaders/3D.vs.glsl", shaderPath))
    {
        initializeVariables();
    }

    void initializeVariables()
    {
        uTexture = glGetUniformLocation(_Program.id(), "uTexture");

        uKd        = glGetUniformLocation(_Program.id(), "uKd");
        uKs        = glGetUniformLocation(_Program.id(), "uKs");
        uShininess = glGetUniformLocation(_Program.id(), "uShininess");

        uLightPos_vs    = glGetUniformLocation(_Program.id(), "uLightPos_vs");
        uLightDir_vs    = glGetUniformLocation(_Program.id(), "uLightDir_vs");
        uLightIntensity = glGetUniformLocation(_Program.id(), "uLightIntensity");
    }
};