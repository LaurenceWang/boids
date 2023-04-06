
#include "imgui.hpp"
#include <pthread.h>

void imguiinit(p6::Context* ctx, Params& p)
{
    auto        fishNb = 100;
    std::string text   = "Hello";

    (*ctx).imgui = [&]() {
        // Show a simple window
        ImGui::Begin("Boids sliders");
        bool fishNbChanged         = ImGui::SliderInt("fish number", &fishNb, 50, 150);
        bool fishSizeChanged       = ImGui::SliderFloat("fish size", &p.fishSize, 0.02f, 0.08f);
        bool fishSeparationChanged = ImGui::SliderFloat("separation strength", &p.sepStr, 0.001f, 0.01f);
        bool fishAlignementChanged = ImGui::SliderFloat("alignment strength", &p.aliStr, 0.005f, 0.05f);
        bool fishCohesionChanged   = ImGui::SliderFloat("cohesion strength", &p.steerStr, 0.1f, 2.5f);
        bool fishNeighboursChanged = ImGui::SliderFloat("neighbour radius", &p.neighRadius, 0.07f, 0.5f);
        // ImGui::InputText("Text", &text);
        ImGui::End();
    };
}