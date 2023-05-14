#include <stdlib.h>
#include <cstddef>
#include "doctest/parts/doctest_fwd.h"
#include "p6/p6.h"
#define DOCTEST_CONFIG_IMPLEMENT
#include <random>
#include "Boids.hpp"
#include "Food.hpp"
#include "Model.hpp"
#include "Object.hpp"
#include "ObstacleCollection.hpp"
#include "Params.hpp"
#include "Program.hpp"
#include "Texture.hpp"
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

int main()
{
    auto                                   ctx      = p6::Context{{.title = "Swimming with boids"}};
    const std::vector<glimac::ShapeVertex> vertices = glimac::sphere_vertices(0.2f, 32, 16);

    /********************PROGRAMS***********************/

    Program lightAndText{};
    Program Objects("Shaders/multiTex3D.fs.glsl");
    Program light("Shaders/pointLight.fs.glsl");

    // GLint uFishTexture = glGetUniformLocation(Objects._Program.id(), "uFishTexture");
    // GLint uMoonTexture = glGetUniformLocation(Objects._Program.id(), "uMoonTexture");

    /********************TEXTURE LOADING***********************/
    glEnable(GL_DEPTH_TEST);

    Texture fishTex("Assets/textures/fish.jpg", 0);
    Texture tunaTex("Assets/textures/tuna-can.jpg", 1);
    Texture porkTex("Assets/textures/pork.jpg", 2);
    Texture moonTexture("Assets/textures/MoonMap.jpg", 3);
    // Texture arpTex("Assets/textures/arp.png", 4);
    // Texture arpTex("Assets/textures/arpd.png", 4);
    Texture arpTex("Assets/textures/Robot.png", 4);

    /*****************************MODEL LOADING****************************/

    Model fishV("Assets/models/fish/model_371254902470.obj");
    Model tunaCan("Assets/models/tuna-can/remeshed_pPhwHX.obj");
    Model pork("Assets/models/pork/model_582071681139.obj");
    // Model arpenteur("Assets/models/arpenteur/arp.obj");
    // Model arpenteur("Assets/models/mantabu/Montabu02.obj");
    Model arpenteur("Assets/models/robot/Robot.obj");

    /*****OBJECT CREATION******/

    Object clownFish(Objects, fishV, fishTex);
    Object obsta(Objects, tunaCan, tunaTex);
    Object food(Objects, pork, porkTex);
    Object lightTest(light, vertices, moonTexture);
    Object arpObj(Objects, arpenteur, arpTex);

    /***********************************VBO & VAOS*************************************/

    //////////////VBO
    GLuint vbos[2];
    glGenBuffers(2, vbos);

    // sphere
    glBindBuffer(GL_ARRAY_BUFFER, vbos[0]);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glimac::ShapeVertex), vertices.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    ///////////////VAO
    GLuint vaos[2];
    glGenVertexArrays(2, vaos);

    // sphere
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

    /********************BOIDS & CO INIT********************/

    // ctx.maximize_window();
    int    fishNb = 100;
    Params p{.separation = 0.070f, .alignment = 0.040f, .steer = 6.f, .neighRadius = 0.5f, .fishSize = 0.02f};
    Boids  boids(fishNb, 0);
    int    fishNb2 = 20;
    Boids  boids2(fishNb2, 1);

    imGuiInit(&ctx, p, fishNb, boids);

    imGuiInit(&ctx, p, fishNb, boids);

    ObstacleCollection obstacle(3);
    obstacle.generateBorders(ctx);
    ObstacleCollection obstacle2(3);

    Food              seaweed(glm::vec3(3, 1, 4), 0.5);
    Food              seaweed2(glm::vec3(-3, 1, 0), 0.5);
    std::vector<Food> meals;
    meals.push_back(seaweed);
    meals.push_back(seaweed2);

    FreeflyCamera ViewMatrixCamera = FreeflyCamera();

    bool Z = false;
    bool Q = false;
    bool S = false;
    bool D = false;

    ctx.update = [&]() {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        /***********************CAMERA MOUSE************************/

        ctx.background({0.33, 0.8, 0.98});
        // ctx.background({0, 0, 0});

        if (Z)
            ViewMatrixCamera.moveFront(0.1);
        if (S)
            ViewMatrixCamera.moveFront(-0.1);
        if (Q)
            ViewMatrixCamera.moveLeft(0.1);
        if (D)
            ViewMatrixCamera.moveLeft(-0.1);

        ctx.key_pressed = [&Z, &S, &Q, &D](const p6::Key& key) {
            if (key.physical == GLFW_KEY_W)
                Z = true;
            if (key.physical == GLFW_KEY_S)
                S = true;
            if (key.physical == GLFW_KEY_A)
                Q = true;
            if (key.physical == GLFW_KEY_D)
                D = true;
        };

        ctx.key_released = [&Z, &S, &Q, &D](const p6::Key& key) {
            if (key.physical == GLFW_KEY_W)
                Z = false;
            if (key.physical == GLFW_KEY_S)
                S = false;
            if (key.physical == GLFW_KEY_A)
                Q = false;
            if (key.physical == GLFW_KEY_D)
                D = false;
        };

        ctx.mouse_dragged = [&ViewMatrixCamera](const p6::MouseDrag& button) {
            ViewMatrixCamera.rotateLeft(-button.delta.x * 50);
            ViewMatrixCamera.rotateUp(button.delta.y * 50);
        };

        /*********************BOIDS & CO MATRIX*********************/

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

        boids.runBoids(p, ctx, for_each_obstacle, meals);
        obstacle.runObstacles(ctx);
        obstacle2.runObstacles(ctx);

        // Objects._Program.use();

        // glUniform1i(uFishTexture, 0);
        // glUniform1i(uMoonTexture, 1);

        glm::mat4 ProjMatrix = glm::perspective(glm::radians(70.f), ctx.aspect_ratio(), 0.1f, 100.f);

        /************************ ARPENTEUR ******************/

        arpObj.createDrawEnvironment(ctx);
        glm::vec3 posArp = ViewMatrixCamera.getPos() - 2.f * ViewMatrixCamera.getDir();
        arpObj.draw(ViewMatrixCamera, glm::vec3(posArp.x, posArp.y - 1, posArp.z + 0.5), -90.f, 0.05);
        arpObj.debindVAO();

        /*************************** FOOD *******************/

        food.createDrawEnvironment(ctx);
        for (auto& meal : meals)
            food.draw(ViewMatrixCamera, glm::vec3(meal.getPos().x, meal.getPos().y, -5), 0.f, 1);

        food.debindVAO();

        /*********************** BOIDS **************************/

        clownFish.createDrawEnvironment(ctx);
        for (auto& fish : boids.getFishPack())
            clownFish.draw(ViewMatrixCamera, glm::vec3(fish.getPos().x, fish.getPos().y, fish.getPos().z), 0.f, p.fishSize * 25);

        clownFish.debindVAO();

        /*************************** OBSTACLES *************************/

        obsta.createDrawEnvironment(ctx);
        for (auto& obs : obstacle.getObstacles())
            obsta.draw(ViewMatrixCamera, glm::vec3(obs.getPos().x, obs.getPos().y, obs.getPos().z), 0.f, 0.02 * obs.getRadius());

        for (auto& obs : obstacle2.getObstacles())
            obsta.draw(ViewMatrixCamera, glm::vec3(obs.getPos().x, obs.getPos().y, obs.getPos().z), 0.f, 0.02 * obs.getRadius());

        // PLUS PROPRE MAIS PLUS LENT? => DEMANDER A JULES
        /*for_each_obstacle([&](const auto& obstacle) {
            obsta.draw(ViewMatrixCamera, glm::vec3(obstacle.getPos().x, obstacle.getPos().y, obstacle.getPos().z), 0.02 * obstacle.getRadius());
        });*/

        obsta.debindVAO();

        /*************LIGHT*****************/

        glBindVertexArray(vaos[0]);
        light._Program.use();

        glm::mat4 lightMVMatrix = ViewMatrixCamera.getViewMatrix();
        lightMVMatrix           = glm::translate(ViewMatrixCamera.getViewMatrix(), glm::vec3(0, 0, -5));
        lightMVMatrix           = glm::scale(
            lightMVMatrix,
            glm::vec3(2, 2, 2)
        );

        glm::mat4 lightNormalMatrix = glm::transpose(glm::inverse(lightMVMatrix));

        glUniformMatrix4fv(light.uMVMatrix, 1, GL_FALSE, glm::value_ptr(lightMVMatrix));
        glUniformMatrix4fv(light.uMVPMatrix, 1, GL_FALSE, glm::value_ptr(ProjMatrix * lightMVMatrix));
        glUniformMatrix4fv(light.uNormalMatrix, 1, GL_FALSE, glm::value_ptr(lightNormalMatrix));

        glm::vec3 Kd = glm::vec3(1, 1, 1);
        glm::vec3 Ks = glm::vec3(1, 1, 1);
        //  glm::vec4 lightDir = ViewMatrixCamera.getViewMatrix() * glm::vec4(1, 1, -5, 1);
        glm::vec4 lightDir = lightMVMatrix * glm::vec4(1, 0, 1, 1);

        glUniform3fv(light.uKd, 1, glm::value_ptr(Kd));
        glUniform3fv(light.uKs, 1, glm::value_ptr(Ks));
        glUniform1f(light.uShininess, 0.5);
        glUniform3fv(light.uLightPos_vs, 1, glm::value_ptr(lightDir));
        glUniform3fv(light.uLightIntensity, 1, glm::value_ptr(glm::vec3(8, 8, 8)));

        glDrawArrays(GL_TRIANGLES, 0, vertices.size());

        /*******TEST TEXTURE + LIGHT *****/

        lightAndText._Program.use();
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, moonTexture.getTextureID());
        glm::mat4 testMVMatrix = ViewMatrixCamera.getViewMatrix();
        testMVMatrix           = glm::translate(ViewMatrixCamera.getViewMatrix(), glm::vec3(-4, -2, -5));
        testMVMatrix           = glm::scale(
            testMVMatrix,
            glm::vec3(2, 2, 2)
        );

        glm::mat4 testNormalMatrix = glm::transpose(glm::inverse(testMVMatrix));
        glUniformMatrix4fv(lightAndText.uMVMatrix, 1, GL_FALSE, glm::value_ptr(testMVMatrix));
        glUniformMatrix4fv(lightAndText.uMVPMatrix, 1, GL_FALSE, glm::value_ptr(ProjMatrix * testMVMatrix));
        glUniformMatrix4fv(lightAndText.uNormalMatrix, 1, GL_FALSE, glm::value_ptr(testNormalMatrix));

        glUniform3fv(lightAndText.uKd, 1, glm::value_ptr(Kd));
        glUniform3fv(lightAndText.uKs, 1, glm::value_ptr(Ks));
        glUniform1f(lightAndText.uShininess, 1);
        glUniform3fv(lightAndText.uLightDir_vs, 1, glm::value_ptr(glm::vec3(glm::rotate(ViewMatrixCamera.getViewMatrix(), ctx.time(), glm::vec3(0, 1, 0)) * glm::vec4(1, 1, 0, 1))));
        glUniform3fv(lightAndText.uLightIntensity, 1, glm::value_ptr(glm::vec3(1, 1, 1)));

        glDrawArrays(GL_TRIANGLES, 0, vertices.size());
        glBindVertexArray(0);
    };

    // Should be done last. It starts the infinite loop.
    ctx.start();
    clownFish.deleteVBO_VAO();
    food.deleteVBO_VAO();
    obsta.deleteVBO_VAO();
    arpObj.deleteVBO_VAO();
    glDeleteVertexArrays(1, vaos);
    glDeleteBuffers(1, vbos);
    arpTex.deleteTex();
    fishTex.deleteTex();
    tunaTex.deleteTex();
    porkTex.deleteTex();
}