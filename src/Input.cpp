#include "Input.h"
#include "Board.h"
#include "Piece.h"
#include <raylib.h>

std::optional<Square> MouseToSquare(const Layout& L, Vector2 m) {
    int x = (int)m.x - L.boardX;
    int y = (int)m.y - L.boardY;
    if (x < 0 || y < 0) return std::nullopt;
    if (x >= 8 * L.tile || y >= 8 * L.tile) return std::nullopt;
    return Square{ x / L.tile, 7 - (y / L.tile) };
}

static inline bool isWhiteChar(char s){ return s>='A'&&s<='Z'; }
static inline bool isBlackChar(char s){ return s>='a'&&s<='z'; }

static char promotionKey(bool white) {
    if (IsKeyDown(KEY_Q)) return white ? 'Q' : 'q';
    if (IsKeyDown(KEY_R)) return white ? 'R' : 'r';
    if (IsKeyDown(KEY_B)) return white ? 'B' : 'b';
    if (IsKeyDown(KEY_N)) return white ? 'N' : 'n';
    return 0; 
}

void HandleClick(Board& board, const Layout& L, Vector2 mouse) {
    auto sq = MouseToSquare(L, mouse);
    if (!sq) return;

    if (!board.selected) {
        char sym = board.charAt(sq->rank, sq->file);
        if (sym == '.') return;
        if (board.whiteToMove && !isWhiteChar(sym)) return;
        if (!board.whiteToMove && !isBlackChar(sym)) return;

        board.selected = *sq;

        // compute legal targets 
        board.legalTargets.clear();
        if (auto p = board.pieceAt(sq->rank, sq->file)) {
            auto moves = p->generateMoves(board, *sq);
            for (auto& mv : moves) if (board.wouldBeLegal(mv)) board.legalTargets.push_back(mv.to);
        }
        return;
    }

    if (board.selected->file == sq->file && board.selected->rank == sq->rank) {
        board.selected.reset();
        board.legalTargets.clear();
        return;
    }

    // Build move; inject promotion if user is holding a key
    Move m{ *board.selected, *sq, 0 };
    char src = board.charAt(m.from.rank, m.from.file);
    m.promotion = promotionKey(std::isupper((unsigned char)src) != 0);

    if (board.tryMakeMove(m)) {
        board.selected.reset();
        board.legalTargets.clear();
    } else {
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
}
