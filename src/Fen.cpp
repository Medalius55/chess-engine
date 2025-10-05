#include "Fen.h"

bool LoadFEN(const std::string& fen, Board& out) {
    for (auto& r : out.squares) r.fill('.');
    int rank = 7, file = 0;

    for (char c : fen) {
        if (c == ' ') break;
        if (c == '/') { rank--; file = 0; continue; }
        if (c >= '1' && c <= '8') { file += (c - '0'); continue; }
        if (file < 8 && rank >= 0 && rank < 8) out.squares[rank][file++] = c;
        else return false;
    }
    
    return true;
}