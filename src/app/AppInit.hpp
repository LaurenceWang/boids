#pragma once
#include "App.hpp"

void App::init(p6::Context& ctx)
{
    imGuiInit(&ctx, _parametres, _fishNb, _boids1, _lodChoice);
    generateBoids();
    generateObstacles(ctx);
    generateFood();
    generateEnvironment();
}

void App::generateBoids()
{
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
    _crys.push_back(objectParameters{glm::vec3(1.5, -1.15, 0), 0, 1});
    _crys.push_back(objectParameters{glm::vec3(-1.5, -1.15, 0), 0, 1});
    _crys.push_back(objectParameters{glm::vec3(0, -1.15, -1), 0, 0.9});
    _crys.push_back(objectParameters{glm::vec3(0.7, -1.15, 0.5), 0, 0.9});
    _crys.push_back(objectParameters{glm::vec3(-0.2, -1.15, 0.6), 45, 0.8});

    _isl.push_back(objectParameters{glm::vec3(0, -1.5, 0), 0.f, 1});
    _isl.push_back(objectParameters{glm::vec3(-9, 3, -9), 45.f, 1});

    _tr.push_back(objectParameters{glm::vec3(-8.3, 3.3, -9), 0.f, 1});
    _tr.push_back(objectParameters{glm::vec3(-10, 3.3, -10), 0.f, 0.7});
}