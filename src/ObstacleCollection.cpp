#include "ObstacleCollection.hpp"

ObstacleCollection::ObstacleCollection(std::vector<Obstacle>& o)
    : _obstacles(o)
{}

ObstacleCollection::ObstacleCollection()
{
    std::vector<Obstacle> o;
    _obstacles = o;
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
    for (int i = 0; i < 71; ++i)
    {
        Obstacle bor(glm::vec2(-ctx.aspect_ratio(), -1 + i * 2 * (ctx.aspect_ratio() / 70)), ctx.aspect_ratio() / 70, true, false);

        _obstacles.push_back(bor);
    }

    for (int i = 0; i < 71; ++i)
    {
        Obstacle bor(glm::vec2(ctx.aspect_ratio(), -1 + i * 2 * (ctx.aspect_ratio() / 70)), ctx.aspect_ratio() / 70, true, false);

        _obstacles.push_back(bor);
    }

    for (int i = 0; i < 71; ++i)
    {
        Obstacle bor(glm::vec2(-ctx.aspect_ratio() + i * 2 * (ctx.aspect_ratio() / 70), -1), ctx.aspect_ratio() / 70, false, true);

        _obstacles.push_back(bor);
    }

    for (int i = 0; i < 71; ++i)
    {
        Obstacle bor(glm::vec2(-ctx.aspect_ratio() + i * (2 * ctx.aspect_ratio() / 70), 1), ctx.aspect_ratio() / 70, false, true);

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
        obs.draw(ctx);
    }
}
