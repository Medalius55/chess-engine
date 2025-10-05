#pragma once
#include <vector>

struct Square { int file{0}; int rank{0}; };
class Board;

struct Move {
    Square from{};
    Square to{};
    char promotion{0};

    bool isCastleKing  = false;
    bool isCastleQueen = false;
    bool isEnPassant   = false;
};

class Piece {
public:
    explicit Piece(bool white) : white_(white) {}
    virtual ~Piece() = default;

    bool isWhite() const { return white_; }
    virtual char symbol() const = 0; 
    virtual std::vector<Move> generateMoves(const Board& b, Square from) const = 0;

protected:
    bool white_;
};