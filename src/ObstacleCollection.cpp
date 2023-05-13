#include "ObstacleCollection.hpp"

ObstacleCollection::ObstacleCollection(std::vector<Obstacle>& o)
    : _obstacles(o)
{}

ObstacleCollection::ObstacleCollection()
{
    std::vector<Obstacle> o;
    _obstacles = o;
}

ObstacleCollection::ObstacleCollection(int nbObstacles)
{
    generateObstacles(nbObstacles);
}

void ObstacleCollection::generateObstacles(int nbObstacles)
{
    for (int i = 0; i < nbObstacles; ++i)
    {
        Obstacle o;
        _obstacles.push_back(o);
    }
}

void ObstacleCollection::generateBorders(p6::Context& ctx)
{
    const int borderLength = 70;
    const int border       = 20;

    // not working, need to be fixed

    for (int i = 0; i < borderLength + 1; ++i)
    {
        Obstacle bor(glm::vec3(border, border, border), ctx.aspect_ratio() / borderLength, true, false);

        _obstacles.push_back(bor);
    }

    for (int i = 0; i < borderLength + 1; ++i)
    {
        Obstacle bor(glm::vec3(-border, border, border), ctx.aspect_ratio() / borderLength, true, false);

        _obstacles.push_back(bor);
    }

    for (int i = 0; i < borderLength + 1; ++i)
    {
        Obstacle bor(glm::vec3(-border, border, -border), ctx.aspect_ratio() / borderLength, false, true);

        _obstacles.push_back(bor);
    }

    for (int i = 0; i < borderLength + 1; ++i)
    {
        Obstacle bor(glm::vec3(-border, -border, -border), ctx.aspect_ratio() / borderLength, false, true);

        _obstacles.push_back(bor);
    }

    for (int i = 0; i < borderLength + 1; ++i)
    {
        Obstacle bor(glm::vec3(-border, -border, border), ctx.aspect_ratio() / borderLength, false, true);

        _obstacles.push_back(bor);
    }

    for (int i = 0; i < borderLength + 1; ++i)
    {
        Obstacle bor(glm::vec3(border, border, -border), ctx.aspect_ratio() / borderLength, false, true);

        _obstacles.push_back(bor);
    }
}

void ObstacleCollection::runObstacles(p6::Context& ctx)
{
    int i = 0;
    for (auto& obs : _obstacles)
    {
        obs.updateBorderPosition(ctx, i);
        ++i;
        // obs.draw(ctx);
    }
}
