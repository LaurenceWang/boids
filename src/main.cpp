#include <stdlib.h>
#include <cstddef>
#include "p6/p6.h"
#define DOCTEST_CONFIG_IMPLEMENT
#include <random>
#include "Fish.hpp"
#include "doctest/doctest.h"

int main(int argc, char* argv[])
{
    unsigned                              seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::mt19937                          generator(seed);
    std::uniform_real_distribution<float> uniformRealDistribution(-1, 1);
    std::uniform_real_distribution<float> uniformRealDistribution2(-1.9, 1.9);
    std::uniform_real_distribution<float> uniformRealDistribution3(-0.001, 0.001);
    { // Run the tests
        if (doctest::Context{}.run() != 0)
            return EXIT_FAILURE;
        // The CI does not have a GPU so it cannot run the rest of the code.
        const bool no_gpu_available = argc >= 2 && strcmp(argv[1], "-nogpu") == 0; // NOLINT(cppcoreguidelines-pro-bounds-pointer-arithmetic)
        if (no_gpu_available)
            return EXIT_SUCCESS;
    }

    // Actual app
    auto ctx = p6::Context{{.title = "Swimming with boids"}};
    ctx.maximize_window();

    glm::vec2 p = glm::vec2(0.f, 0.f);
    glm::vec2 v = glm::vec2(0.003f, 0.003f);
    glm::vec2 d = glm::vec2(-1.f, 1.f);
    // Speed     s = Speed(d, 0.0001f);
    // Fish      f(p, s, 0.05f);
    Fish f2;

    // std::cout << p[0] << std::endl;

    std::vector<float>
        coordinates;
    for (int i = 0; i < 100; ++i)
    {
        coordinates.push_back(uniformRealDistribution2(generator));
        coordinates.push_back(uniformRealDistribution(generator));
    }

    std::vector<Fish> boids;

    for (int i = 0; i < 50; ++i)
    {
        // glm::vec2 coord = glm::vec2(uniformRealDistribution2(generator), uniformRealDistribution(generator));
        // glm::vec2 velo  = glm::vec2(uniformRealDistribution3(generator), uniformRealDistribution3(generator));
        glm::vec2 coord = p6::random::point();
        // glm::vec2 coord = glm::vec2(0, 0);
        glm::vec2 direc = p6::random::direction();
        // glm::vec2 velo  = glm::vec2(0.001, 0.001);
        float velo = p6::random::number(0, 0.00001f);
        // glm::vec2(p6::random::number(0, 0.001f), p6::random::number(0, 0.001f));
        Speed speed = Speed(direc, velo);
        Fish  b(coord, speed, 0.05f);
        boids.push_back(b);
    }

    std::vector<float> speed;
    speed.reserve(100);
    for (int i = 0; i < 100; ++i)
    {
        speed.push_back(uniformRealDistribution3(generator));
    }

    // Declare your infinite update loop.
    ctx.update = [&]() {
        ctx.background(p6::NamedColor::Blue);
        /*ctx.circle(
            p6::Center{ctx.mouse()},
            p6::Radius{0.2f}
        );*/

        // f.move();
        // f.drawFish(ctx);

        // f2.move();
        // f2.drawFish(ctx);

        // ctx.triangle(p6::Point2D(-0.05,-0.05), p6::Point2D(0.05,-0.05), p6::Point2D(0.05,0.05), p6::Center(0,0),  );

        p.x += v.x * d.x;
        p.y += v.y * d.y;
        ctx.equilateral_triangle(p6::Center(p.x, p.y), p6::Radius{0.02f}, p6::Rotation{p6::Angle(p6::Radians{p.y})});
        /*p[0] += v * d[0];
        p[1] += v * d[1];

        ctx.square(
            p6::Center{p[0], p[1]},
            p6::Radius{0.02f}
        );*/

        /*for (int i = 0; i < 100; ++i)
        {
            coordinates[i] += speed[i];
            coordinates[i + 1] += speed[i];
            ctx.equilateral_triangle(
                p6::Center{coordinates[i], coordinates[i + 1]},
                p6::Radius{0.05f}
            );
        }*/

        for (auto& boid : boids)
        {
            boid.applyForces(boids);
            boid.move();
            boid.drawFish(ctx);
        }
        // for (unsigned int i = 0; i < boids.size(); ++i)
        // {
        //     boids[i].move();

        //     /*for (unsigned int j = 0; j < boids.size(); ++j)
        //     {
        //         if (boids[j].getDistance(boids[j + 1]) < 0.1)
        //         {
        //             std::cout << "haha" << std::endl;
        //             // boids[j + 1].steerAway(boids[j].getAwayDir(boids[j + 1]));
        //         }
        //     }
        //     // boids[i].move();*/

        //     /*std::cout << "getDistance" << boids[i].getDistance(boids[i + 1]) << std::endl;
        //     std::cout << "glm::distance" << glm::distance(boids[i].getPos(), boids[i + 1].getPos());*/
        //     boids[i].drawFish(ctx);
        // }
    };

    // Should be done last. It starts the infinite loop.
    ctx.start();
}