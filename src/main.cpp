#include <stdlib.h>
#include <cstddef>
#include "doctest/parts/doctest_fwd.h"
#include "p6/p6.h"
#define DOCTEST_CONFIG_IMPLEMENT
#include <random>
#include "Boids.hpp"
#include "Food.hpp"
#include "ObstacleCollection.hpp"
#include "Params.hpp"
#include "doctest/doctest.h"
#include "glimac/FreeflyCamera.hpp"
#include "glimac/cone_vertices.hpp"
#include "glimac/sphere_vertices.hpp"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/geometric.hpp"
#include "glm/gtc/random.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "imgui.hpp"
using ObstacleHandler = std::function<void(Obstacle const&)>;

int main(int argc, char* argv[])
{
    { // Run the tests
        if (doctest::Context{}.run() != 0)
            return EXIT_FAILURE;
        // The CI does not have a GPU so it cannot run the rest of the code.
        const bool no_gpu_available = argc >= 2 && strcmp(argv[1], "-nogpu") == 0; // NOLINT(cppcoreguidelines-pro-bounds-pointer-arithmetic)
        if (no_gpu_available)
            return EXIT_SUCCESS;
    }

    // Actual app
    auto ctx = p6::Context{{.title = "Swimming with boids"}};

    const std::vector<glimac::ShapeVertex> vertices  = glimac::sphere_vertices(0.2f, 32, 16);
    const std::vector<glimac::ShapeVertex> vertices2 = glimac::cone_vertices(0.2f, 0.2f, 32, 16);

    const p6::Shader shader = p6::load_shader(
        "Shaders/3D.vs.glsl",
        "Shaders/multiTex3D.fs.glsl"
    );

    GLuint shaderID = shader.id();

    GLint uniformMVP    = glGetUniformLocation(shaderID, "uMVPMatrix");
    GLint uniformMV     = glGetUniformLocation(shaderID, "uMVMatrix");
    GLint uniformNormal = glGetUniformLocation(shaderID, "uNormalMatrix");
    // GLint uFishTexture  = glGetUniformLocation(shaderID, "uFishTexture");
    // GLint uMoonTexture  = glGetUniformLocation(shaderID, "uMoonTexture");

    // glUniform1i(uFishTexture, 0);
    //  glUniform1i(uMoonTexture, 1);

    /********************TEXTURE LOADING***********************/

    ///////////////fish
    GLuint     fishTextureID = 0;
    const auto textureFish   = p6::load_image_buffer("Assets/textures/fish.jpg");

    glGenTextures(1, &fishTextureID);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, fishTextureID);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, textureFish.width(), textureFish.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, textureFish.data());

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Unbind the texture
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, 0);

    glEnable(GL_DEPTH_TEST);

    ///////////////////moon
    GLuint moonTextureID = 0;

    const auto textureMoon = p6::load_image_buffer("Assets/textures/MoonMap.jpg");

    glGenTextures(1, &moonTextureID);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, moonTextureID);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, textureMoon.width(), textureMoon.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, textureMoon.data());

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Unbind the texture
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, 0);

    //////////////////earth

    GLuint earthTextureID = 0;

    const auto textureEarth = p6::load_image_buffer("Assets/textures/EarthMap.jpg");

    glGenTextures(1, &earthTextureID);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, earthTextureID);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, textureEarth.width(), textureEarth.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, textureEarth.data());

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Unbind the texture
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, 0);

    /*****************************MODEL LOADING****************************/
    std::vector<glimac::ShapeVertex> fishVertices;

    std::vector<unsigned int> vertexIndices, uvIndices, normalIndices;
    std::vector<glm::vec3>    temp_vertices;
    std::vector<glm::vec2>    temp_uvs;
    std::vector<glm::vec3>    temp_normals;

    FILE* file = fopen("Assets/models/fish/model_371254902470.obj", "r");
    if (file == NULL)
    {
        printf("Impossible to open the file !\n");
        return false;
    }

    while (1)
    {
        char lineHeader[128];
        // read the first word of the line
        int res = fscanf(file, "%s", lineHeader);
        if (res == EOF)
        {
            break;
        }

        if (strcmp(lineHeader, "v") == 0)
        {
            glm::vec3 vertex;
            fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
            temp_vertices.push_back(vertex);
        }
        else if (strcmp(lineHeader, "vt") == 0)
        {
            glm::vec2 uv;
            fscanf(file, "%f %f\n", &uv.x, &uv.y);
            temp_uvs.push_back(uv);
        }
        else if (strcmp(lineHeader, "vn") == 0)
        {
            glm::vec3 normal;
            fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
            temp_normals.push_back(normal);
        }
        else if (strcmp(lineHeader, "f") == 0)
        {
            std::string  vertex1, vertex2, vertex3;
            unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
            int          matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2]);
            if (matches != 9)
            {
                printf("File can't be read by our simple parser : ( Try exporting with other options\n");
                return false;
            }
            vertexIndices.push_back(vertexIndex[0]);
            vertexIndices.push_back(vertexIndex[1]);
            vertexIndices.push_back(vertexIndex[2]);
            uvIndices.push_back(uvIndex[0]);
            uvIndices.push_back(uvIndex[1]);
            uvIndices.push_back(uvIndex[2]);
            normalIndices.push_back(normalIndex[0]);
            normalIndices.push_back(normalIndex[1]);
            normalIndices.push_back(normalIndex[2]);
        }
    };

    for (unsigned int i = 0; i < vertexIndices.size(); i++)
    {
        unsigned int vertexIndex = vertexIndices[i];
        glm::vec3    vertex      = temp_vertices[vertexIndex - 1];

        unsigned int normalsIndex = normalIndices[i];
        glm::vec3    normal       = temp_normals[normalsIndex - 1];

        unsigned int uvIndex   = uvIndices[i];
        glm::vec2    texCoords = temp_uvs[uvIndex - 1];

        fishVertices.push_back({vertex, normal, texCoords});
    }

    /***********************************VBO & VAOS*************************************/

    GLuint vbos[2];
    glGenBuffers(2, vbos);

    glBindBuffer(GL_ARRAY_BUFFER, vbos[0]);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glimac::ShapeVertex), vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindBuffer(GL_ARRAY_BUFFER, vbos[1]);
    // glBufferData(GL_ARRAY_BUFFER, vertices2.size() * sizeof(glimac::ShapeVertex), vertices2.data(), GL_STATIC_DRAW);
    glBufferData(GL_ARRAY_BUFFER, fishVertices.size() * sizeof(glimac::ShapeVertex), fishVertices.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    GLuint vaos[2];
    glGenVertexArrays(2, vaos);

    glBindVertexArray(vaos[0]);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, vbos[0]);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glimac::ShapeVertex), (const GLvoid*)(offsetof(glimac::ShapeVertex, position)));
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(glimac::ShapeVertex), (const GLvoid*)(offsetof(glimac::ShapeVertex, normal)));
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(glimac::ShapeVertex), (const GLvoid*)(offsetof(glimac::ShapeVertex, texCoords)));

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);

    glBindVertexArray(vaos[1]);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, vbos[1]);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glimac::ShapeVertex), (const GLvoid*)(offsetof(glimac::ShapeVertex, position)));
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(glimac::ShapeVertex), (const GLvoid*)(offsetof(glimac::ShapeVertex, normal)));
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(glimac::ShapeVertex), (const GLvoid*)(offsetof(glimac::ShapeVertex, texCoords)));

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);

    /********************BOIDS & CO INIT********************/

    // ctx.maximize_window();
    int    fishNb = 100;
    Params p{.separation = 0.043f, .alignment = 0.048f, .steer = 2.5f, .neighRadius = 0.380f, .fishSize = 0.02f};
    // bool   nbChanged   = false;
    // bool   sizeChanged = false;

    Boids boids;
    boids.generateFish(fishNb, 0);

    int   fishNb2 = 20;
    Boids boids2;
    boids.generateFish(fishNb2, 1);

    imGuiInit(&ctx, p, fishNb, boids);

    imGuiInit(&ctx, p, fishNb, boids);

    ObstacleCollection obstacle;

    obstacle.generateObstacles(3);
    obstacle.generateBorders(ctx);

    ObstacleCollection obstacle2;

    obstacle2.generateObstacles(3);

    Food              seaweed(glm::vec3(3, 1, 0), 0.5);
    Food              seaweed2(glm::vec3(-3, 1, 0), 0.5);
    std::vector<Food> meals;
    meals.push_back(seaweed);
    meals.push_back(seaweed2);

    FreeflyCamera ViewMatrixCamera = FreeflyCamera();

    bool Z = false;
    bool Q = false;
    bool S = false;
    bool D = false;
    // Declare your infinite update loop.

    ctx.update = [&]() {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        /*CAMERA MOUSE***/

        ctx.background({0.33, 0.8, 0.98});

        if (Z)
        {
            ViewMatrixCamera.moveFront(0.1);
        }
        if (Q)
        {
            ViewMatrixCamera.moveLeft(0.1);
            // ViewMatrixCamera.rotateLeft(0.1);
        }
        if (S)
        {
            ViewMatrixCamera.moveFront(-0.1);
        }
        if (D)
        {
            // ViewMatrixCamera.rotateLeft(-0.1);
            ViewMatrixCamera.moveLeft(-0.1);
        }

        ctx.key_pressed = [&Z, &Q, &S, &D](const p6::Key& key) {
            if (key.physical == GLFW_KEY_W)
            {
                Z = true;
            }
            if (key.physical == GLFW_KEY_A)
            {
                Q = true;
            }
            if (key.physical == GLFW_KEY_S)
            {
                S = true;
            }
            if (key.physical == GLFW_KEY_D)
            {
                D = true;
            }
        };

        ctx.key_released = [&Z, &Q, &S, &D](const p6::Key& key) {
            if (key.physical == GLFW_KEY_W)
            {
                Z = false;
            }
            if (key.physical == GLFW_KEY_A)
            {
                Q = false;
            }
            if (key.physical == GLFW_KEY_S)
            {
                S = false;
            }
            if (key.physical == GLFW_KEY_D)
            {
                D = false;
            }
        };

        ctx.mouse_dragged = [&ViewMatrixCamera](const p6::MouseDrag& button) {
            ViewMatrixCamera.rotateLeft(-button.delta.x * 50);
            ViewMatrixCamera.rotateUp(button.delta.y * 50);
        };

        /*********************BOIDS & CO MATRIX*********************/

        // seaweed.draw(ctx);
        // seaweed2.draw(ctx);

        const auto for_each_obstacle = [&](ObstacleHandler const& handler) {
            for (auto const& obs : obstacle.getObstacles())
            {
                handler(obs);
            }

            for (auto const& obs : obstacle2.getObstacles())
            {
                handler(obs);
            }
        };
        ;
        boids.runBoids(p, ctx, for_each_obstacle, meals);
        obstacle.runObstacles(ctx);
        obstacle2.runObstacles(ctx);

        shader.use();

        glBindVertexArray(vaos[0]);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, earthTextureID);

        glm::mat4 ProjMatrix = glm::perspective(glm::radians(70.f), ctx.aspect_ratio(), 0.1f, 100.f);

        /******* FOOD *******/

        glm::mat4 Food1MVMatrix = ViewMatrixCamera.getViewMatrix();
        Food1MVMatrix           = glm::translate(ViewMatrixCamera.getViewMatrix(), glm::vec3(seaweed.getPos().x, seaweed.getPos().y, -5));
        Food1MVMatrix           = glm::scale(
            Food1MVMatrix,
            glm::vec3(2, 2, 2)
        );
        glm::mat4 Food1NormalMatrix = glm::transpose(glm::inverse(Food1MVMatrix));
        glUniformMatrix4fv(uniformMV, 1, GL_FALSE, glm::value_ptr(Food1MVMatrix));
        glUniformMatrix4fv(uniformMVP, 1, GL_FALSE, glm::value_ptr(ProjMatrix * Food1MVMatrix));
        glUniformMatrix4fv(uniformNormal, 1, GL_FALSE, glm::value_ptr(Food1NormalMatrix));
        glDrawArrays(GL_TRIANGLES, 0, vertices.size());

        glm::mat4 Food2MVMatrix = ViewMatrixCamera.getViewMatrix();
        Food2MVMatrix           = glm::translate(ViewMatrixCamera.getViewMatrix(), glm::vec3(seaweed2.getPos().x, seaweed2.getPos().y, -5));
        Food2MVMatrix           = glm::scale(
            Food2MVMatrix,
            glm::vec3(2, 2, 2)
        );
        glm::mat4 Food2NormalMatrix = glm::transpose(glm::inverse(Food2MVMatrix));
        glUniformMatrix4fv(uniformMV, 1, GL_FALSE, glm::value_ptr(Food2MVMatrix));
        glUniformMatrix4fv(uniformMVP, 1, GL_FALSE, glm::value_ptr(ProjMatrix * Food2MVMatrix));
        glUniformMatrix4fv(uniformNormal, 1, GL_FALSE, glm::value_ptr(Food2NormalMatrix));
        glDrawArrays(GL_TRIANGLES, 0, vertices.size());
        glBindVertexArray(0);

        /******* BOIDS *******/

        glBindVertexArray(vaos[1]);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, 0);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, fishTextureID);

        glm::mat4 boidMVMatrix = ViewMatrixCamera.getViewMatrix();
        for (int i = 0; i < boids.sizeFishpack(); ++i)
        {
            std::vector<Fish> cur = boids.getFishPack();
            glm::vec3         pos = cur[i].getPos();

            boidMVMatrix = glm::translate(ViewMatrixCamera.getViewMatrix(), glm::vec3(pos.x, pos.y, -5));
            boidMVMatrix = glm::scale(
                boidMVMatrix,
                glm::vec3(p.fishSize * 25, p.fishSize * 25, p.fishSize * 25)
            );

            glm::mat4 boidNormalMatrix = glm::transpose(glm::inverse(boidMVMatrix));

            glUniformMatrix4fv(uniformMV, 1, GL_FALSE, glm::value_ptr(boidMVMatrix));
            glUniformMatrix4fv(uniformMVP, 1, GL_FALSE, glm::value_ptr(ProjMatrix * boidMVMatrix));
            glUniformMatrix4fv(uniformNormal, 1, GL_FALSE, glm::value_ptr(boidNormalMatrix));
            glDrawArrays(GL_TRIANGLES, 0, fishVertices.size());
        }
        glBindVertexArray(0);

        /******* OBSTACLES *******/

        glBindVertexArray(vaos[0]);

        // glActiveTexture(GL_TEXTURE0);
        // glBindTexture(GL_TEXTURE_2D, 0);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, moonTextureID);

        glm::mat4 obs1MVMatrix = ViewMatrixCamera.getViewMatrix();
        glm::mat4 obs2MVMatrix = ViewMatrixCamera.getViewMatrix();

        for (int i = 0; i < 3; ++i)
        {
            glm::vec3 pos = obstacle.getObstacles()[i].getPos();
            float     siz = obstacle.getObstacles()[i].getRadius();

            obs1MVMatrix = glm::translate(ViewMatrixCamera.getViewMatrix(), glm::vec3(pos.x, pos.y, -5));
            obs1MVMatrix = glm::scale(
                obs1MVMatrix,
                glm::vec3(5 * siz, 5 * siz, 5 * siz)
            );
            glUniformMatrix4fv(uniformMV, 1, GL_FALSE, glm::value_ptr(obs1MVMatrix));
            glUniformMatrix4fv(uniformMVP, 1, GL_FALSE, glm::value_ptr(ProjMatrix * obs1MVMatrix));
            glDrawArrays(GL_TRIANGLES, 0, vertices.size());

            pos          = obstacle2.getObstacles()[i].getPos();
            siz          = obstacle2.getObstacles()[i].getRadius();
            obs2MVMatrix = glm::translate(ViewMatrixCamera.getViewMatrix(), glm::vec3(pos.x, pos.y, -5));
            obs2MVMatrix = glm::scale(
                obs2MVMatrix,
                glm::vec3(1 + siz, 1 + siz, 1 + siz)
            );
            glm::mat4 obsNormalMatrix = glm::transpose(glm::inverse(obs2MVMatrix));
            glUniformMatrix4fv(uniformMV, 1, GL_FALSE, glm::value_ptr(obs2MVMatrix));
            glUniformMatrix4fv(uniformMVP, 1, GL_FALSE, glm::value_ptr(ProjMatrix * obs2MVMatrix));
            glUniformMatrix4fv(uniformNormal, 1, GL_FALSE, glm::value_ptr(obsNormalMatrix));
            glDrawArrays(GL_TRIANGLES, 0, vertices.size());
        }
    };

    // Should be done last. It starts the infinite loop.
    ctx.start();
    glDeleteVertexArrays(1, vaos);
    glDeleteBuffers(1, vbos);
    glDeleteTextures(1, &earthTextureID);
    glDeleteTextures(1, &moonTextureID);
    glDeleteTextures(1, &fishTextureID);
}