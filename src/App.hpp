#pragma once
#include <vector>
#include "Boids.hpp"
#include "Food.hpp"
#include "Object.hpp"
#include "ObstacleCollection.hpp"
#include "Params.hpp"
#include "arpenteur/arpenteur.hpp"
#include "glimac/FreeflyCamera.hpp"
#include "imgui.hpp"
#include "p6/p6.h"
#include "skybox/skybox.hpp"

using ObstacleHandler = std::function<void(Obstacle const&)>;
class App {
private:
    Program       _ObjectProgram;
    FreeflyCamera _ViewMatrixCamera;
    Params        _parametres{.separation = 0.70f, .alignment = 0.40f, .steer = 15.f, .neighRadius = 0.5f, .fishSize = 0.02f};
    int           _fishNb  = 100;
    int           _fishNb2 = 20;

    bool _Z    = false;
    bool _S    = false;
    bool _Q    = false;
    bool _D    = false;
    bool _UP   = false;
    bool _DOWN = false;

    // std::vector<Boids> _boids;
    //  std::vector<ObstacleCollection> _obstacles;
    Boids               _boids1;
    Boids               _boids2;
    ObstacleCollection  _obstacles1;
    ObstacleCollection  _obstacles2;
    std::vector<Food>   _food;
    Object              _arpenteurRender;
    Object              _boid1Render;
    Object              _boid2Render;
    Object              _obstacleRender;
    Object              _foodRender;
    std::vector<Object> _boidsRender;
    Skybox              _skybox;
    Arpenteur           _arpenteur;

    // environment

    Object _portal{_ObjectProgram, Model("Assets/models/portal/portal.obj"), Texture("Assets/models/portal/portal1.png", 0)};
    Object _island{_ObjectProgram, Model("Assets/models/island/island.obj"), Texture("Assets/models/island/island.png", 0)};
    Object _tree{_ObjectProgram, Model("Assets/models/tree/tree.obj"), Texture("Assets/models/tree/tree.jpg", 0)};
    Object _chest{_ObjectProgram, Model("Assets/models/chest/chest-test.obj"), Texture("Assets/models/chest/chest.png", 0)};
    Object _crystal{_ObjectProgram, Model("Assets/models/crystals/crystals.obj"), Texture("Assets/models/crystals/crystals.png", 0)};
    Object _castle{_ObjectProgram, Model("Assets/models/castle/castle.obj"), Texture("Assets/models/castle/castle.png", 0)};

    // std::vector<std::pair<Object, objectParameters>> _environment;
    std::vector<std::pair<std::unique_ptr<Object>, objectParameters>> _environment;

    void generateBoids();
    void generateObstacles(p6::Context& ctx);
    void generateFood();
    void generateEnvironment();
    void run(p6::Context& ctx);
    void for_each_obstacle(ObstacleHandler const& handler);
    // void for_each_boid(std::function<void(Fish const&)> const& handler);
    // void for_each_boid_renderer(std::function<void(Object const&)> const& handler);

public:
    App(p6::Context& ctx);
    void cameraMovement(p6::Context& ctx);
    void sceneRender(p6::Context& ctx);
    void sceneClean();

    FreeflyCamera getCamera()
    {
        return _ViewMatrixCamera;
    };
};
