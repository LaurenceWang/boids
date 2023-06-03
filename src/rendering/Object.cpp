#include "Object.hpp"
#include "glm/gtc/type_ptr.hpp"

Object::Object(Object& obj)
    : _program(obj._program) {}

Object::Object(Program& program, Model vertices)
    : _program(program), _vertices(vertices.getVertices())
{
    _lodVertices.push_back(vertices.getVertices());
    _lodVertices.push_back(vertices.getVertices());
    createVBO();
    createVAO();
}

Object::Object(Program& program, Model vertices, Texture texture)
    : Object(program, vertices)
{
    _texture = texture;
}

Object::Object(Program& program, std::vector<glimac::ShapeVertex> vertices)
    : _program(program), _vertices(vertices), _texture(Texture("Assets/textures/default_tex.png", 0))
{
    _lodVertices.push_back(vertices);
    _lodVertices.push_back(vertices);
    createVBO();
    createVAO();
}

Object::Object(Program& program, std::vector<glimac::ShapeVertex> vertices, Texture texture)
    : Object(program, vertices)
{
    _lodVertices.push_back(vertices);
    _lodVertices.push_back(vertices);
    _texture = texture;
}

Object::Object(Program& program, Model vertices, Model lowVertices, Texture texture)
    : _program(program), _texture(texture)
{
    _lodVertices.push_back(vertices.getVertices());
    _lodVertices.push_back(lowVertices.getVertices());
    createVBO();
    createVAO();
}

void Object::createVBO()
{
    glGenBuffers(1, &_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glBufferData(GL_ARRAY_BUFFER, (_lodVertices[_lod]).size() * sizeof(glimac::ShapeVertex), (_lodVertices[_lod]).data(), GL_STATIC_DRAW);
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

void Object::createDrawEnvironment(p6::Context& ctx)
{
    //_program._shader.use();
    glBindVertexArray(_vao);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, _texture.getTextureID());
    _ProjMatrix = glm::perspective(glm::radians(70.f), ctx.aspect_ratio(), 0.1f, 100.f);
}

void Object::draw(const glm::mat4& ViewMatrixCamera, objectParameters parameters)
{
    glm::mat4 MVMatrix = ViewMatrixCamera;
    MVMatrix           = glm::translate(MVMatrix, parameters.position);
    MVMatrix           = glm::rotate(MVMatrix, glm::radians(parameters.degRotate), glm::vec3(0.0f, 1.0f, 0.0f));
    MVMatrix           = glm::scale(
        MVMatrix,
        glm::vec3(parameters.scaleSize, parameters.scaleSize, parameters.scaleSize)
    );

    glm::mat4 NormalMatrix = glm::transpose(glm::inverse(MVMatrix));
    glUniformMatrix4fv(_program.uMVMatrix, 1, GL_FALSE, glm::value_ptr(MVMatrix));
    glUniformMatrix4fv(_program.uMVPMatrix, 1, GL_FALSE, glm::value_ptr(_ProjMatrix * MVMatrix));
    glUniformMatrix4fv(_program.uNormalMatrix, 1, GL_FALSE, glm::value_ptr(NormalMatrix));

    glDrawArrays(GL_TRIANGLES, 0, (_lodVertices[_lod]).size());
}

void Object::finalDraw(p6::Context& ctx, const glm::mat4& ViewMatrixCamera, objectParameters parameters, glm::vec3 arpPos, int lodChoice)
{
    createDrawEnvironment(ctx);
    adjustLOD(arpPos, parameters.position, lodChoice);
    draw(ViewMatrixCamera, parameters);
    debindVAO();
}

void Object::distanceLOD(glm::vec3 arpenteurPos, glm::vec3 objPos)
{
    double distance = glm::distance(arpenteurPos, objPos);
    _lod            = (distance > 7) ? 1 : 0;
}

void Object::adjustLOD(glm::vec3 arpenteurPos, glm::vec3 objPos, int choice)
{
    if (choice < 2)
        _lod = choice;
    else
        distanceLOD(arpenteurPos, objPos);
}

void Object::debindVAO()
{
    glBindVertexArray(0);
}

void Object::deleteRessources()
{
    glDeleteVertexArrays(1, &_vao);
    glDeleteBuffers(1, &_vbo);
    _texture.deleteTex();
}