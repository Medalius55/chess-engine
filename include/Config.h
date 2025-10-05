#pragma once
#include <raylib.h>

struct ColorsTheme {
    Color light  = {240,217,181,255};
    Color dark   = {181,136, 99,255};
    Color select = {120,180,255,140};
    Color hint   = { 40,200,100,120};
    Color coords = { 50, 50, 50,255};
    Color text   = { 60, 60, 60,255};
};

struct Config {
    int screenW = 800;
    int screenH = 800;
    int padding = 40;
    ColorsTheme theme{};
    int uiFontSize = 18;
};
