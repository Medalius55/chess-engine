#include "Perft.h"
#include "Pieces.h"   // for symbols via Board::charAt if you want richer MoveToString
#include <cstdio>
#include <cctype>

// Generate all LEGAL moves for the current side to move
std::vector<Move> GenerateAllLegalMoves(Board& b) {
    std::vector<Move> out;
    out.reserve(64);
    for (int r = 0; r < 8; ++r) {
        for (int f = 0; f < 8; ++f) {
            auto p = b.pieceAt(r, f);
            if (!p) continue;
            if (p->isWhite() != b.whiteToMove) continue;

            auto plist = p->generateMoves(b, Square{f, r}); // pseudo-legal
            for (auto& mv : plist) {
                if (b.wouldBeLegal(mv)) out.push_back(mv);
            }
        }
    }
    return out;
}

uint64_t Perft(Board& b, int depth) {
    if (depth == 0) return 1;

    uint64_t nodes = 0;
    auto moves = GenerateAllLegalMoves(b);
    for (const auto& m : moves) {
        // apply
        if (!b.tryMakeMove(m)) continue; // should succeed after wouldBeLegal, but be safe
        nodes += Perft(b, depth - 1);
        // undo
        bool ok = b.undo();
        (void)ok;
    }
    return nodes;
}

std::vector<std::pair<Move, uint64_t>> PerftDivide(Board& b, int depth) {
    std::vector<std::pair<Move, uint64_t>> res;
    if (depth <= 0) return res;
    auto moves = GenerateAllLegalMoves(b);
    res.reserve(moves.size());
    for (const auto& m : moves) {
        if (!b.tryMakeMove(m)) continue;
        uint64_t sub = Perft(b, depth - 1);
        bool ok = b.undo(); (void)ok;
        res.emplace_back(m, sub);
    }
    return res;
}

// --- Pretty printing helpers ---

static inline char fileChar(int f){ return char('a' + f); }
static inline char rankChar(int r){ return char('1' + r); }

std::string MoveToString(const Move& m) {
    // Simple coordinate notation: e2e4, e1g1 (castle), e5d6 ep
    std::string s;
    s += fileChar(m.from.file);
    s += rankChar(m.from.rank);
    s += fileChar(m.to.file);
    s += rankChar(m.to.rank);
    if (m.promotion) { s += char(std::toupper((unsigned char)m.promotion)); }
    return s;
}

void PrintPerftDivide(Board& b, int depth) {
    auto div = PerftDivide(b, depth);
    uint64_t total = 0;
    for (auto& [m, cnt] : div) {
        total += cnt;
        std::printf("%s: %llu\n", MoveToString(m).c_str(),
                    static_cast<unsigned long long>(cnt));
    }
    std::printf("Total: %llu\n", static_cast<unsigned long long>(total));
}
