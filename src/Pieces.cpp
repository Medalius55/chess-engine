#include "Pieces.h"
#include "Board.h"
#include <cctype>

static inline bool inb(int f, int r) { return f >= 0 && f < 8 && r >= 0 && r < 8; }
static inline bool sameSide(char a, char b) {
    if (a == '.' || b == '.') return false;
    return (std::isupper((unsigned char)a) && std::isupper((unsigned char)b)) ||
            (std::islower((unsigned char)a) && std::islower((unsigned char)b));
}

char Pawn::symbol() const   { return white_ ? 'P' : 'p'; }
char Knight::symbol() const { return white_ ? 'N' : 'n'; }
char Bishop::symbol() const { return white_ ? 'B' : 'b'; }
char Rook::symbol() const   { return white_ ? 'R' : 'r'; }
char Queen::symbol() const  { return white_ ? 'Q' : 'q'; }
char King::symbol() const   { return white_ ? 'K' : 'k'; }

std::vector<Move> Pawn::generateMoves(const Board& b, Square from) const {
    std::vector<Move> out;
    int dir = white_ ? +1 : -1;
    int startRank = white_ ? 1 : 6;
    int promoRank = white_ ? 7 : 0;
    int f = from.file, r = from.rank;

    // single
    int r1 = r + dir;
    if (inb(f,r1) && b.charAt(r1,f) == '.') {
        Move m{from, {f,r1}, 0};
        if (r1 == promoRank) m.promotion = white_ ? 'Q' : 'q';
        out.push_back(m);
        // double
        if (r == startRank) {
            int r2 = r + 2*dir;
            if (inb(f,r2) && b.charAt(r2,f) == '.') {
                out.push_back(Move{from, {f,r2}, 0});
            }
        }
    }
    // captures
    for (int df : {-1,+1}) {
        int nf=f+df, nr=r+dir;
        if (!inb(nf,nr)) continue;
        char dst = b.charAt(nr,nf);
        if (dst!='.' && !sameSide(symbol(), dst)) {
            Move m{from, {nf,nr}, 0};
            if (nr == promoRank) m.promotion = white_ ? 'Q' : 'q';
            out.push_back(m);
        }
    }
    return out;
}

std::vector<Move> Knight::generateMoves(const Board& b, Square from) const {
    std::vector<Move> out;
    static const int K[8][2]={{+1,+2},{+2,+1},{+2,-1},{+1,-2},{-1,-2},{-2,-1},{-2,+1},{-1,+2}};
    for (auto &d:K){
        int nf=from.file+d[0], nr=from.rank+d[1];
        if (!inb(nf,nr)) continue;
        char dst = b.charAt(nr,nf);
        if (dst=='.' || !sameSide(symbol(), dst)) out.push_back(Move{from,{nf,nr},0});
    }
    return out;
}

static void slide(const Board& b, const Piece& pc, Square from, int df,int dr, std::vector<Move>& out){
    int f=from.file+df, r=from.rank+dr;
    while (inb(f,r)) {
        char dst = b.charAt(r,f);
        if (dst=='.') { out.push_back(Move{from,{f,r},0}); }
        else { if (!sameSide(pc.symbol(), dst)) out.push_back(Move{from,{f,r},0}); break; }
        f+=df; r+=dr;
    }
}

std::vector<Move> Bishop::generateMoves(const Board& b, Square from) const {
    std::vector<Move> out;
    slide(b,*this,from,+1,+1,out);
    slide(b,*this,from,-1,+1,out);
    slide(b,*this,from,+1,-1,out);
    slide(b,*this,from,-1,-1,out);
    return out;
}
std::vector<Move> Rook::generateMoves(const Board& b, Square from) const {
    std::vector<Move> out;
    slide(b,*this,from,+1,0,out);
    slide(b,*this,from,-1,0,out);
    slide(b,*this,from,0,+1,out);
    slide(b,*this,from,0,-1,out);
    return out;
}
std::vector<Move> Queen::generateMoves(const Board& b, Square from) const {
    std::vector<Move> out;
    slide(b,*this,from,+1,0,out);  slide(b,*this,from,-1,0,out);
    slide(b,*this,from,0,+1,out);  slide(b,*this,from,0,-1,out);
    slide(b,*this,from,+1,+1,out); slide(b,*this,from,-1,+1,out);
    slide(b,*this,from,+1,-1,out); slide(b,*this,from,-1,-1,out);
    return out;
}
std::vector<Move> King::generateMoves(const Board& b, Square from) const {
    std::vector<Move> out;
    for (int dr=-1; dr<=1; ++dr) for (int df=-1; df<=1; ++df){
        if (!df && !dr) continue;
        int nf=from.file+df, nr=from.rank+dr;
        if (!inb(nf,nr)) continue;
        char dst = b.charAt(nr,nf);
        if (dst=='.' || !sameSide(symbol(), dst)) out.push_back(Move{from,{nf,nr},0});
    }
    return out;
}
