#include <cstdio>
#include <cstdlib>
#include <string>
#include "Board.h"
#include "Fen.h"
#include "Perft.h"

// Run like:  ./perft_runner "startpos" 3
int main(int argc, char** argv) {
    std::string fen = "startpos";
    int depth = 3;

    if (argc >= 2) fen = argv[1];
    if (argc >= 3) depth = std::atoi(argv[2]);

    Board b;

    if (fen == "startpos") {
        // Standard initial position
        LoadFEN("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1", b);
    } else {
        if (!LoadFEN(fen, b)) {
            std::fprintf(stderr, "Failed to load FEN.\n");
            return 1;
        }
    }

    std::printf("Perft divide (depth %d):\n", depth);
    PrintPerftDivide(b, depth);
    return 0;
}
