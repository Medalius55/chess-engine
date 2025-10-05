#include "App.h"
#include "Fen.h"

int main() {
    App app{ Config{} };
    app.init();

    Board board;
    LoadFEN(STARTPOS_FEN, board);

    while (app.running()) {
        app.pollInput(board);
        app.beginFrame();
        app.render(board);
        app.endFrame();
    }

    app.shutdown();
    return 0;
}