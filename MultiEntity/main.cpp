#include "raylib.h"
#include "Game.h"

int main() {
    const int screenWidth = 800;
    const int screenHeight = 800;
    InitWindow(screenWidth, screenHeight, "Boid Gamingz");
    SetTargetFPS(60);

    Game::instance().load();

    while (!WindowShouldClose()) {
        Game::instance().loop();
        Game::instance().draw();
    }
    Game::instance().close();
    CloseWindow();
    return 0;
}