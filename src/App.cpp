#include "App.h"
#include "Render.h"
#include "Input.h"
#include <raylib.h>

void App::init() {
    SetConfigFlags(FLAG_MSAA_4X_HINT);
    InitWindow(cfg.screenW, cfg.screenH, "chess");
    SetTargetFPS(120);
    layout = MakeLayout(cfg);
    initialized = true;
}

bool App::running() const {
    return !WindowShouldClose();
}

void App::pollInput(Board& b) {
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        HandleClick(b, layout, GetMousePosition());
    }
}

void App::beginFrame() const { BeginDrawing(); }
void App::endFrame()   const { EndDrawing(); }

void App::render(const Board& b) const {
    ClearBackground(RAYWHITE);
    DrawText("Pixel-art pieces (procedural)", 10, 10, cfg.uiFontSize, cfg.theme.text);
    DrawBoard(cfg, layout);
    DrawOverlays(cfg, layout, b);
    DrawPieces(cfg, layout, b);
    DrawCoords(cfg, layout);
}

void App::shutdown() {
    if (initialized) CloseWindow();
    initialized = false;
}