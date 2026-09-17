#include "game.h"
#include "constants.h"
#include "input.h"
#include "raylib.h"

/*
    game.c

    Implementation of the Game module.

    Phase 7 scope:
    - Mine by holding the mouse button, with tile hardness and progress
    - Use a wooden pickaxe on stone and ore, or an axe on trees
    - Step automatically over one-tile obstacles while walking
*/

void Game_Init(Game *game)
{
    CameraController_Init(&game->cameraController);
    World_Init(&game->world);

    int spawnTileX = WORLD_WIDTH_TILES / 2;
    int surfaceTileY = World_FindSurfaceY(&game->world, spawnTileX);

    /* Center the player over a tile with their feet on generated grass. */
    Vector2 spawnPosition = {
        spawnTileX * TILE_SIZE + (TILE_SIZE - PLAYER_WIDTH) / 2.0f,
        surfaceTileY * TILE_SIZE - PLAYER_HEIGHT
    };
    Player_Init(&game->player, spawnPosition);
    Interaction_Init(&game->interaction);
    ParticleSystem_Init(&game->particleSystem);
    Inventory_Init(&game->inventory);
    DroppedItemSystem_Init(&game->droppedItemSystem);

    /* A small starter stack makes placement immediately testable. Mined
       blocks will stack into the same slot when the player collects them. */
    Inventory_AddItem(&game->inventory, ITEM_DIRT, 20);
    Inventory_AddItem(&game->inventory, ITEM_WOODEN_PICKAXE, 1);
    Inventory_AddItem(&game->inventory, ITEM_WOODEN_AXE, 1);

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
                       &game->inventory, deltaTime);
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
