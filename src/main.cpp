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

    const std::vector<glimac::ShapeVertex> vertices = glimac::sphere_vertices(0.2f, 32, 16);
    const p6::Shader                       shader   = p6::load_shader(
                                "Shaders/3D.vs.glsl",
                                "Shaders/normals.fs.glsl"
                            );

    FreeflyCamera cam;
    cam.moveFront(-5);

    GLint uniformMVP    = glGetUniformLocation(shader.id(), "uMVPMatrix");
    GLint uniformMV     = glGetUniformLocation(shader.id(), "uMVMatrix");
    GLint uniformNormal = glGetUniformLocation(shader.id(), "uNormalMatrix");

    glEnable(GL_DEPTH_TEST);

    GLuint vbo, vao;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glimac::ShapeVertex), vertices.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    const GLuint VERTEX_ATTR_POSITION = 0;
    glEnableVertexAttribArray(VERTEX_ATTR_POSITION);

    const GLuint VERTEX_ATTR_NORMAL = 1;
    glEnableVertexAttribArray(VERTEX_ATTR_NORMAL);

    const GLuint VERTEX_ATTR_TEXCOORDS = 2;
    glEnableVertexAttribArray(VERTEX_ATTR_TEXCOORDS);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    glVertexAttribPointer(VERTEX_ATTR_POSITION, 3, GL_FLOAT, GL_FALSE, sizeof(glimac::ShapeVertex), (void*)offsetof(glimac::ShapeVertex, position));

    glVertexAttribPointer(VERTEX_ATTR_NORMAL, 3, GL_FLOAT, GL_FALSE, sizeof(glimac::ShapeVertex), (void*)offsetof(glimac::ShapeVertex, normal));

    glVertexAttribPointer(VERTEX_ATTR_TEXCOORDS, 2, GL_FLOAT, GL_FALSE, sizeof(glimac::ShapeVertex), (void*)offsetof(glimac::ShapeVertex, texCoords));

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);

    // ctx.maximize_window();
    int    fishNb = 100;
    Params p{.separation = 0.001f, .alignment = 0.02f, .steer = 1.5f, .neighRadius = 0.5f, .fishSize = 0.02f};
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

    obstacle.generateObstacles(2);

    Food              seaweed;
    Food              seaweed2;
    std::vector<Food> meals;
    meals.push_back(seaweed);
    meals.push_back(seaweed2);

    bool Z = false;
    bool Q = false;
    bool S = false;
    bool D = false;
    // Declare your infinite update loop.

    ctx.update = [&]() {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        ctx.background({0.33, 0.8, 0.98});

        if (Z)
        {
            cam.moveFront(0.1);
            // cam.moveFront(0.1);
        }
        if (Q)
        {
            // cam.moveLeft(0.1);
            cam.rotateLeft(0.1);
        }
        if (S)
        {
            cam.moveFront(-0.1);
        }
        if (D)
        {
            cam.rotateLeft(-0.1);

            // cam.moveLeft(-0.1);
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

        /*ctx.mouse_dragged = [&cam](const p6::MouseDrag& button) {
            cam.rotateLeft(-button.delta.x * 50);
            cam.rotateUp(button.delta.y * 50);
        };*/

        // MVMatrix = glm::translate(glm::mat4(1), {0.1, 0.1, 0.1});

        seaweed.draw(ctx);
        seaweed2.draw(ctx);

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

        glBindVertexArray(vao);
        glm::mat4 ProjMatrix = glm::perspective(glm::radians(70.f), ctx.aspect_ratio(), 0.1f, 100.f);
        glm::mat4 MVMatrix   = glm::translate(glm::mat4(1), glm::vec3(0., 0., -5.));
        /*MVMatrix             = glm::scale(
            MVMatrix,
            glm::vec3(0.5, 0.5, 0.5)
        );*/
        // glm::mat4 MVMatrix = cam.getViewMatrix();
        //  MVMatrix               = glm::scale(MVMatrix, glm::vec3(p.fishSize * 25, p.fishSize * 25, p.fishSize * 25));
        glm::mat4 NormalMatrix = glm::transpose(glm::inverse(MVMatrix));

        glUniformMatrix4fv(uniformNormal, 1, GL_FALSE, glm::value_ptr(NormalMatrix));
        glUniformMatrix4fv(uniformMV, 1, GL_FALSE, glm::value_ptr(MVMatrix));
        glUniformMatrix4fv(uniformMVP, 1, GL_FALSE, glm::value_ptr(ProjMatrix * MVMatrix));
        // glDrawArrays(GL_TRIANGLES, 0, vertices.size());

        for (int i = 0; i < boids.sizeFishpack(); ++i)
        {
            std::vector<Fish> cur = boids.getFishPack();
            glm::vec3         pos = cur[i].getPos();

            // MVMatrix = glm::translate(MVMatrix, glm::vec3(pos.x, pos.y, pos.z));

            MVMatrix = glm::translate(glm::mat4(1), glm::vec3(pos.x, pos.y, -5));
            //  MVMatrix = glm::translate(glm::mat4(1), pos);
            MVMatrix = glm::scale(
                MVMatrix,
                glm::vec3(p.fishSize * 25, p.fishSize * 25, p.fishSize * 25)
            );

            // MVMatrix = cam.getViewMatrix();

            // glUniformMatrix4fv(uniformNormal, 1, GL_FALSE, glm::value_ptr(NormalMatrix));
            glUniformMatrix4fv(uniformMV, 1, GL_FALSE, glm::value_ptr(MVMatrix));
            glUniformMatrix4fv(uniformMVP, 1, GL_FALSE, glm::value_ptr(ProjMatrix * MVMatrix));
            glDrawArrays(GL_TRIANGLES, 0, vertices.size());
        }

        glBindVertexArray(0);
    };

    // Should be done last. It starts the infinite loop.
    ctx.start();
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
}