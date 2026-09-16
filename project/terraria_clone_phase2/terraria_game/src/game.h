#ifndef GAME_H
#define GAME_H

#include "camera_controller.h"
#include "player.h"
#include "world.h"

/*
    game.h

    Purpose:
    This is the top-level module that ties every other system together.
    main.c should stay tiny — it just creates the window and calls into
    Game_Init / Game_Update / Game_Draw / Game_Shutdown every frame.

    Phase 3 adds a Player alongside the World and CameraController.
    Later phases can add inventory and interaction systems the same way.
*/

typedef struct Game
{
    CameraController cameraController;
    World world;
    Player player;
} Game;

/* Called once at startup. Sets up all subsystems. */
void Game_Init(Game *game);

/* Called once per frame, before drawing. Updates game logic. */
void Game_Update(Game *game, float deltaTime);

/* Called once per frame, inside Raylib's BeginDrawing/EndDrawing block.
   Draws everything that exists in world space (inside the camera). */
void Game_Draw(Game *game);

/* Called once when the program is closing. Frees any resources. */
void Game_Shutdown(Game *game);

#endif /* GAME_H */
