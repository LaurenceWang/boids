#pragma once
#include <vector>
#include "Boids.hpp"
#include "Food.hpp"
#include "Object.hpp"
#include "ObstacleCollection.hpp"
#include "Params.hpp"
#include "glimac/FreeflyCamera.hpp"
#include "imgui.hpp"
#include "p6/p6.h"

using ObstacleHandler = std::function<void(Obstacle const&)>;
class App {
private:
    Program       _ObjectProgram;
    FreeflyCamera _ViewMatrixCamera;
    Params        _parametres{.separation = 0.070f, .alignment = 0.040f, .steer = 6.f, .neighRadius = 0.5f, .fishSize = 0.02f};
    int           _fishNb  = 100;
    int           _fishNb2 = 20;

    bool _Z = false;
    bool _S = false;
    bool _Q = false;
    bool _D = false;

    // std::vector<Boids>              _boids;
    // std::vector<ObstacleCollection> _obstacles;
    Boids              _boids1;
    Boids              _boids2;
    ObstacleCollection _obstacles1;
    ObstacleCollection _obstacles2;
    std::vector<Food>  _food;
    Object             _arpenteur;
    Object             _boid1Render;
    Object             _boid2Render;
    Object             _obstacleRender;
    Object             _foodRender;

    void generateBoids();
    void generateObstacles(p6::Context& ctx);
    void generateFood();
    void run(p6::Context& ctx);
    void for_each_obstacle(ObstacleHandler const& handler);

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
