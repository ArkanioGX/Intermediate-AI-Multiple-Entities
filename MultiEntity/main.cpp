#include "raylib.h"
#include "Game.h"

int main() {
    const int screenWidth = 1920;
    const int screenHeight = 1080;
    SetConfigFlags(FLAG_MSAA_4X_HINT);
    InitWindow(screenWidth, screenHeight, "Boid Gamingz");
    int display = GetCurrentMonitor();
    SetWindowSize(screenWidth, screenHeight);
    //SetTargetFPS(60);
    ToggleFullscreen();

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