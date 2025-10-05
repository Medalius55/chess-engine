#include "Layout.h"

Layout MakeLayout(const Config& cfg) {
    Layout L;
    const int boardSizePx = cfg.screenH - 2 * cfg.padding;
    L.tile = boardSizePx / 8;
    L.boardX = (cfg.screenW - (L.tile * 8)) / 2;
    L.boardY = cfg.padding;
    return L;
}