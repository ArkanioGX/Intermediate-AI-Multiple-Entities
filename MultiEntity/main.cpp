#include "raylib.h"
#include "Game.h"

int main() {
    const int screenWidth = 1000;
    const int screenHeight = 1000;
    SetConfigFlags(FLAG_MSAA_4X_HINT);
    InitWindow(screenWidth, screenHeight, "Boid Gamingz");
    int display = GetCurrentMonitor();
    SetWindowSize(screenWidth, screenHeight);
    //ToggleFullscreen();

    Game::instance().load();

    while (!WindowShouldClose()) {
        Game::instance().loop();
        Game::instance().draw();
        Game::instance().RemoveDeadActors();
    }
    Game::instance().close();
    CloseWindow();
    return 0;
}