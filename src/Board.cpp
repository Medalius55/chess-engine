#include "Board.h"
#include "Pieces.h"
#include "Attack.h"

#include <cctype>
#include <optional>
#include <string>
#include <cmath>   // std::abs

// --------- helpers ---------

static std::optional<Square> findKing(const Board& b, bool white) {
    char king = white ? 'K' : 'k';
    for (int r = 0; r < 8; ++r) {
        for (int f = 0; f < 8; ++f) {
            if (b.charAt(r, f) == king) return Square{f, r};
        }
    }
    return std::nullopt;
}

// Snapshot-based simulator (no std::function; explicit undo state).
struct SimState {
    Move m{};
    std::unique_ptr<Piece> savedTo{};
    std::unique_ptr<Piece> epCaptured{};
    std::optional<Square> savedEP{};
    bool savedWK{}, savedWQ{}, savedBK{}, savedBQ{};
    int rookFromF = -1, rookToF = -1, rookRank = -1;
    bool didPromotion = false;

    void undo(Board& b) {
        auto& fromCell = b.grid[m.from.rank][m.from.file];
        auto& toCell   = b.grid[m.to.rank][m.to.file];

        // move back moving piece
        fromCell = std::move(toCell);
        // restore original destination occupant
        toCell   = std::move(savedTo);

        // undo rook move if castling
        if (rookToF != -1) {
            b.grid[rookRank][rookFromF] = std::move(b.grid[rookRank][rookToF]);
        }

        // restore EP-captured pawn (if any)
        if (epCaptured) {
            int dir = (m.from.rank < m.to.rank) ? +1 : -1; // white moved up, black down
            int capRank = m.to.rank - dir;
            b.grid[capRank][m.to.file] = std::move(epCaptured);
        }

        // restore side-effects
        b.enPassantTarget = savedEP;
        b.canCastleWK = savedWK; b.canCastleWQ = savedWQ;
        b.canCastleBK = savedBK; b.canCastleBQ = savedBQ;
    }
};

// simulate a move; if applyPromotion==true, actually replace piece on promotion
static SimState simulate(Board& b, const Move& m, bool applyPromotion) {
    SimState st;
    st.m = m;

    auto& fromCell = b.grid[m.from.rank][m.from.file];
    auto& toCell   = b.grid[m.to.rank][m.to.file];

    // save global/state
    st.savedTo = std::move(toCell);
    st.savedEP = b.enPassantTarget;
    st.savedWK = b.canCastleWK; st.savedWQ = b.canCastleWQ;
    st.savedBK = b.canCastleBK; st.savedBQ = b.canCastleBQ;

    // clear EP by default
    b.enPassantTarget.reset();

    // moving side (for EP dir)
    char movingSym = b.charAt(m.from.rank, m.from.file);

    // move piece
    toCell = std::move(fromCell);

    // promotion
    if (applyPromotion && m.promotion) {
        st.didPromotion = true;
        toCell = Board::makePieceFromFen(m.promotion);
    }

    // en passant capture removes pawn behind the target
    if (m.isEnPassant) {
        int dir = (std::isupper((unsigned char)movingSym) ? +1 : -1);
        int capRank = m.to.rank - dir;
        st.epCaptured = std::move(b.grid[capRank][m.to.file]);
    }

    // castling rook move
    if (m.isCastleKing || m.isCastleQueen) {
        st.rookRank = m.from.rank;
        if (m.isCastleKing) { st.rookFromF = 7; st.rookToF = 5; }
        else                { st.rookFromF = 0; st.rookToF = 3; }
        b.grid[st.rookRank][st.rookToF] = std::move(b.grid[st.rookRank][st.rookFromF]);
    }

    // update castling rights when king/rooks move or their home squares are captured
    auto touchSquare = [&](int r,int f){
        char c = b.charAt(r,f);
        if (r==0 && f==4) { b.canCastleWK=false; b.canCastleWQ=false; } // white king moved/captured
        if (r==7 && f==4) { b.canCastleBK=false; b.canCastleBQ=false; } // black king moved/captured
        if (r==0 && f==0) b.canCastleWQ=false; // white a1 rook
        if (r==0 && f==7) b.canCastleWK=false; // white h1 rook
        if (r==7 && f==0) b.canCastleBQ=false; // black a8 rook
        if (r==7 && f==7) b.canCastleBK=false; // black h8 rook
    };
    touchSquare(m.from.rank, m.from.file);
    touchSquare(m.to.rank,   m.to.file);

    // set EP target on pawn double push
    if (std::tolower((unsigned char)movingSym) == 'p' && std::abs(m.to.rank - m.from.rank) == 2) {
        int mid = (m.to.rank + m.from.rank)/2;
        b.enPassantTarget = Square{ m.from.file, mid };
    }

    return st;
}

// --------- Board methods ---------

Board::Board() {
    for (auto& r : grid) for (auto& c : r) c.reset();
}

std::unique_ptr<Piece> Board::makePieceFromFen(char c) {
    bool white = std::isupper((unsigned char)c) != 0;
    switch (std::tolower((unsigned char)c)) {
        case 'p': return std::make_unique<Pawn>(white);
        case 'n': return std::make_unique<Knight>(white);
        case 'b': return std::make_unique<Bishop>(white);
        case 'r': return std::make_unique<Rook>(white);
        case 'q': return std::make_unique<Queen>(white);
        case 'k': return std::make_unique<King>(white);
        default:  return nullptr;
    }
}

// Returns true if making `m` would be legal (king not left in check).
bool Board::wouldBeLegal(const Move& m) const {
    Board& self = const_cast<Board&>(*this);

    Piece* src = self.pieceAt(m.from.rank, m.from.file);
    if (!src || (self.whiteToMove != src->isWhite())) return false;

    // Find the generated move that lands on the same destination (adopt its flags)
    const Move* chosen = nullptr;
    auto list = src->generateMoves(self, m.from);
    for (auto& cand : list) {
        if (cand.to.file == m.to.file && cand.to.rank == m.to.rank) {
            chosen = &cand;
            break;
        }
    }
    if (!chosen) return false;

    // Simulate using the *candidate* (has proper flags like EP/castle)
    auto sim = simulate(self, *chosen, /*applyPromotion*/false);

    bool illegal = false;

    // For castling: start, through, dest must not be attacked
    if (chosen->isCastleKing || chosen->isCastleQueen) {
        int dir = chosen->isCastleKing ? +1 : -1;
        Square s0 = chosen->from;
        Square s1{ chosen->from.file + dir, chosen->from.rank };
        Square s2 = chosen->to;
        if (IsSquareAttacked(self, s0, !self.whiteToMove) ||
            IsSquareAttacked(self, s1, !self.whiteToMove) ||
            IsSquareAttacked(self, s2, !self.whiteToMove)) {
            illegal = true;
        }
    }

    if (!illegal) {
        auto k = findKing(self, self.whiteToMove);
        illegal = (!k.has_value() || IsSquareAttacked(self, *k, !self.whiteToMove));
    }

    sim.undo(self);
    return !illegal;
}

bool Board::tryMakeMove(const Move& m) {
    Piece* src = pieceAt(m.from.rank, m.from.file);
    if (!src || (whiteToMove != src->isWhite())) return false;

    // Match by destination only; then adopt the candidate’s flags
    const Move* chosen = nullptr;
    auto list = src->generateMoves(*this, m.from);
    for (auto& cand : list) {
        if (cand.to.file == m.to.file && cand.to.rank == m.to.rank) {
            chosen = &cand;
            break;
        }
    }
    if (!chosen) return false;

    if (!wouldBeLegal(*chosen)) return false;

    auto sim = simulate(*this, *chosen, /*applyPromotion*/true);
    (void)sim;
    whiteToMove = !whiteToMove;
    return true;
}

// --------- status helpers (optional UI) ---------

bool Board::inCheck(bool white) const {
    char k = white ? 'K' : 'k';
    Square ks{-1,-1};
    for (int r=0;r<8;++r) for (int f=0;f<8;++f) if (charAt(r,f)==k) { ks={f,r}; goto found; }
found:
    if (ks.file<0) return false;
    return IsSquareAttacked(*this, ks, !white);
}

bool Board::anyLegalMoves() const {
    // Check mobility for the current side to move, without mutating state
    for (int r = 0; r < 8; ++r) {
        for (int f = 0; f < 8; ++f) {
            auto p = pieceAt(r, f);
            if (!p || (p->isWhite() != whiteToMove)) continue;

            auto moves = p->generateMoves(*this, Square{f, r});
            for (auto& m : moves) {
                if (wouldBeLegal(m)) return true; // wouldBeLegal is const-safe
            }
        }
    }
    return false;
}

std::string Board::statusString() const {
    bool side = whiteToMove;
    bool check = inCheck(side);
    bool any = anyLegalMoves();
    if (any) return side ? "White to move" : "Black to move";
    if (check) return side ? "Checkmate - Black wins" : "Checkmate - White wins";
    return "Stalemate";
}