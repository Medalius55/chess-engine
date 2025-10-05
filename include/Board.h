#pragma once
#include <array>
#include <memory>
#include <optional>
#include <vector>
#include "Piece.h"

class Piece; // fwd

using BoardGrid = std::array<std::array<std::unique_ptr<Piece>,8>,8>;

struct Board {
    BoardGrid grid{};
    std::optional<Square> selected{};
    bool whiteToMove = true;

    std::vector<Square> legalTargets;

    Board();

    // Access
    const Piece* pieceAt(int rank, int file) const { return grid[rank][file].get(); }
    Piece*       pieceAt(int rank, int file)       { return grid[rank][file].get(); }

    // Return printable symbol for renderer/sprites: uppercase white, lowercase black, or '.'
    char charAt(int rank, int file) const {
        auto p = pieceAt(rank, file);
        return p ? p->symbol() : '.';
    }

    // Move application (pseudo-legal gate only)
    bool tryMakeMove(const Move& m);

    // Factory to create a piece from FEN char
    static std::unique_ptr<Piece> makePieceFromFen(char c);
};
