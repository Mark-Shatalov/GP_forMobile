#include "interaction.h"
#include "constants.h"
#include "tile.h"
#include <math.h>

/*
    interaction.c

    Mouse coordinates begin in screen space. GetScreenToWorld2D converts
    them into the same world coordinates used by players and tiles. Dividing
    those coordinates by TILE_SIZE identifies the tile under the pointer.
*/

static Rectangle GetTileBounds(int tileX, int tileY)
{
    return (Rectangle){
        (float)(tileX * TILE_SIZE),
        (float)(tileY * TILE_SIZE),
        (float)TILE_SIZE,
        (float)TILE_SIZE
    };
}

static bool IsTileWithinReach(const Player *player, int tileX, int tileY)
{
    Vector2 playerCenter = Player_GetCenter(player);
    Vector2 tileCenter = {
        tileX * TILE_SIZE + TILE_SIZE / 2.0f,
        tileY * TILE_SIZE + TILE_SIZE / 2.0f
    };
    float distanceX = tileCenter.x - playerCenter.x;
    float distanceY = tileCenter.y - playerCenter.y;
    float distanceSquared = distanceX * distanceX + distanceY * distanceY;
    float reachDistance = BLOCK_REACH_TILES * TILE_SIZE;

    return distanceSquared <= reachDistance * reachDistance;
}

static bool HasSolidNeighbor(const World *world, int tileX, int tileY)
{
    return Tile_IsSolid(World_GetTile(world, tileX - 1, tileY)) ||
           Tile_IsSolid(World_GetTile(world, tileX + 1, tileY)) ||
           Tile_IsSolid(World_GetTile(world, tileX, tileY - 1)) ||
           Tile_IsSolid(World_GetTile(world, tileX, tileY + 1));
}

static bool TileOverlapsPlayer(const Player *player, int tileX, int tileY)
{
    Rectangle tileBounds = GetTileBounds(tileX, tileY);
    Rectangle playerBounds = {
        player->position.x,
        player->position.y,
        player->width,
        player->height
    };

    return CheckCollisionRecs(tileBounds, playerBounds);
}

static void TryMineBlock(Interaction *interaction, World *world,
                         ParticleSystem *particleSystem,
                         DroppedItemSystem *droppedItemSystem)
{
    TileType tileType = World_GetTile(world,
                                     interaction->targetTileX,
                                     interaction->targetTileY);
    if (tileType == TILE_AIR)
    {
        return;
    }

    Rectangle tileBounds = GetTileBounds(interaction->targetTileX,
                                         interaction->targetTileY);
    Color tileColor = Tile_GetColor(tileType);
    Vector2 tileCenter = {
        tileBounds.x + tileBounds.width / 2.0f,
        tileBounds.y + tileBounds.height / 2.0f
    };

    World_SetTile(world, interaction->targetTileX,
                  interaction->targetTileY, TILE_AIR);
    ParticleSystem_SpawnBlockBreak(particleSystem, tileBounds, tileColor);
    DroppedItemSystem_Spawn(droppedItemSystem,
                            Item_FromTileType(tileType), 1, tileCenter);
}

static void TryPlaceBlock(const Interaction *interaction, World *world,
                          const Player *player, Inventory *inventory)
{
    int tileX = interaction->targetTileX;
    int tileY = interaction->targetTileY;
    const ItemStack *selectedStack = Inventory_GetSelectedStack(inventory);
    TileType tileToPlace = Item_GetPlacedTile(selectedStack->type);

    if (selectedStack->quantity <= 0 || tileToPlace == TILE_AIR)
    {
        return;
    }
    if (World_GetTile(world, tileX, tileY) != TILE_AIR)
    {
        return;
    }
    if (!HasSolidNeighbor(world, tileX, tileY))
    {
        return;
    }
    if (TileOverlapsPlayer(player, tileX, tileY))
    {
        return;
    }

    World_SetTile(world, tileX, tileY, tileToPlace);
    Inventory_RemoveOneSelected(inventory);
}

void Interaction_Init(Interaction *interaction)
{
    interaction->targetTileX = 0;
    interaction->targetTileY = 0;
    interaction->targetIsInWorld = false;
    interaction->targetIsInReach = false;
}

void Interaction_Update(Interaction *interaction, World *world,
                        const Player *player, Camera2D camera,
                        InteractionInput input,
                        ParticleSystem *particleSystem,
                        DroppedItemSystem *droppedItemSystem,
                        Inventory *inventory)
{
    Vector2 mouseWorldPosition =
        GetScreenToWorld2D(input.mouseScreenPosition, camera);

    interaction->targetTileX =
        (int)floorf(mouseWorldPosition.x / TILE_SIZE);
    interaction->targetTileY =
        (int)floorf(mouseWorldPosition.y / TILE_SIZE);
    interaction->targetIsInWorld =
        World_IsInBounds(interaction->targetTileX,
                         interaction->targetTileY);
    interaction->targetIsInReach =
        interaction->targetIsInWorld &&
        IsTileWithinReach(player, interaction->targetTileX,
                          interaction->targetTileY);

    if (!interaction->targetIsInReach)
    {
        return;
    }

    if (input.minePressed)
    {
        TryMineBlock(interaction, world, particleSystem, droppedItemSystem);
    }
    else if (input.placePressed)
    {
        TryPlaceBlock(interaction, world, player, inventory);
    }
}

void Interaction_Draw(const Interaction *interaction)
{
    if (!interaction->targetIsInWorld)
    {
        return;
    }

    Rectangle tileBounds = GetTileBounds(interaction->targetTileX,
                                         interaction->targetTileY);
    Color outlineColor = interaction->targetIsInReach ? YELLOW : RED;

    DrawRectangleLinesEx(tileBounds, 1.0f, outlineColor);
}
