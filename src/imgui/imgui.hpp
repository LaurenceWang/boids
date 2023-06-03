#pragma once

#include "Params.hpp"
#include "boids/Boids.hpp"
#include "p6/p6.h"

void imGuiInit(p6::Context* ctx, Params& p, int& fishNb, Boids& boids, int& lodChoice);