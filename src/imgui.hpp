#pragma once

#include "Boids.hpp"
#include "Params.hpp"
#include "p6/p6.h"

void imGuiInit(p6::Context* ctx, Params& p, int& fishNb, Boids& boids, int& lodChoice);