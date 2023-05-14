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
    Texture arpTex("Assets/textures/Robot.png", 4);

    /*****************************MODEL LOADING****************************/

    Model fishV("Assets/models/fish/model_371254902470.obj");
    Model tunaCan("Assets/models/tuna-can/remeshed_pPhwHX.obj");
    Model pork("Assets/models/pork/model_582071681139.obj");
    Model arpenteur("Assets/models/robot/Robot.obj");

    /*****OBJECT CREATION******/

    Object clownFish(Objects, fishV, fishTex);
    Object obsta(Objects, tunaCan, tunaTex);
    Object food(Objects, pork, porkTex);
    Object lightTest(light, vertices, moonTexture);
    Object arpObj(Objects, arpenteur, arpTex);
    Object lightSphere(light, vertices);
    Object lightAndTex(lightAndText, vertices, moonTexture);

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
        /*for (auto& obs : obstacle.getObstacles())
            obsta.draw(ViewMatrixCamera, glm::vec3(obs.getPos().x, obs.getPos().y, obs.getPos().z), 0.f, 0.02 * obs.getRadius());

        for (auto& obs : obstacle2.getObstacles())
            obsta.draw(ViewMatrixCamera, glm::vec3(obs.getPos().x, obs.getPos().y, obs.getPos().z), 0.f, 0.02 * obs.getRadius());*/

        for_each_obstacle([&](const auto& obstacle) {
            obsta.draw(ViewMatrixCamera, glm::vec3(obstacle.getPos().x, obstacle.getPos().y, obstacle.getPos().z), 0.f, 0.02 * obstacle.getRadius());
        });

        obsta.debindVAO();

        /*************LIGHT*****************/

        lightSphere.createDrawEnvironment(ctx);
        lightSphere.draw(ViewMatrixCamera, glm::vec3(0, 0, -5), 0.f, 2);
        lightSphere.debindVAO();

        lightAndTex.createDrawEnvironment(ctx);
        lightAndTex.draw(ViewMatrixCamera, glm::vec3(-4, -2, -5), 0.f, 2);
        lightAndTex.debindVAO();
    };

    // Should be done last. It starts the infinite loop.
    ctx.start();
    clownFish.deleteVBO_VAO();
    food.deleteVBO_VAO();
    obsta.deleteVBO_VAO();
    arpObj.deleteVBO_VAO();
    lightSphere.deleteVBO_VAO();
    lightAndTex.deleteVBO_VAO();
    arpTex.deleteTex();
    fishTex.deleteTex();
    tunaTex.deleteTex();
    porkTex.deleteTex();
}