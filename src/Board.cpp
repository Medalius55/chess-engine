#include "Board.h"
#include "Pieces.h"
#include <cctype>

Board::Board() {
    for (auto& r : grid) for (auto& c : r) c.reset();
}

std::unique_ptr<Piece> Board::makePieceFromFen(char c) {
    bool white = std::isupper((unsigned char)c) != 0;
    char t = (char)std::tolower((unsigned char)c);

    switch (t) {
        case 'p': return std::make_unique<Pawn>(white);
        case 'n': return std::make_unique<Knight>(white);
        case 'b': return std::make_unique<Bishop>(white);
        case 'r': return std::make_unique<Rook>(white);
        case 'q': return std::make_unique<Queen>(white);
        case 'k': return std::make_unique<King>(white);
        default:  return nullptr;
    }
}

bool Board::tryMakeMove(const Move& m) {
    // bounds & piece presence
    if (m.from.file<0||m.from.file>7||m.from.rank<0||m.from.rank>7) return false;
    if (m.to.file<0||m.to.file>7||m.to.rank<0||m.to.rank>7) return false;

    Piece* src = pieceAt(m.from.rank, m.from.file);
    if (!src) return false;
    if (whiteToMove != src->isWhite()) return false;

    // generate pseudo-legal and see if target allowed
    auto moves = src->generateMoves(*this, m.from);
    bool ok=false; Move chosen{};
    for (auto& x : moves) {
        if (x.to.file==m.to.file && x.to.rank==m.to.rank) { ok=true; chosen=x; break; }
    }
    if (!ok) return false;

    // apply (capture handled by overwrite)
    grid[m.to.rank][m.to.file] = std::move(grid[m.from.rank][m.from.file]);

    // promotion (simple to queen if set)
    if (chosen.promotion) {
        grid[m.to.rank][m.to.file] = makePieceFromFen(chosen.promotion);
    }

    whiteToMove = !whiteToMove;
    return true;
}
