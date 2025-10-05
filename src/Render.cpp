#include "Render.h"
#include "Sprites.h"
#include "Board.h"
#include <raylib.h>
#include <string>
#include <cstdio>
#include <cctype>
#include <algorithm>

// ----- piece palette helpers -----
Color PieceFill(char p) {
    bool white = std::isupper(static_cast<unsigned char>(p)) != 0;
    return white ? Color{250,250,250,255} : Color{20,20,20,255};
}
Color PieceOutline(char p, bool darkSquare) {
    bool white = std::isupper(static_cast<unsigned char>(p)) != 0;
    if (white) return Color{0,0,0,160};
    return darkSquare ? Color{255,255,255,160} : Color{0,0,0,160};
}

// ----- board -----
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

// ----- overlays (selection + legal targets) -----
void DrawOverlays(const Config& cfg, const Layout& L, const Board& b) {
    if (b.selected) {
        int sx = L.boardX + b.selected->file * L.tile;
        int sy = L.boardY + (7 - b.selected->rank) * L.tile;
        DrawRectangle(sx, sy, L.tile, L.tile, cfg.theme.select);
    }

    for (const auto& to : b.legalTargets) {
        int cx = L.boardX + to.file * L.tile + L.tile / 2;
        int cy = L.boardY + (7 - to.rank) * L.tile + L.tile / 2;

        DrawCircle(cx, cy, L.tile * 0.14f, cfg.theme.hint);
        if (b.charAt(to.rank, to.file) != '.') {
            DrawCircleLines(cx, cy, L.tile * 0.22f, cfg.theme.hint);
        }
    }
}

// ----- coords -----
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

// ----- color utils for sprites -----
static inline unsigned char clampUC(int v){ return (unsigned char)std::max(0,std::min(255,v)); }
Color Lighten(Color c, float a) {
    int r = (int)(c.r + a * (255 - c.r));
    int g = (int)(c.g + a * (255 - c.g));
    int b = (int)(c.b + a * (255 - c.b));
    return Color{clampUC(r), clampUC(g), clampUC(b), c.a};
}
Color Darken(Color c, float a) {
    int r = (int)(c.r * (1.0f - a));
    int g = (int)(c.g * (1.0f - a));
    int b = (int)(c.b * (1.0f - a));
    return Color{clampUC(r), clampUC(g), clampUC(b), c.a};
}

// ----- sprite renderer -----
void DrawSpriteIntoTilePal(const Sprite& s, int x, int y, int tile,
                           Color base, Color outline, bool /*darkSquare*/)
{
    int px = tile / std::max(s.w, s.h);
    if (px < 1) px = 1;

    int spriteW = s.w * px;
    int spriteH = s.h * px;
    int ox = x + (tile - spriteW) / 2;
    int oy = y + (tile - spriteH) / 2;

    Color light = Lighten(base, 0.28f);
    Color dark  = Darken (base, 0.25f);

    // outlines
    for (int r = 0; r < s.h; ++r)
        for (int c = 0; c < s.w; ++c)
            if (s.rows[r][c] == 'o')
                DrawRectangle(ox + c*px, oy + r*px, px, px, outline);

    // fills/shades
    for (int r = 0; r < s.h; ++r) {
        for (int c = 0; c < s.w; ++c) {
            char ch = s.rows[r][c];
            if (ch == '.' || ch == ' ' || ch == 'o') continue;
            Color use = (ch == '+') ? light : (ch == '-') ? dark : base;
            DrawRectangle(ox + c*px, oy + r*px, px, px, use);
        }
    }
}

// ----- pieces -----
void DrawPieces(const Config& cfg, const Layout& L, const Board& b) {
    (void)cfg;
    for (int r = 0; r < 8; ++r) {
        for (int f = 0; f < 8; ++f) {
            char p = b.charAt(r, f);
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

// ----- status bar -----
void DrawStatus(const Config& cfg, const Layout& L, const Board& b) {
    const std::string s = b.statusString();

    const int boardW = 8 * L.tile;
    const int barH   = std::max(22, L.tile / 2);
    const int margin = 8;
    const int x      = L.boardX;
    const int y      = L.boardY + 8 * L.tile + margin;

    Color bg = { 32, 160, 80, 160 };
    if (s.find("Checkmate") != std::string::npos) bg = Color{200, 40, 40, 200};
    else if (s.find("Stalemate") != std::string::npos) bg = Color{220, 180, 40, 200};

    DrawRectangle(x, y, boardW, barH, bg);

    const int fontSize = std::max(16, L.tile / 2);
    int textW = MeasureText(s.c_str(), fontSize);
    int tx = x + (boardW - textW) / 2;
    int ty = y + (barH - fontSize) / 2;

    DrawText(s.c_str(), tx+1, ty+1, fontSize, Color{0,0,0,140});
    DrawText(s.c_str(), tx,   ty,   fontSize, RAYWHITE);
}
