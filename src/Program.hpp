#pragma once

#include "p6/p6.h"

struct Program {
    p6::Shader _shader;

    GLint uMVPMatrix;
    GLint uMVMatrix;
    GLint uNormalMatrix;

    GLint uTexture;

    GLint uKd;
    GLint uKs;
    GLint uShininess;

    GLint uLightPos_vs;
    GLint uLightPosArp_vs;
    GLint uLightDir_vs;
    GLint uLightIntensity;
    GLint fragPosLightSpace;

    Program()
        : _shader(p6::load_shader("Shaders/3D.vs.glsl", "Shaders/texLights.fs.glsl"))
    {
        initializeVariables();
    }

    Program(std::string shaderPath)
        : _shader(p6::load_shader("Shaders/3D.vs.glsl", shaderPath))
    {
        initializeVariables();
    }

    Program(std::string shaderVSPath, std::string shaderFSPath)
        : _shader(p6::load_shader(shaderVSPath, shaderFSPath))
    {
        initializeVariables();
    }

    void initializeVariables()
    {
        uMVPMatrix    = glGetUniformLocation(_shader.id(), "uMVPMatrix");
        uMVMatrix     = glGetUniformLocation(_shader.id(), "uMVMatrix");
        uNormalMatrix = glGetUniformLocation(_shader.id(), "uNormalMatrix");

        uTexture = glGetUniformLocation(_shader.id(), "uTexture");

        uKd        = glGetUniformLocation(_shader.id(), "uKd");
        uKs        = glGetUniformLocation(_shader.id(), "uKs");
        uShininess = glGetUniformLocation(_shader.id(), "uShininess");

        uLightPos_vs    = glGetUniformLocation(_shader.id(), "uLightPos_vs");
        uLightPosArp_vs = glGetUniformLocation(_shader.id(), "uLightPosArp_vs");
        uLightDir_vs    = glGetUniformLocation(_shader.id(), "uLightDir_vs");
        uLightIntensity = glGetUniformLocation(_shader.id(), "uLightIntensity");

        fragPosLightSpace = glGetUniformLocation(_shader.id(), "fragPosLightSpace");
    }
};