#include <stdlib.h>
#include <cstddef>
#include "doctest/parts/doctest_fwd.h"
#include "p6/p6.h"
#define DOCTEST_CONFIG_IMPLEMENT
#include <random>
#include "Boids.hpp"
#include "Fish.hpp"
#include "Obstacle.hpp"
#include "Params.hpp"
#include "doctest/doctest.h"
#include "imgui.hpp"

int main(int argc, char* argv[])
{
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
    // ctx.maximize_window();
    int    fishNb = 100;
    Params p{0.001f, 0.02f, 1.5f, 0.1f, 0.02f};
    bool   nbChanged   = false;
    bool   sizeChanged = false;
    imguiinit(&ctx, p, fishNb, nbChanged, sizeChanged);

    // std::vector<Fish> boids;

    // std::cout << fishNb << std::endl;

    Boids boids;
    boids.generateFish(fishNb, p.fishSize);

    std::vector<Obstacle> obs;
    Obstacle              o;
    obs.push_back((o));

    // std::vector<Obstacle> borders;
    /*for (float i = 0; i < ctx.inverse_aspect_ratio(); i += 0.01f)
    {
        Obstacle bor(glm::vec2(-ctx.aspect_ratio(), -ctx.inverse_aspect_ratio() + i * 150 * 0.02f), 0.02f);

        obs.push_back(bor);
    }*/

    /*for (float i = 0; i < ctx.aspect_ratio(); i += 0.01f)
    {
        Obstacle bor(glm::vec2(-ctx.aspect_ratio() + i * 150 * 0.02f, -ctx.inverse_aspect_ratio()), 0.02f);

        obs.push_back(bor);
    }*/
    for (int i = 0; i < ctx.inverse_aspect_ratio() * 100; ++i)
    {
        Obstacle bor(glm::vec2(-ctx.aspect_ratio(), -1 + i * 2 * 0.02f), 0.02f);

        obs.push_back(bor);
    }

    for (int i = 0; i < ctx.aspect_ratio() * 70; ++i)
    {
        Obstacle bor(glm::vec2(-ctx.aspect_ratio() + i * 2 * 0.02f, -1), 0.02f);

        obs.push_back(bor);
    }

    for (int i = 0; i < ctx.aspect_ratio() * 70; ++i)
    {
        Obstacle bor(glm::vec2(-ctx.aspect_ratio() + i * 2 * 0.02f, 1), 0.02f);

        obs.push_back(bor);
    }

    for (int i = 0; i < ctx.inverse_aspect_ratio() * 100; ++i)
    {
        Obstacle bor(glm::vec2(+ctx.aspect_ratio(), -1 + i * 2 * 0.02f), 0.02f);

        obs.push_back(bor);
    }

    // Declare your infinite update loop.
    ctx.update = [&]() {
        if (nbChanged)
        {
            boids.adjustBoids(fishNb, p.fishSize);
        }
        if (sizeChanged)
        {
            boids.resizeBoids(p.fishSize);
        }
        ctx.background(p6::NamedColor::Blue);
        boids.runBoids(p, ctx, obs);

        for (int i = 0; i < obs.size(); ++i)
        {
            obs[i].draw(ctx);
        };

        // o.draw(ctx);
        // bor.draw(ctx);
    };

    // Should be done last. It starts the infinite loop.
    ctx.start();
}
