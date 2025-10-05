#include "Attack.h"
#include <cctype>

static inline bool inb(int f,int r){ return f>=0&&f<8&&r>=0&&r<8; }
static inline bool isW(char c){ return c>='A'&&c<='Z'; }
static inline bool isB(char c){ return c>='a'&&c<='z'; }

static bool ray(const Board& b, int f,int r, int df,int dr, bool byWhite,
                const char* hitters) {
    f+=df; r+=dr;
    while (inb(f,r)) {
        char c = b.charAt(r,f);
        if (c=='.'){ f+=df; r+=dr; continue; }
        if ((byWhite && isW(c)) || (!byWhite && isB(c))) {
            // Is it one of the sliding attackers we’re testing for?
            for (const char* p=hitters; *p; ++p) { if (std::tolower(*p)==std::tolower(c)) return true; }
        }
        break;
    }
    return false;
}

bool IsSquareAttacked(const Board& b, Square sq, bool byWhite) {
    int f=sq.file, r=sq.rank;

    // Pawns
    int dir = byWhite ? +1 : -1;
    for (int df : {-1,+1}) {
        int nf=f+df, nr=r-dir; // attackers sit one rank behind target
        if (inb(nf,nr)) {
            char c = b.charAt(nr,nf);
            if (byWhite ? (c=='P') : (c=='p')) return true;
        }
    }

    // Knights
    static const int K[8][2]={{+1,+2},{+2,+1},{+2,-1},{+1,-2},{-1,-2},{-2,-1},{-2,+1},{-1,+2}};
    for (auto& d:K){
        int nf=f+d[0], nr=r+d[1];
        if (!inb(nf,nr)) continue;
        char c=b.charAt(nr,nf);
        if (byWhite ? (c=='N') : (c=='n')) return true;
    }

    // Bishops/Queens (diagonals)
    if (ray(b,f,r,+1,+1,byWhite,"BQ") ||
        ray(b,f,r,-1,+1,byWhite,"BQ") ||
        ray(b,f,r,+1,-1,byWhite,"BQ") ||
        ray(b,f,r,-1,-1,byWhite,"BQ")) return true;

    // Rooks/Queens (orthogonals)
    if (ray(b,f,r,+1,0,byWhite,"RQ") ||
        ray(b,f,r,-1,0,byWhite,"RQ") ||
        ray(b,f,r,0,+1,byWhite,"RQ") ||
        ray(b,f,r,0,-1,byWhite,"RQ")) return true;

    // King (adjacent)
    for (int dr=-1; dr<=1; ++dr) for (int df=-1; df<=1; ++df){
        if (!df && !dr) continue;
        int nf=f+df, nr=r+dr;
        if (!inb(nf,nr)) continue;
        char c=b.charAt(nr,nf);
        if (byWhite ? (c=='K') : (c=='k')) return true;
    }
    return false;
}
