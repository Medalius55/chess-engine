#include "Input.h"

std::optional<Square> MouseToSquare(const Layout& L, Vector2 m) {
    int x = static_cast<int>(m.x) - L.boardX;
    int y = static_cast<int>(m.y) - L.boardY;

    if (x < 0 || y < 0) return std::nullopt;
    if (x >= 8 * L.tile || y >= 8 * L.tile) return std::nullopt;

    return Square{ x / L.tile, 7 - (y / L.tile) };
}

void HandleClick(Board& board, const Layout& L, Vector2 mouse) {
    auto sq = MouseToSquare(L, mouse);
    if (!sq) return;
    if (board.selected && board.selected->file == sq->file 
        && board.selected->rank == sq->rank) {
        board.selected.reset();
    } else {
        board.selected = *sq;
    }
}