#pragma once
#include "Config.h"
#include "Layout.h"
#include "Board.h"

struct Sprite;

Color PieceFill(char p);
Color PieceOutline(char p, bool darkSquare);

void DrawBoard(const Config& cfg, const Layout& L);
void DrawOverlays(const Config& cfg, const Layout& L, const Board& b);
void DrawPieces(const Config& cfg, const Layout& L, const Board& b);
void DrawCoords(const Config& cfg, const Layout& L);

// Color helpers
Color Lighten(Color c, float a);
Color Darken(Color c, float a);

void DrawSpriteIntoTilePal(const Sprite& s, int x, int y, int tile,
                           Color base, Color outline, bool darkSquare);
