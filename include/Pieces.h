#pragma once
#include "Piece.h"

class Pawn   : public Piece { public: using Piece::Piece; char symbol() const override; std::vector<Move> generateMoves(const Board&, Square) const override; };
class Knight : public Piece { public: using Piece::Piece; char symbol() const override; std::vector<Move> generateMoves(const Board&, Square) const override; };
class Bishop : public Piece { public: using Piece::Piece; char symbol() const override; std::vector<Move> generateMoves(const Board&, Square) const override; };
class Rook   : public Piece { public: using Piece::Piece; char symbol() const override; std::vector<Move> generateMoves(const Board&, Square) const override; };
class Queen  : public Piece { public: using Piece::Piece; char symbol() const override; std::vector<Move> generateMoves(const Board&, Square) const override; };
class King   : public Piece { public: using Piece::Piece; char symbol() const override; std::vector<Move> generateMoves(const Board&, Square) const override; };
