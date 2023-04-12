
#include "imgui.hpp"
#include <pthread.h>

void imguiinit(p6::Context* ctx, Params& p, int& fishNb, bool& nbChanged, bool& sizeChanged)
{
    //  std::string text = "Hello";

    (*ctx).imgui = [&]() {
        // Show a simple window
        ImGui::Begin("Boids sliders");
        nbChanged =
            if (ImGui::SliderInt("fish number", &fishNb, 1, 250)) // USe this approach instead of storing booleans
        {
            boid.resize;
        }
        sizeChanged = ImGui::SliderFloat("fish size", &p.fishSize, 0.02f, 0.08f);
        ImGui::SliderFloat("separation strength", &p.sepStr, 0.001f, 0.01f);
        ImGui::SliderFloat("alignment strength", &p.aliStr, 0.005f, 0.05f);
        ImGui::SliderFloat("cohesion strength", &p.steerStr, 0.1f, 2.5f);
        ImGui::SliderFloat("neighbour radius", &p.neighRadius, 0.07f, 0.5f);
        // ImGui::InputText("Text", &text);
        ImGui::End();
    };
}