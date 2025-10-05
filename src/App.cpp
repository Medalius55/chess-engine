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
    // Hotkeys
    if (IsKeyPressed(KEY_Z)) { if (b.undo()) { b.selected.reset(); b.legalTargets.clear(); } }
    if (IsKeyPressed(KEY_X)) { if (b.redo()) { b.selected.reset(); b.legalTargets.clear(); } }

    // Mouse clicks on HUD buttons
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        Vector2 m = GetMousePosition();
        HudButtons hb = ComputeHudButtons(layout);
        if (CheckCollisionPointRec(m, hb.undo)) {
            if (b.undo()) { b.selected.reset(); b.legalTargets.clear(); }
            return; // consume click
        }
        if (CheckCollisionPointRec(m, hb.redo)) {
            if (b.redo()) { b.selected.reset(); b.legalTargets.clear(); }
            return; // consume click
        }
        // Otherwise, pass through to board click
        HandleClick(b, layout, m);
    }
}

void App::beginFrame() const { BeginDrawing(); }
void App::endFrame()   const { EndDrawing(); }

void App::render(const Board& b) const {
    ClearBackground(RAYWHITE);

    DrawBoard(cfg, layout);
    DrawPieces(cfg, layout, b);
    DrawOverlays(cfg, layout, b);
    DrawCoords(cfg, layout);

    DrawStatus(cfg, layout, b);
    DrawHudButtons(cfg, layout, b, ComputeHudButtons(layout));
}

void App::shutdown() {
    if (initialized) CloseWindow();
    initialized = false;
}
