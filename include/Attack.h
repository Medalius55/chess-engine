#pragma once
#include "Board.h"

// Returns true if 'sq' is attacked by the given side.
bool IsSquareAttacked(const Board& b, Square sq, bool byWhite);