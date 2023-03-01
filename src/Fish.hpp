#pragma once

#include <vector>
#include "p6/p6.h"

class Fish {
private:
    std::vector<float> pos;
    std::vector<float> vel;
    std::vector<float> dir;
    float              size;

public:
    Fish();
    Fish(const Fish& f);
    Fish(std::vector<float> position, std::vector<float> velocity, std::vector<float> direction, float size);
    ~Fish() = default;

    void drawFish(p6::Context& context);
    void updatePosition(std::vector<float> position);
    void move();
};