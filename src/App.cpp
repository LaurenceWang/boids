#include "App.hpp"
#include "glimac/sphere_vertices.hpp"

App::App(p6::Context& ctx)
    : _ObjectProgram{}, _skybox(returnVertices())
{
    imGuiInit(&ctx, _parametres, _fishNb, _boids1, _lodChoice);
    generateBoids();
    generateObstacles(ctx);
    generateFood();

    generateEnvironment();
    _ViewMatrixCamera.moveFront(-12.f);
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
    if (_UP)
        _ViewMatrixCamera.moveUp(0.1);
    if (_DOWN)
        _ViewMatrixCamera.moveUp(-0.1);

    ctx.key_pressed = [this](const p6::Key& key) {
        if (key.physical == GLFW_KEY_W)
            _Z = true;
        if (key.physical == GLFW_KEY_S)
            _S = true;
        if (key.physical == GLFW_KEY_A)
            _Q = true;
        if (key.physical == GLFW_KEY_D)
            _D = true;
        if (key.physical == GLFW_KEY_F)
            _UP = true;
        if (key.physical == GLFW_KEY_SPACE)
            _DOWN = true;
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
        if (key.physical == GLFW_KEY_F)
            _UP = false;
        if (key.physical == GLFW_KEY_SPACE)
            _DOWN = false;
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
    std::vector<Obstacle> obstacles;
    obstacles.push_back(Obstacle(glm::vec3(-1.7, -7, 2.5), 0.7));
    obstacles.push_back(Obstacle(glm::vec3(3.2, -7, -2), 0.7));
    obstacles.push_back(Obstacle(glm::vec3(-2, -3, -5), 1));
    obstacles.push_back(Obstacle(glm::vec3(-4, -5.5, -0.5), 0.8));
    obstacles.push_back(Obstacle(glm::vec3(2, -4.7, -8), 0.8));
    _obstacles2 = ObstacleCollection(obstacles);
}

void App::generateFood()
{
    _food.emplace_back(glm::vec3(3, 1, -4), 0.5);
    _food.emplace_back(glm::vec3(-5, 0, 3), 0.5);
}

void App::generateEnvironment()
{
    Model modPortal("Assets/models/portal/portal.obj");

    objectParameters portalParameters{glm::vec3(0, -1.5, 0), 0.f, 1};

    // Stocker un pointeur vers l'objet dans _environment
    _environment.emplace_back(std::make_pair(modPortal, portalParameters));

    /*Model   modPortal("Assets/models/portal/portal.obj");
    Texture texPortal("Assets/models/portal/portal1.png", 0);
    Object  portal(_ObjectProgram, Model("Assets/models/portal/portal.obj"), Texture("Assets/models/portal/portal1.png", 0));
    // Object           portal(_ObjectProgram, modPortal, texPortal);
    objectParameters portalParameters{glm::vec3(0, -1.5, 0), 0.f, 1};

    _environment.emplace_back(std::make_pair(std::move(Object(_ObjectProgram, Model("Assets/models/portal/portal.obj"), Texture("Assets/models/portal/portal1.png", 0))), objectParameters{glm::vec3(0, -1.5, 0), 0.f, 1}));*/

    //_environment.push_back(std::make_pair(Object(_ObjectProgram, modPortal, texPortal), portalParameters));

    // environment.push_back(portal, glm::vec3)
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
    _arpenteur.updatePosition(_ViewMatrixCamera);
}

void App::sceneRender(p6::Context& ctx)
{
    run(ctx);
    glm::vec3 posArp{_arpenteur.getPos().x, _arpenteur.getPos().y - 0.5, _arpenteur.getPos().z - 4};

    _ObjectProgram._shader.use();
    lights.sendUniform(_ViewMatrixCamera.getViewMatrix(), posArp);

    _arpenteurRender.createDrawEnvironment(ctx);

    _arpenteurRender.draw(_ViewMatrixCamera.getViewMatrix(), glm::vec3(posArp.x, posArp.y, posArp.z + 1.5), -_arpenteur.getDir(), 1, posArp);
    _arpenteurRender.debindVAO();

    _foodRender.createDrawEnvironment(ctx);
    for (auto& meal : _food)
    {
        _boid1Render.adjustLOD(_arpenteur.getPos(), meal.getPos(), _lodChoice);
        _foodRender.draw(_ViewMatrixCamera.getViewMatrix(), glm::vec3(meal.getPos()), 0.f, 1, posArp);
    }

    _foodRender.debindVAO();

    _boid1Render.createDrawEnvironment(ctx);
    for (auto& fish : _boids1.getFishPack())
    {
        _boid1Render.adjustLOD(_arpenteur.getPos(), fish.getPos(), _lodChoice);
        _boid1Render.draw(_ViewMatrixCamera.getViewMatrix(), fish.getPos(), 0.f, _parametres.fishSize, posArp);
    }
    _boid1Render.debindVAO();

    _boid2Render.createDrawEnvironment(ctx);
    for (auto& fish : _boids2.getFishPack())
    {
        _boid1Render.adjustLOD(_arpenteur.getPos(), fish.getPos(), _lodChoice);
        _boid2Render.draw(_ViewMatrixCamera.getViewMatrix(), fish.getPos(), 0.f, _parametres.fishSize, posArp);
    }

    _boid2Render.debindVAO();

    _boid3Render.createDrawEnvironment(ctx);
    _boid3Render.draw(_ViewMatrixCamera.getViewMatrix(), _boids2.getFishPack().at(0).getPos(), 0.f, _parametres.fishSize * 1.3, posArp);
    _boid3Render.debindVAO();
    _obstacleRender.createDrawEnvironment(ctx);

    // for (auto& obs : _obstacles1.getObstacles())
    //_obstacleRender.draw(_ViewMatrixCamera.getViewMatrix(), glm::vec3(obs.getPos().x, obs.getPos().y, obs.getPos().z), 0.f, 0.02 * obs.getRadius());

    // for (auto& obs : _obstacles1.getObstacles())
    //_obstacleRender.draw(_ViewMatrixCamera.getViewMatrix(), glm::vec3(obs.getPos().x, obs.getPos().y, obs.getPos().z + 1), 0.f, 1);

    /*for (auto& obs : _obstacles2.getObstacles())
    {
        _obstacleRender.adjustLOD(_arpenteur.getPos(), obs.getPos(), _lodChoice);
        _obstacleRender.draw(_ViewMatrixCamera.getViewMatrix(), glm::vec3(obs.getPos().x, obs.getPos().y, obs.getPos().z), 0.f, obs.getRadius(), posArp);
    }

    _obstacleRender.debindVAO();*/

    _portal.createDrawEnvironment(ctx);
    _portal.adjustLOD(_arpenteur.getPos(), glm::vec3(0, -1.5, 0), _lodChoice);
    _portal.draw(_ViewMatrixCamera.getViewMatrix(), glm::vec3(0, -1.5, 0), 0.f, 1, posArp);
    _portal.debindVAO();

    _island.createDrawEnvironment(ctx);
    _island.adjustLOD(_arpenteur.getPos(), glm::vec3(0, -1.5, 0), _lodChoice);
    _island.draw(_ViewMatrixCamera.getViewMatrix(), glm::vec3(0, -1.5, 0), 45.f, 1, posArp);
    _island.adjustLOD(_arpenteur.getPos(), glm::vec3(-9, 3, -9), _lodChoice);
    _island.draw(_ViewMatrixCamera.getViewMatrix(), glm::vec3(-9, 3, -9), 45.f, 1, posArp);
    _island.debindVAO();

    _tree.createDrawEnvironment(ctx);
    _tree.adjustLOD(_arpenteur.getPos(), glm::vec3(-8.3, 3.3, -9), _lodChoice);
    _tree.draw(_ViewMatrixCamera.getViewMatrix(), glm::vec3(-8.3, 3.3, -9), 0.f, 1, posArp);
    _tree.adjustLOD(_arpenteur.getPos(), glm::vec3(-10, 3.3, -10), _lodChoice);
    _tree.draw(_ViewMatrixCamera.getViewMatrix(), glm::vec3(-10, 3.3, -10), 0.f, 0.7, posArp);
    _tree.debindVAO();

    _chest.createDrawEnvironment(ctx);
    _chest.adjustLOD(_arpenteur.getPos(), glm::vec3(-9.2, 3.3, -8.7), _lodChoice);
    _chest.draw(_ViewMatrixCamera.getViewMatrix(), glm::vec3(-9.2, 3.3, -8.7), 180.f, 1, posArp);
    _chest.debindVAO();

    _crystal.createDrawEnvironment(ctx);
    _crystal.draw(_ViewMatrixCamera.getViewMatrix(), glm::vec3(1.5, -1.15, 0), 0, 1, posArp);
    _crystal.draw(_ViewMatrixCamera.getViewMatrix(), glm::vec3(-1.5, -1.15, 0), 0, 1, posArp);
    _crystal.draw(_ViewMatrixCamera.getViewMatrix(), glm::vec3(0, -1.15, -1), 0, 0.9, posArp);
    _crystal.draw(_ViewMatrixCamera.getViewMatrix(), glm::vec3(0.7, -1.15, 0.5), 0, 0.9, posArp);
    _crystal.draw(_ViewMatrixCamera.getViewMatrix(), glm::vec3(-0.2, -1.15, 0.6), 45, 0.8, posArp);
    _crystal.debindVAO();

    _castle.createDrawEnvironment(ctx);
    _chest.adjustLOD(_arpenteur.getPos(), glm::vec3(12, 2, -13), _lodChoice);
    _castle.draw(_ViewMatrixCamera.getViewMatrix(), glm::vec3(12, 2, -13), 90.f, 0.6, posArp);
    _castle.debindVAO();

    /*for (auto& env : _environment)
        (env.first).draw(_ViewMatrixCamera.getViewMatrix(), env.second);*/
    /*for (auto& env : _environment)
        std::cout << env.second.scaleSize << std::endl;*/

    _skybox.renderSkybox(glm::mat4(glm::mat3(_ViewMatrixCamera.getViewMatrix())), ctx);
}

void App::sceneClean()
{
    _portal.deleteRessources();
    _boid1Render.deleteRessources();
    _boid2Render.deleteRessources();
    _foodRender.deleteRessources();
    _arpenteurRender.deleteRessources();
    _portal.deleteRessources();
    _island.deleteRessources();
    _tree.deleteRessources();
    _chest.deleteRessources();
    _crystal.deleteRessources();
    _castle.deleteRessources();
    _skybox.deleteRessources();
}
