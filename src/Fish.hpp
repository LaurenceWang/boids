#pragma once

#include <vector>

class Fish {
private:
    std::vector<float> pos;
    std::vector<float> vel;
    std::vector<float> dir;

public:
    Fish();
    Fish(const Fish& f);
    Fish(std::vector<float> position, std::vector<float> velocity, std::vector<float> direction);
    ~Fish() = default;

    void drawFish();
    
};