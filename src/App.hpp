#pragma once
#include <cstdlib>
#include <vector>
#include "Food.hpp"
#include "Object.hpp"
#include "Params.hpp"
#include "arpenteur/arpenteur.hpp"
#include "boids/Boids.hpp"
#include "camera_test/TrackballCamera.hpp"
#include "glimac/FreeflyCamera.hpp"
#include "imgui.hpp"
#include "light/Light.hpp"
#include "obstacles/ObstacleCollection.hpp"
#include "p6/p6.h"
#include "skybox/skybox.hpp"

using ObstacleHandler = std::function<void(Obstacle const&)>;
class App {
private:
    Program       _ObjectProgram;
    FreeflyCamera _ViewMatrixCamera;
    Params        _parametres{.separation = 1.0f, .alignment = 0.40f, .steer = 15.f, .neighRadius = 0.5f, .fishSize = 0.68f};
    int           _fishNb    = 100;
    int           _fishNb2   = 20;
    int           _lodChoice = 0;

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
    std::vector<Object> _boidsRender;
    Skybox              _skybox;
    Arpenteur           _arpenteur;

    Light lights{_ObjectProgram, glm::vec4(0.f, 0.f, 0.f, 1), glm::vec4(1, 1, 1, 0), glm::vec3(0.5, 0.5, 0.5)};

    // environment
    Object _arpenteurRender{_ObjectProgram, Model("Assets/models/clownfish/clownfishmid.obj"), Texture("Assets/textures/nemo.jpg", 4)};
    Object _boid1Render{_ObjectProgram, Model("Assets/models/fish/bluefishmid.obj"), Model("Assets/models/fish/bluefishlow.obj"), Texture("Assets/textures/bluefish1.jpg", 0)};
    Object _boid2Render{_ObjectProgram, Model("Assets/models/fish2/bluefish2mid.obj"), Model("Assets/models/fish2/bluefish2low.obj"), Texture("Assets/textures/bluefish2.jpg", 5)};
    Object _boid3Render{_ObjectProgram, Model("Assets/models/dory/dorymid.obj"), Model("Assets/models/dory/dorylow.obj"), Texture("Assets/textures/dory.png", 5)};
    Object _obstacleRender{_ObjectProgram, Model("Assets/models/rock/rock.obj"), Model("Assets/models/rock/rock-low.obj"), Texture("Assets/models/rock/rock.png", 0)};
    Object _foodRender{_ObjectProgram, Model("Assets/models/plant/plant.obj"), Model("Assets/models/plant/plant-low.obj"), Texture("Assets/models/plant/plant.png", 4)};
    Object _portal{_ObjectProgram, Model("Assets/models/portal/portal.obj"), Model("Assets/models/portal/portal-low.obj"), Texture("Assets/models/portal/portal1.png", 0)};
    Object _tree{_ObjectProgram, Model("Assets/models/tree/tree.obj"), Model("Assets/models/tree/tree-low.obj"), Texture("Assets/models/tree/tree.jpg", 0)};
    Object _chest{_ObjectProgram, Model("Assets/models/chest/chest-test.obj"), Model("Assets/models/chest/chest-low.obj"), Texture("Assets/models/chest/chest.png", 0)};
    Object _crystal{_ObjectProgram, Model("Assets/models/crystals/crystals.obj"), Texture("Assets/models/crystals/crystals.png", 0)};
    Object _castle{_ObjectProgram, Model("Assets/models/castle/castle.obj"), Model("Assets/models/castle/castle-low.obj"), Texture("Assets/models/castle/castle.png", 0)};
    Object _island{_ObjectProgram, Model("Assets/models/island/island.obj"), Model("Assets/models/island/island-low.obj"), Texture("Assets/models/island/island.png", 0)};

    // objectParameters
    std::vector<objectParameters> _crys;
    std::vector<objectParameters> _isl;
    std::vector<objectParameters> _tr;
    objectParameters              _port{glm::vec3(0, -1.5, 0), 0.f, 1};
    objectParameters              _ches{glm::vec3(-9.2, 3.3, -8.7), 180.f, 1};
    objectParameters              _cas{glm::vec3(12, 2, -13), 90.f, 0.6};

    void generateBoids();
    void generateObstacles(p6::Context& ctx);
    void generateFood();
    void generateEnvironment();
    void run(p6::Context& ctx);
    void for_each_obstacle(ObstacleHandler const& handler);

public:
    App(p6::Context& ctx);
    void          cameraMovement(p6::Context& ctx);
    void          sceneRender(p6::Context& ctx);
    void          sceneClean();
    FreeflyCamera getCamera()
    {
        return _ViewMatrixCamera;
    };
};
