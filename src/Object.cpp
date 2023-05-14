#include "Object.hpp"
#include "glm/gtc/type_ptr.hpp"

Object::Object(Object& obj)
    : _program(obj._program) {}

Object::Object(Program& program, std::vector<glimac::ShapeVertex> vertices, Texture texture)
    : _program(program), _vertices(vertices), _texture(texture)
{
    createVBO();
    createVAO();
}

Object::Object(Program& program, Model vertices, Texture texture)
    : _program(program), _vertices(vertices.getVertices()), _texture(texture)
{
    createVBO();
    createVAO();
}

Object::Object(Program& program, std::vector<glimac::ShapeVertex> vertices)
    : _program(program), _vertices(vertices)
{
    Texture defTex("Assets/textures/default_tex.png", 0);
    _texture = defTex;
    createVBO();
    createVAO();
}

Object::Object(Program& program, Model vertices)
    : _program(program), _vertices(vertices.getVertices())
{
    createVBO();
    createVAO();
}

void Object::createVBO()
{
    glGenBuffers(1, &_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glBufferData(GL_ARRAY_BUFFER, _vertices.size() * sizeof(glimac::ShapeVertex), _vertices.data(), GL_STATIC_DRAW);
}

void Object::createVAO()
{
    glGenVertexArrays(1, &_vao);
    glBindVertexArray(_vao);

    const GLuint VERTEX_ATTR_POSITION  = 0;
    const GLuint VERTEX_ATTR_NORMAL    = 1;
    const GLuint VERTEX_ATTR_TEXCOORDS = 2;

    glEnableVertexAttribArray(VERTEX_ATTR_POSITION);
    glEnableVertexAttribArray(VERTEX_ATTR_NORMAL);
    glEnableVertexAttribArray(VERTEX_ATTR_TEXCOORDS);

    glBindBuffer(GL_ARRAY_BUFFER, _vbo);

    glVertexAttribPointer(VERTEX_ATTR_POSITION, 3, GL_FLOAT, GL_FALSE, sizeof(glimac::ShapeVertex), (const GLvoid*)(offsetof(glimac::ShapeVertex, position)));
    glVertexAttribPointer(VERTEX_ATTR_NORMAL, 3, GL_FLOAT, GL_FALSE, sizeof(glimac::ShapeVertex), (const GLvoid*)(offsetof(glimac::ShapeVertex, normal)));
    glVertexAttribPointer(VERTEX_ATTR_TEXCOORDS, 2, GL_FLOAT, GL_FALSE, sizeof(glimac::ShapeVertex), (const GLvoid*)(offsetof(glimac::ShapeVertex, texCoords)));
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);
}

void Object::createDrawEnvironment(p6::Context& ctx) // TODO if needed vector of Texture, passing entire texture of just GLuint bc might need to
{
    _program._Program.use(); // TODO change confusing names
    glBindVertexArray(_vao);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, _texture.getTextureID());
    _ProjMatrix = glm::perspective(glm::radians(70.f), ctx.aspect_ratio(), 0.1f, 100.f);
}

void Object::draw(const FreeflyCamera& ViewMatrixCamera, glm::vec3 position, float degRotate, float scaleSize)
{
    glm::mat4 MVMatrix = ViewMatrixCamera.getViewMatrix();
    MVMatrix           = glm::translate(ViewMatrixCamera.getViewMatrix(), position);
    MVMatrix           = glm::rotate(MVMatrix, glm::radians(degRotate), glm::vec3(0.0f, 1.0f, 0.0f));
    MVMatrix           = glm::scale(
        MVMatrix,
        glm::vec3(scaleSize, scaleSize, scaleSize)
    );

    glm::mat4 NormalMatrix = glm::transpose(glm::inverse(MVMatrix));

    glUniformMatrix4fv(_program.uMVMatrix, 1, GL_FALSE, glm::value_ptr(MVMatrix));
    glUniformMatrix4fv(_program.uMVPMatrix, 1, GL_FALSE, glm::value_ptr(_ProjMatrix * MVMatrix));
    glUniformMatrix4fv(_program.uNormalMatrix, 1, GL_FALSE, glm::value_ptr(NormalMatrix));

    glUniform3fv(_program.uKd, 1, glm::value_ptr(glm::vec3(1, 1, 1)));
    glUniform3fv(_program.uKs, 1, glm::value_ptr(glm::vec3(1, 1, 1)));
    glUniform1f(_program.uShininess, 0.5);
    glUniform3fv(_program.uLightPos_vs, 1, glm::value_ptr(MVMatrix * glm::vec4(1, 0, 1, 1)));
    glUniform3fv(_program.uLightDir_vs, 1, glm::value_ptr(glm::vec4(1, 1, 0, 1)));
    glUniform3fv(_program.uLightIntensity, 1, glm::value_ptr(glm::vec3(8, 8, 8))); // change to 1 to see effects on light & tex object

    glDrawArrays(GL_TRIANGLES, 0, _vertices.size());

    // glBindVertexArray(0);
}

void Object::debindVAO()
{
    glBindVertexArray(0);
}

void Object::deleteVBO_VAO()
{
    glDeleteVertexArrays(1, &_vao);
    glDeleteBuffers(1, &_vbo);
}