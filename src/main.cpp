#include <stdlib.h>
#include <cstddef>
#include "p6/p6.h"
#define DOCTEST_CONFIG_IMPLEMENT
#include <random>
#include "Boids.hpp"
#include "Fish.hpp"
#include "Params.hpp"
#include "doctest/doctest.h"

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
    ctx.maximize_window();
    auto        fishNb = 100;
    std::string text   = "Hello";

    Params p{0.001f, 0.02f, 1.5f, 0.1f, 0.02f};
    // std::vector<Fish> boids;

    ctx.imgui = [&]() {
        // Show a simple window
        ImGui::Begin("Boids sliders");
        ImGui::SliderInt("fish number", &fishNb, 50, 150);
        ImGui::SliderFloat("fish size", &p.fishSize, 0.02f, 0.08f);
        ImGui::SliderFloat("separation strength", &p.sepStr, 0.001f, 0.01f);
        ImGui::SliderFloat("alignment strength", &p.aliStr, 0.005f, 0.05f);
        ImGui::SliderFloat("cohestion strength", &p.steerStr, 0.1f, 2.5f);
        ImGui::SliderFloat("neighbour radius", &p.neighRadius, 0.07f, 0.5f);
        ImGui::InputText("Text", &text);
        ImGui::End();

        // ImGui::ShowDemoWindow();
    };

    std::cout << fishNb << std::endl;

    Boids boids;
    boids.generateFish(fishNb, p.fishSize);

    // Declare your infinite update loop.
    ctx.update = [&]() {
        ctx.background(p6::NamedColor::Blue);
        boids.runBoids(p, ctx);
    };

    // Should be done last. It starts the infinite loop.
    ctx.start();
}