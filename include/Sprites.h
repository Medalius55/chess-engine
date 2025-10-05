#pragma once
#include <cctype>

// Generic pixel sprite: rows is an array of C strings, each of length w.
// Legend (per char):
//   '#' = base fill (main color)
//   '+' = light shade (lighter than base)
//   '-' = dark shade (darker than base)
//   'o' = outline pixel (explicit outline pixel)
//   '.' or ' ' = empty
struct Sprite {
    int w;
    int h;
    const char* const* rows;
};

// 16×16 sprites (detailed silhouettes)
extern const Sprite SPR_K; // king
extern const Sprite SPR_Q; // queen
extern const Sprite SPR_R; // rook
extern const Sprite SPR_B; // bishop
extern const Sprite SPR_N; // knight
extern const Sprite SPR_P; // pawn

inline const Sprite* GetSprite(char p) {
    switch (std::tolower(static_cast<unsigned char>(p))) {
        case 'k': return &SPR_K;
        case 'q': return &SPR_Q;
        case 'r': return &SPR_R;
        case 'b': return &SPR_B;
        case 'n': return &SPR_N;
        case 'p': return &SPR_P;
        default:  return nullptr;
    }
}
