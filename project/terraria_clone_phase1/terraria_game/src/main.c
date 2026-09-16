#include "raylib.h"
#include "constants.h"
#include "game.h"

/*
    main.c

    Purpose:
    This file only does three things:
    1. Open the window (Raylib setup)
    2. Run the game loop, calling into game.c every frame
    3. Close the window when the loop ends

    No game logic lives here. If you find yourself wanting to add
    "real" behavior to this file, it probably belongs in game.c or a
    more specific module instead.
*/

int main(void)
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, WINDOW_TITLE);
    SetTargetFPS(TARGET_FPS);

    Game game;
    Game_Init(&game);

    while (!WindowShouldClose())
    {
        float deltaTime = GetFrameTime();

        /* ---- Update ---- */
        Game_Update(&game, deltaTime);

        /* ---- Draw ---- */
        BeginDrawing();
            ClearBackground((Color){ 30, 30, 40, 255 });

            BeginMode2D(game.cameraController.camera);
                Game_Draw(&game);
            EndMode2D();

            /* Debug info drawn in screen space (outside the camera) */
            DrawFPS(10, 10);
            DrawText("Phase 1: Engine + Camera2D + Test Grid", 10, 35, 20, RAYWHITE);
            DrawText("Move: WASD / Arrow Keys   Zoom: Mouse Wheel", 10, 60, 18, LIGHTGRAY);
        EndDrawing();
    }

    Game_Shutdown(&game);
    CloseWindow();

    return 0;
}
