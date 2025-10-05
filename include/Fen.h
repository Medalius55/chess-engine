#pragma once
#include <string>
#include "Board.h"

inline constexpr const char* STARTPOS_FEN =
    "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

bool LoadFEN(const std::string& fen, Board& out);