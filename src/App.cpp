#include "App.hpp"

void App::cameraMovement(p6::Context& ctx)
{
    if (_Z)
        _ViewMatrixCamera.moveFront(0.1);
    if (_S)
        _ViewMatrixCamera.moveFront(-0.1);
    if (_Q)
        _ViewMatrixCamera.moveLeft(0.1);
    if (_D)
        _ViewMatrixCamera.moveLeft(-0.1);

    ctx.key_pressed = [this](const p6::Key& key) {
        if (key.physical == GLFW_KEY_W)
            _Z = true;
        if (key.physical == GLFW_KEY_S)
            _S = true;
        if (key.physical == GLFW_KEY_A)
            _Q = true;
        if (key.physical == GLFW_KEY_D)
            _D = true;
    };

    ctx.key_released = [this](const p6::Key& key) {
        if (key.physical == GLFW_KEY_W)
            _Z = false;
        if (key.physical == GLFW_KEY_S)
            _S = false;
        if (key.physical == GLFW_KEY_A)
            _Q = false;
        if (key.physical == GLFW_KEY_D)
            _D = false;
    };

    ctx.mouse_dragged = [&](const p6::MouseDrag& button) {
        _ViewMatrixCamera.rotateLeft(-button.delta.x * 50);
        _ViewMatrixCamera.rotateUp(button.delta.y * 50);
    };
}

void App::generateBoids()
{
    _boids1 = Boids(_fishNb, 0);
    _boids2 = Boids(_fishNb2, 1);
}

void App::generateObstacles(p6::Context& ctx)
{
    ObstacleCollection obstacle(3);
    obstacle.generateBorders(ctx);

    _obstacles1 = obstacle;
    _obstacles2 = ObstacleCollection(3);
}

void App::generateFood()
{
    _food.emplace_back(glm::vec3(3, 1, 4), 0.5);
    _food.emplace_back(glm::vec3(-3, 1, 0), 0.5);
}

void App::createObjets()
{
    /*_arpenteur;
               _boid1Render;
                 _boid2Render;
               _obstacleRender;
                 _foodRender;
*/

    Program Objects("Shaders/multiTex3D.fs.glsl");

    Texture fishTex("Assets/textures/fish.jpg", 0);
    Model   fishV("Assets/models/fish/model_371254902470.obj");

    //_boid1Render = Object(Objects, fishV, fishTex);
    Texture fish2Tex("Assets/textures/fish2.jpg", 5);

    Texture tunaTex("Assets/textures/tuna-can.jpg", 1);
}