#include "game.h"
#include "constants.h"
#include "input.h"
#include "raylib.h"

/*
    game.c

    Implementation of the Game module.

    Phase 5 scope:
    - Store stackable items in a ten-slot hotbar
    - Drop collectible items when blocks are mined
    - Place and consume the selected block item
    - Change the selected slot with the wheel or number keys
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
    Inventory_Init(&game->inventory);
    DroppedItemSystem_Init(&game->droppedItemSystem);

    /* A small starter stack makes placement immediately testable. Mined
       blocks will stack into the same slot when the player collects them. */
    Inventory_AddItem(&game->inventory, ITEM_DIRT, 20);

    game->cameraController.camera.target = Player_GetCenter(&game->player);
}

void Game_Update(Game *game, float deltaTime)
{
    PlayerInput input = Input_GetPlayerInput();
    InteractionInput interactionInput = Input_GetInteractionInput();
    InventoryInput inventoryInput = Input_GetInventoryInput();

    Inventory_UpdateSelection(&game->inventory, inventoryInput);

    Player_Update(&game->player, &game->world, input, deltaTime);
    CameraController_Update(&game->cameraController,
                            Player_GetCenter(&game->player), deltaTime);
    Interaction_Update(&game->interaction, &game->world, &game->player,
                       game->cameraController.camera, interactionInput,
                       &game->particleSystem, &game->droppedItemSystem,
                       &game->inventory);
    ParticleSystem_Update(&game->particleSystem, deltaTime);
    DroppedItemSystem_Update(&game->droppedItemSystem, &game->world,
                             &game->player, &game->inventory, deltaTime);
}

void Game_Draw(Game *game)
{
    World_Draw(&game->world, game->cameraController.camera);
    DroppedItemSystem_Draw(&game->droppedItemSystem);
    ParticleSystem_Draw(&game->particleSystem);
    Player_Draw(&game->player);
    Interaction_Draw(&game->interaction);
}

void Game_DrawUI(const Game *game)
{
    Inventory_Draw(&game->inventory);
}

void Game_Shutdown(Game *game)
{
    (void)game; /* current game objects do not allocate dynamic memory */
}
