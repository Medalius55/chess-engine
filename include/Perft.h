#pragma once
#include <cstdint>
#include <vector>
#include <string>
#include "Board.h"

// A single legal move list from the current side to move.
std::vector<Move> GenerateAllLegalMoves(Board& b);

// Perft node counter. Counts the number of leaf nodes at `depth`.
uint64_t Perft(Board& b, int depth);

// Perft "divide": returns (move -> leaf count) for depth >= 1.
// Useful for pinpointing which move's subtree is wrong.
std::vector<std::pair<Move, uint64_t>> PerftDivide(Board& b, int depth);

// Pretty-printer helpers
std::string MoveToString(const Move& m);
void PrintPerftDivide(Board& b, int depth);