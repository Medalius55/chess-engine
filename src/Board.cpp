#include "Board.h"

Board::Board() {
    for (auto& r : squares) r.fill('.');
}