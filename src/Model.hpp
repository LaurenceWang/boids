#pragma once

#include <cstdlib>
#include <string>
#include <vector>
#include "glimac/common.hpp"
#include "p6/p6.h"

class Model {
private:
    std::vector<glimac::ShapeVertex> _vertices;

    bool lineStart(const char* line, const char* letter);
    void pushIndice(std::vector<glm::vec3>& vertex, std::vector<glm::vec3>& normal, std::vector<glm::vec2>& texC, int& vIndex, int& uvIndex, int& nIndex);

public:
    Model();
    explicit Model(std::vector<glimac::ShapeVertex> vertices);
    explicit Model(std::string path);
    ~Model() = default;

    void                             LoadFromFile(const std::string fileName);
    std::vector<glimac::ShapeVertex> getVertices() const;
};