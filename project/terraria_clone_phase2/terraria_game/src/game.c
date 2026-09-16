#include "game.h"
#include "constants.h"
#include "input.h"
#include "raylib.h"

/*
    game.c

    Implementation of the Game module.

    Phase 3 scope:
    - Read movement and jump input
    - Update player physics and tile collision
    - Make the camera follow the player
    - Draw the world and player
*/

void Game_Init(Game *game)
{
    CameraController_Init(&game->cameraController);
    World_Init(&game->world);

    /* Spawn near the world's middle with the player's feet one tile
       above the surface. Gravity will place the player on the grass. */
    Vector2 spawnPosition = {
        (WORLD_WIDTH_TILES * TILE_SIZE) / 2.0f - PLAYER_WIDTH / 2.0f,
        (GROUND_LEVEL_TILE - 1) * TILE_SIZE - PLAYER_HEIGHT
    };
    Player_Init(&game->player, spawnPosition);

    game->cameraController.camera.target = Player_GetCenter(&game->player);
}

void Game_Update(Game *game, float deltaTime)
{
    PlayerInput input = Input_GetPlayerInput();

    Player_Update(&game->player, &game->world, input, deltaTime);
    CameraController_Update(&game->cameraController,
                            Player_GetCenter(&game->player), deltaTime);
}

void Game_Draw(Game *game)
{
    World_Draw(&game->world, game->cameraController.camera);
    Player_Draw(&game->player);
}

void Game_Shutdown(Game *game)
{
    (void)game; /* current game objects do not allocate dynamic memory */
}
