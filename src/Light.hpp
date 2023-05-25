#pragma once
#include "Object.hpp"
#include "Program.hpp"
#include "glm/gtc/type_ptr.hpp"

struct lightProgram : public Program {
    p6::Shader _Program;

    // GLint uMVPMatrix;
    // GLint uMVMatrix;
    GLint uModel;

    GLint  lightSpaceMatrix;
    GLint  DepthBiasMVP;
    GLint  shadowMap;
    GLuint lightInvDirID;

    lightProgram()
        : _Program(p6::load_shader("Shaders/simpleDepthShader.vs.glsl", "Shaders/empty.fs.glsl"))
    {
        initializeVariables();
    }

    lightProgram(std::string shaderPath)
        : _Program(p6::load_shader("Shaders/depth.vs.glsl", shaderPath))
    {
        initializeVariables();
    }

    void initializeVariables()
    {
        // uMVPMatrix       = glGetUniformLocation(_Program.id(), "uMVPMatrix");
        // uMVMatrix        = glGetUniformLocation(_Program.id(), "uMVMatrix");
        uModel           = glGetUniformLocation(_Program.id(), "uModel");
        lightSpaceMatrix = glGetUniformLocation(_Program.id(), "lightSpaceMatrix");
        DepthBiasMVP     = glGetUniformLocation(_Program.id(), "DepthBiasMVP");
        shadowMap        = glGetUniformLocation(_Program.id(), "shadowMap");
        lightInvDirID    = glGetUniformLocation(_Program.id(), "LightInvDirection_worldspace");
    }
};

class Light : public Object {
private:
    lightProgram& _program;
    glm::mat4     _depthMVP;

public:
    Light(lightProgram& program, Model vertices)
        : Object(program, vertices), _program(program){};

    Light(lightProgram& program, Model vertices, Texture tex)
        : Object(program, vertices, tex), _program(program){};

    Light(lightProgram& program, std::vector<glimac::ShapeVertex> vertices)
        : Object(program, vertices), _program(program){};

    Light(lightProgram& program, std::vector<glimac::ShapeVertex> vertices, Texture tex)
        : Object(program, vertices, tex), _program(program){};

    void createDrawEnvironment(p6::Context& ctx)
    {
        _program._Program.use();
        glBindVertexArray(_vao);
        _ProjMatrix = glm::perspective(glm::radians(70.f), ctx.aspect_ratio(), 0.1f, 100.f);
    };

    void draw(glm::vec3 lightDir, glm::vec3 position, float degRotate, float scaleSize)
    {
        glm::mat4 depthProjectionMatrix = glm::ortho<float>(-10, 10, -10, 10, -10, 20);
        glm::mat4 depthViewMatrix       = glm::lookAt(lightDir, glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));

        glm::mat4 depthModelMatrix = glm::translate(glm::mat4(1), position);
        depthModelMatrix           = glm::rotate(depthModelMatrix, glm::radians(degRotate), glm::vec3(0.0f, 1.0f, 0.0f));
        depthModelMatrix           = glm::scale(
            depthModelMatrix,
            glm::vec3(scaleSize, scaleSize, scaleSize)
        );

        _depthMVP = depthProjectionMatrix * depthViewMatrix * depthModelMatrix;

        glUniformMatrix4fv(_program.lightSpaceMatrix, 1, GL_FALSE, glm::value_ptr(_depthMVP));

        glDrawArrays(GL_TRIANGLES, 0, _vertices.size());
    };

    /*void draw2(glm::mat4 MatrixID, glm::mat4 ModelMatrixID, glm::mat4 viewMatrixID, glm::mat4 depthBias, glm::vec3 lightInvDir, GLint texture)
    {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, _texture.getTextureID());
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture);

        glUniformMatrix4fv(_program.uMVPMatrix, 1, GL_FALSE, glm::value_ptr(MatrixID));
        glUniformMatrix4fv(_program.uModel, 1, GL_FALSE, glm::value_ptr(ModelMatrixID));
        glUniformMatrix4fv(_program.uMVMatrix, 1, GL_FALSE, glm::value_ptr(viewMatrixID));

        glUniformMatrix4fv(_program.DepthBiasMVP, 1, GL_FALSE, glm::value_ptr(depthBias));

        glUniform3f(_program.lightInvDirID, lightInvDir.x, lightInvDir.y, lightInvDir.z);

        glDrawArrays(GL_TRIANGLES, 0, _vertices.size());
    };*/

    void draw2(glm::mat4 ViewMatrixCamera, glm::vec3 position, float degRotate, float scaleSize, glm::vec3 lightDir, GLint texture)
    {
        glm::mat4 ModelMatrix = glm::translate(glm::mat4(1), position);
        ModelMatrix           = glm::rotate(ModelMatrix, glm::radians(degRotate), glm::vec3(0.0f, 1.0f, 0.0f));
        ModelMatrix           = glm::scale(
            ModelMatrix,
            glm::vec3(scaleSize, scaleSize, scaleSize)
        );

        glm::mat4 MVP = _ProjMatrix * ViewMatrixCamera * ModelMatrix;

        glm::mat4 biasMatrix(
            0.5, 0.0, 0.0, 0.0,
            0.0, 0.5, 0.0, 0.0,
            0.0, 0.0, 0.5, 0.0,
            0.5, 0.5, 0.5, 1.0
        );

        glm::mat4 depthBiasMVP = biasMatrix * _depthMVP;

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, _texture.getTextureID());
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture);

        glUniformMatrix4fv(_program.uMVPMatrix, 1, GL_FALSE, glm::value_ptr(MVP));
        glUniformMatrix4fv(_program.uModel, 1, GL_FALSE, glm::value_ptr(ModelMatrix));
        glUniformMatrix4fv(_program.uMVMatrix, 1, GL_FALSE, glm::value_ptr(ViewMatrixCamera));

        glUniformMatrix4fv(_program.DepthBiasMVP, 1, GL_FALSE, glm::value_ptr(depthBiasMVP));

        glUniform3f(_program.lightInvDirID, lightDir.x, lightDir.y, lightDir.z);

        glDrawArrays(GL_TRIANGLES, 0, _vertices.size());
    };
};