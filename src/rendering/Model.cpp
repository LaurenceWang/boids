#include "Model.hpp"

Model::Model()
{
    std::vector<glimac::ShapeVertex> newVertices;
    _vertices = newVertices;
}

Model::Model(std::vector<glimac::ShapeVertex> vertices)
    : _vertices(vertices) {}

Model::Model(std::string path)
{
    std::vector<glimac::ShapeVertex> newVertices;
    _vertices = newVertices;
    LoadFromFile(path);
}

void Model::LoadFromFile(const std::string fileName)
{
    FILE* file = fopen(fileName.c_str(), "r");
    if (file == NULL)
    {
        std::cerr << "couldn't open file \n";
    }

    std::vector<glm::vec3> temp_vertices, temp_normals;
    std::vector<glm::vec2> temp_uvs;

    char lineHeader[128];
    while (1)
    {
        int res = fscanf(file, "%s", lineHeader);
        if (res == EOF)
        {
            break;
        }

        if (lineStart(lineHeader, "v"))
        {
            glm::vec3 vertex;
            fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
            temp_vertices.push_back(vertex);
        }
        else if (lineStart(lineHeader, "vt"))
        {
            glm::vec2 uv;
            fscanf(file, "%f %f\n", &uv.x, &uv.y);
            temp_uvs.push_back(uv);
        }
        else if (lineStart(lineHeader, "vn"))
        {
            glm::vec3 normal;
            fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
            temp_normals.push_back(normal);
        }
        else if (lineStart(lineHeader, "f"))
        {
            // std::string  vertex1, vertex2, vertex3;
            int v0, v1, v2, uv0, uv1, uv2, n0, n1, n2;

            fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &v0, &uv0, &n0, &v1, &uv1, &n1, &v2, &uv2, &n2);

            pushIndice(temp_vertices, temp_normals, temp_uvs, v0, uv0, n0);
            pushIndice(temp_vertices, temp_normals, temp_uvs, v1, uv2, n1);
            pushIndice(temp_vertices, temp_normals, temp_uvs, v2, uv2, n2);
        }
    }

    fclose(file);
}

bool Model::lineStart(const char* line, const char* letters)
{
    return strcmp(line, letters) == 0;
}

void Model::pushIndice(std::vector<glm::vec3>& vertex, std::vector<glm::vec3>& normal, std::vector<glm::vec2>& texC, int& vIndex, int& uvIndex, int& nIndex)
{
    glm::vec3 vrtx      = vertex[vIndex - 1];
    glm::vec3 nrml      = normal[nIndex - 1];
    glm::vec2 texCoords = texC[uvIndex - 1];

    _vertices.push_back({vrtx, nrml, texCoords});
}

std::vector<glimac::ShapeVertex> Model::getVertices() const
{
    return _vertices;
}