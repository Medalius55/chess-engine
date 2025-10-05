#include "Fen.h"
#include <cctype>

bool LoadFEN(const std::string& fen, Board& out) {
    // clear board
    for (auto& r : out.grid) for (auto& c : r) c.reset();

    int rank = 7, file = 0;
    for (char ch : fen) {
        if (ch == ' ') break;
        if (ch == '/') { rank--; file = 0; continue; }
        if (ch >= '1' && ch <= '8') { file += (ch - '0'); continue; }
        if (file < 8 && rank >= 0 && rank < 8) {
            out.grid[rank][file] = Board::makePieceFromFen(ch);
            file++;
        } else return false;
    }
    // side to move
    auto sp = fen.find(' ');
    if (sp != std::string::npos && sp+1 < fen.size()) {
        out.whiteToMove = (fen[sp+1] == 'w');
    } else {
        out.whiteToMove = true;
    }
    return true;
}
