#include "Input.h"
#include "Board.h"
#include "Piece.h"

std::optional<Square> MouseToSquare(const Layout& L, Vector2 m) {
    int x = (int)m.x - L.boardX;
    int y = (int)m.y - L.boardY;
    if (x < 0 || y < 0) return std::nullopt;
    if (x >= 8 * L.tile || y >= 8 * L.tile) return std::nullopt;
    return Square{ x / L.tile, 7 - (y / L.tile) };
}

static inline bool isWhiteChar(char s){ return s>='A'&&s<='Z'; }
static inline bool isBlackChar(char s){ return s>='a'&&s<='z'; }

void HandleClick(Board& board, const Layout& L, Vector2 mouse) {
    auto sq = MouseToSquare(L, mouse);
    if (!sq) return;

    // If nothing is selected yet: select a friendly piece and compute its moves
    if (!board.selected) {
        char sym = board.charAt(sq->rank, sq->file);
        if (sym == '.') return;
        if (board.whiteToMove && !isWhiteChar(sym)) return;
        if (!board.whiteToMove && !isBlackChar(sym)) return;

        board.selected = *sq;

       // After: board.selected = *sq;
        board.legalTargets.clear();
        if (auto p = board.pieceAt(sq->rank, sq->file)) {
            auto moves = p->generateMoves(board, *sq);       // pseudo-legal
            for (auto& mv : moves) {
                if (board.wouldBeLegal(mv)) {                // filter by king safety
                    board.legalTargets.push_back(mv.to);
                }
            }
        }
        return;
    }

    // Clicking the same square: deselect and clear targets
    if (board.selected->file == sq->file && board.selected->rank == sq->rank) {
        board.selected.reset();
        board.legalTargets.clear();  
        return;
    }

    // Try to make a move from selected -> clicked
    Move m{ *board.selected, *sq, 0 };
    if (board.tryMakeMove(m)) {
        board.selected.reset();
        board.legalTargets.clear();
        return;
    }

    // If illegal: if clicked friendly piece, switch selection and recompute targets
    char sym = board.charAt(sq->rank, sq->file);
    if ((board.whiteToMove && isWhiteChar(sym)) || (!board.whiteToMove && isBlackChar(sym))) {
        board.selected = *sq;
        board.legalTargets.clear();
        if (auto p = board.pieceAt(sq->rank, sq->file)) {
            auto moves = p->generateMoves(board, *sq);
            for (auto& mv : moves) if (board.wouldBeLegal(mv)) board.legalTargets.push_back(mv.to);
        }
    }
}
