#include "game.h"
#include "constants.h"
#include "input.h"
#include "raylib.h"

/*
    game.c

    Implementation of the Game module.

    Phase 4 scope:
    - Keep the Phase 3 player and camera behavior
    - Target nearby tiles with the mouse
    - Mine and place blocks
    - Update and draw block-break particles
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
    Interaction_Init(&game->interaction);
    ParticleSystem_Init(&game->particleSystem);

    game->cameraController.camera.target = Player_GetCenter(&game->player);
}

void Game_Update(Game *game, float deltaTime)
{
    PlayerInput input = Input_GetPlayerInput();
    InteractionInput interactionInput = Input_GetInteractionInput();

    Player_Update(&game->player, &game->world, input, deltaTime);
    CameraController_Update(&game->cameraController,
                            Player_GetCenter(&game->player), deltaTime);
    Interaction_Update(&game->interaction, &game->world, &game->player,
                       game->cameraController.camera, interactionInput,
                       &game->particleSystem);
    ParticleSystem_Update(&game->particleSystem, deltaTime);
}

void Game_Draw(Game *game)
{
    World_Draw(&game->world, game->cameraController.camera);
    ParticleSystem_Draw(&game->particleSystem);
    Player_Draw(&game->player);
    Interaction_Draw(&game->interaction);
}

void Game_Shutdown(Game *game)
{
    (void)game; /* current game objects do not allocate dynamic memory */
}
