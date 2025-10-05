#pragma once
#include "Config.h"

struct Layout {
    int tile   = 100;
    int boardX = 0;
    int boardY = 0;
};

Layout MakeLayout(const Config& cfg);