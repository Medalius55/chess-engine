#include "Render.h"
#include "Sprites.h"
#include <raylib.h>
#include <string>
#include <cstdio>
#include <cctype>

Color PieceFill(char p) {
    bool white = std::isupper(static_cast<unsigned char>(p)) != 0;
    return white ? Color{250,250,250,255} : Color{20,20,20,255};
}
Color PieceOutline(char p, bool darkSquare) {
    bool white = std::isupper(static_cast<unsigned char>(p)) != 0;
    if (white) return Color{0,0,0,160};
    return darkSquare ? Color{255,255,255,160} : Color{0,0,0,160};
}

void DrawBoard(const Config& cfg, const Layout& L) {
    for (int r = 0; r < 8; ++r) {
        for (int f = 0; f < 8; ++f) {
            bool dark = ((r + f) % 2) != 0;
            int x = L.boardX + f * L.tile;
            int y = L.boardY + (7 - r) * L.tile;
            DrawRectangle(x, y, L.tile, L.tile, dark ? cfg.theme.dark : cfg.theme.light);
        }
    }
}

void DrawOverlays(const Config& cfg, const Layout& L, const Board& b) {
    if (!b.selected) return;
    int sx = L.boardX + b.selected->file * L.tile;
    int sy = L.boardY + (7 - b.selected->rank) * L.tile;
    DrawRectangle(sx, sy, L.tile, L.tile, cfg.theme.select);

    const int dx[4] = { 1, -1, 0,  0};
    const int dy[4] = { 0,  0, 1, -1};
    for (int i = 0; i < 4; ++i) {
        int nf = b.selected->file + dx[i];
        int nr = b.selected->rank + dy[i];
        if (nf >= 0 && nf < 8 && nr >= 0 && nr < 8) {
            int cx = L.boardX + nf * L.tile + L.tile / 2;
            int cy = L.boardY + (7 - nr) * L.tile + L.tile / 2;
            DrawCircle(cx, cy, L.tile * 0.12f, cfg.theme.hint);
        }
    }
}

void DrawCoords(const Config& cfg, const Layout& L) {
    for (int f = 0; f < 8; ++f) {
        std::string fileLabel(1, 'a' + f);
        int x = L.boardX + f * L.tile;
        int y = L.boardY + 7 * L.tile;
        DrawText(fileLabel.c_str(), x + L.tile - 14, y + L.tile - 18, 14, cfg.theme.coords);
    }
    for (int r = 0; r < 8; ++r) {
        char buf[8];
        std::snprintf(buf, sizeof(buf), "%d", r + 1);
        int x = L.boardX - 18;
        int y = L.boardY + (7 - r) * L.tile;
        DrawText(buf, x, y + 4, 14, cfg.theme.coords);
    }
}

// ---- color utils ----
static inline unsigned char clampUC(int v){ return (unsigned char)std::max(0,std::min(255,v)); }

Color Lighten(Color c, float a) {
    // mix toward white
    int r = (int)(c.r + a * (255 - c.r));
    int g = (int)(c.g + a * (255 - c.g));
    int b = (int)(c.b + a * (255 - c.b));
    return Color{clampUC(r), clampUC(g), clampUC(b), c.a};
}

Color Darken(Color c, float a) {
    // mix toward black
    int r = (int)(c.r * (1.0f - a));
    int g = (int)(c.g * (1.0f - a));
    int b = (int)(c.b * (1.0f - a));
    return Color{clampUC(r), clampUC(g), clampUC(b), c.a};
}

// ---- palette-aware sprite renderer ----
void DrawSpriteIntoTilePal(const Sprite& s, int x, int y, int tile,
                           Color base, Color outline, bool darkSquare)
{
    // integer scaling for crisp pixels
    int px = tile / std::max(s.w, s.h);
    if (px < 1) px = 1;

    int spriteW = s.w * px;
    int spriteH = s.h * px;
    int ox = x + (tile - spriteW) / 2;
    int oy = y + (tile - spriteH) / 2;

    // precompute shades
    Color light = Lighten(base, 0.28f);
    Color dark  = Darken (base, 0.25f);

    // first pass: explicit outline pixels 'o'
    for (int r = 0; r < s.h; ++r) {
        for (int c = 0; c < s.w; ++c) {
            if (s.rows[r][c] == 'o') {
                int rx = ox + c * px;
                int ry = oy + r * px;
                DrawRectangle(rx, ry, px, px, outline);
            }
        }
    }

    // second pass: fills/shades
    for (int r = 0; r < s.h; ++r) {
        for (int c = 0; c < s.w; ++c) {
            char ch = s.rows[r][c];
            if (ch == '.' || ch == ' ' || ch == 'o') continue;

            Color use = base;
            if (ch == '+') use = light;
            else if (ch == '-') use = dark;

            int rx = ox + c * px;
            int ry = oy + r * px;
            DrawRectangle(rx, ry, px, px, use);
        }
    }
}

// ---- draw pieces: call palette renderer ----
void DrawPieces(const Config& cfg, const Layout& L, const Board& b) {
    (void)cfg;
    for (int r = 0; r < 8; ++r) {
        for (int f = 0; f < 8; ++f) {
            char p = b.squares[r][f];
            if (p == '.') continue;

            int x = L.boardX + f * L.tile;
            int y = L.boardY + (7 - r) * L.tile;
            bool dark = ((r + f) % 2) != 0;

            const Sprite* spr = GetSprite(p);
            if (spr) {
                Color base    = PieceFill(p);
                Color outline = PieceOutline(p, dark);
                DrawSpriteIntoTilePal(*spr, x, y, L.tile, base, outline, dark);
            }
        }
    }
}