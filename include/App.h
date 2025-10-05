#pragma once
#include "Config.h"
#include "Layout.h"
#include "Board.h"

struct App {
    Config cfg{};
    Layout layout{};
    bool initialized = false;

    explicit App(const Config& c) : cfg(c) {}
    void init();
    bool running() const;
    void pollInput(Board& b);
    void beginFrame() const;
    void render(const Board& b) const;
    void endFrame() const;
    void shutdown();
};