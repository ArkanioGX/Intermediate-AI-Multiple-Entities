#include "raylib.h"
#include "Game.h"

int main() {
    const int screenWidth = 1000;
    const int screenHeight = 1000;
    InitWindow(screenWidth, screenHeight, "Boid Gamingz");
    int display = GetCurrentMonitor();
    SetWindowSize(GetMonitorWidth(display), GetMonitorHeight(display));
    ToggleFullscreen();

    Game::instance().load();

    while (!WindowShouldClose()) {
        Game::instance().loop();
        Game::instance().draw();
    }
    Game::instance().close();
    CloseWindow();
    return 0;
}