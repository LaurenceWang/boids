#include "App.hpp"
#include "glimac/sphere_vertices.hpp"

App::App(p6::Context& ctx)
    : _ObjectProgram{"Shaders/multiTex3D.fs.glsl"}, _arpenteur{_ObjectProgram, Model("Assets/models/clownfish/clownfishmid.obj"), Texture("Assets/textures/nemo.jpg", 4)}, _boid1Render{_ObjectProgram, Model("Assets/models/fish/bluefishmid.obj"), Texture("Assets/textures/bluefish1.jpg", 0)}, _boid2Render{_ObjectProgram, Model("Assets/models/fish2/bluefish2mid.obj"), Texture("Assets/textures/bluefish2.jpg", 5)}, _obstacleRender{_ObjectProgram, glimac::sphere_vertices(1.f, 32, 16)}, _foodRender{_ObjectProgram, Model("Assets/models/pork/model_582071681139.obj"), Texture("Assets/textures/pork.jpg", 4)}, _skybox(returnVertices())
{
    imGuiInit(&ctx, _parametres, _fishNb, _boids1);
    generateBoids();
    generateObstacles(ctx);
    generateFood();
    _ViewMatrixCamera.moveFront(-5.f);
    //_ViewMatrixCamera.moveLeft(-5.f);
    //_boidsRender.push_back(_boid1Render);
    //_boidsRender.push_back(_boid2Render);
}

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
        if (ImGui::GetIO().WantCaptureMouse)
        {
            return;
        }
        _ViewMatrixCamera.rotateLeft(-button.delta.x * 50);
        _ViewMatrixCamera.rotateUp(button.delta.y * 50);
    };
}

void App::generateBoids()
{
    //_boids.push_back(Boids(_fishNb, 0));
    //_boids.push_back(Boids(_fishNb2, 1));
    _boids1 = Boids(_fishNb, 0);
    _boids2 = Boids(_fishNb2, 1);
}

void App::generateObstacles(p6::Context& ctx)
{
    ObstacleCollection obstacle(0);
    obstacle.generateBorders(ctx);

    _obstacles1 = obstacle;
    _obstacles2 = ObstacleCollection(0);
}

void App::generateFood()
{
    _food.emplace_back(glm::vec3(3, 1, 4), 0.5);
    _food.emplace_back(glm::vec3(-3, 1, 0), 0.5);
}

void App::for_each_obstacle(ObstacleHandler const& handler)
{
    for (auto const& obs : _obstacles1.getObstacles())
    {
        handler(obs);
    }

    for (auto const& obs : _obstacles2.getObstacles())
    {
        handler(obs);
    }
}

/*void App::for_each_boid(std::function<void(Fish const&)> const& handler)
{
    for (auto const& boid : _boids)
    {
        for (auto const& fish : boid.getFishPack())
        {
            handler(fish);
        }
    }
}*/

/*void App::for_each_boid_renderer(std::function<void(Object const&)> const& handler)
{
    for (auto const& renderer : _boidsRender)
    {
        handler(renderer);
    }
}*/

void App::run(p6::Context& ctx)
{
    _boids1.runBoids(
        _parametres, ctx, [&](ObstacleHandler handler) { for_each_obstacle(handler); }, _food
    );
    _boids2.runBoids(
        _parametres, ctx, [&](ObstacleHandler handler) { for_each_obstacle(handler); }, _food
    );
    _obstacles1.runObstacles(ctx);
    _obstacles2.runObstacles(ctx);
}

void App::sceneRender(p6::Context& ctx)
{
    run(ctx);

    _arpenteur.createDrawEnvironment(ctx);
    glm::vec3 posArp = _ViewMatrixCamera.getPos() - 2.f * _ViewMatrixCamera.getDir();
    _arpenteur.draw(_ViewMatrixCamera.getViewMatrix(), glm::vec3(posArp.x, posArp.y - 0.5, posArp.z + 0.5), 0.f, 1);
    _arpenteur.debindVAO();

    _foodRender.createDrawEnvironment(ctx);
    for (auto& meal : _food)
        _foodRender.draw(_ViewMatrixCamera.getViewMatrix(), glm::vec3(meal.getPos().x, meal.getPos().y, -5), 0.f, 1);

    _foodRender.debindVAO();

    _boid1Render.createDrawEnvironment(ctx);
    for (auto& fish : _boids1.getFishPack())
        _boid1Render.draw(_ViewMatrixCamera.getViewMatrix(), glm::vec3(fish.getPos().x, fish.getPos().y, fish.getPos().z), 0.f, _parametres.fishSize * 25);

    _boid1Render.debindVAO();

    _boid2Render.createDrawEnvironment(ctx);
    for (auto& fish : _boids2.getFishPack())
        _boid2Render.draw(_ViewMatrixCamera.getViewMatrix(), glm::vec3(fish.getPos().x, fish.getPos().y, fish.getPos().z), 0.f, _parametres.fishSize * 25);
    _boid2Render.debindVAO();

    _obstacleRender.createDrawEnvironment(ctx);

    /*for (auto& obs : _obstacles1.getObstacles())
        _obstacleRender.draw(_ViewMatrixCamera.getViewMatrix(), glm::vec3(obs.getPos().x, obs.getPos().y, obs.getPos().z), 0.f, 0.02 * obs.getRadius());*/

    /*for (auto& obs : _obstacles1.getObstacles())
        _obstacleRender.draw(_ViewMatrixCamera.getViewMatrix(), glm::vec3(obs.getPos().x, obs.getPos().y, obs.getPos().z), 0.f, 1);*/

    for (auto& obs : _obstacles2.getObstacles())
        _obstacleRender.draw(_ViewMatrixCamera.getViewMatrix(), glm::vec3(obs.getPos().x, obs.getPos().y, obs.getPos().z), 0.f, 0.02 * obs.getRadius());

    _obstacleRender.debindVAO();

    _skybox.renderSkybox(glm::mat4(glm::mat3(_ViewMatrixCamera.getViewMatrix())), ctx);
}

void App::sceneClean()
{
    _boid1Render.deleteVBO_VAO();
    _boid2Render.deleteVBO_VAO();
    _foodRender.deleteVBO_VAO();
    _arpenteur.deleteVBO_VAO();
    _skybox.deleteVBO_VAO();
}
