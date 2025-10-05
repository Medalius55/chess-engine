#include "Fen.h"
#include <cctype>
#include <sstream>

static int fileOf(char f){ return (f>='a'&&f<='h')? f-'a' : -1; }
static int rankOf(char r){ return (r>='1'&&r<='8')? r-'1' : -1; }

bool LoadFEN(const std::string& fen, Board& out) {
    for (auto& r : out.grid) for (auto& c : r) c.reset();

    std::istringstream ss(fen);
    std::string boardPart, stm, castle, ep;
    ss >> boardPart >> stm >> castle >> ep;

    int rank = 7, file = 0;
    for (char ch : boardPart) {
        if (ch == '/') { rank--; file = 0; continue; }
        if (ch >= '1' && ch <= '8') { file += (ch - '0'); continue; }
        if (file < 8 && rank >= 0 && rank < 8) {
            out.grid[rank][file] = Board::makePieceFromFen(ch);
            file++;
        } else return false;
    }

    out.whiteToMove = (stm == "w");

    // Castling rights
    out.canCastleWK = out.canCastleWQ = out.canCastleBK = out.canCastleBQ = false;
    if (castle != "-") {
        for (char c : castle) {
            if (c=='K') out.canCastleWK = true;
            if (c=='Q') out.canCastleWQ = true;
            if (c=='k') out.canCastleBK = true;
            if (c=='q') out.canCastleBQ = true;
        }
    }

    // En passant target
    out.enPassantTarget.reset();
    if (ep != "-" && ep.size()==2) {
        int f = fileOf(ep[0]);
        int r = rankOf(ep[1]);
        if (f>=0 && r>=0) out.enPassantTarget = Square{f,r};
    }

    return true;
}
