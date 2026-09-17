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

typedef struct TreeTilePosition
{
    int x;
    int y;
    TileType type;
} TreeTilePosition;

static bool IsTreeTile(TileType tileType)
{
    return tileType == TILE_WOOD || tileType == TILE_LEAVES;
}

static void AddTreeNeighbor(World *world, TreeTilePosition tilesToBreak[],
                            int *tileCount, int tileX, int tileY,
                            int minedTileY)
{
    TileType tileType = World_GetTile(world, tileX, tileY);
    if (!World_IsInBounds(tileX, tileY) || !IsTreeTile(tileType))
    {
        return;
    }

    /* Screen-space Y increases downward. Wood below the mined block has a
       larger Y coordinate, so leave it standing. Leaves are always included
       once the search reaches the canopy, including its hanging edges. */
    if (tileType == TILE_WOOD && tileY > minedTileY)
    {
        return;
    }

    /* Clearing a tile when it enters the list also marks it as visited.
       This prevents the same tile from being added more than once. */
    World_SetTile(world, tileX, tileY, TILE_AIR);
    tilesToBreak[*tileCount] = (TreeTilePosition){ tileX, tileY, tileType };
    (*tileCount)++;
}

static void BreakConnectedTree(World *world, int startTileX, int startTileY,
                               ParticleSystem *particleSystem,
                               DroppedItemSystem *droppedItemSystem)
{
    /* This queue can hold the entire world, so even a large player-built
       connected tree cannot overflow it. Natural trees use only a few slots. */
    TreeTilePosition tilesToBreak[WORLD_WIDTH_TILES * WORLD_HEIGHT_TILES];
    int tileCount = 0;
    int nextTile = 0;

    AddTreeNeighbor(world, tilesToBreak, &tileCount,
                    startTileX, startTileY, startTileY);

    while (nextTile < tileCount)
    {
        TreeTilePosition position = tilesToBreak[nextTile];
        nextTile++;

        AddTreeNeighbor(world, tilesToBreak, &tileCount,
                        position.x - 1, position.y, startTileY);
        AddTreeNeighbor(world, tilesToBreak, &tileCount,
                        position.x + 1, position.y, startTileY);
        AddTreeNeighbor(world, tilesToBreak, &tileCount,
                        position.x, position.y - 1, startTileY);
        AddTreeNeighbor(world, tilesToBreak, &tileCount,
                        position.x, position.y + 1, startTileY);
    }

    for (int i = 0; i < tileCount; i++)
    {
        TreeTilePosition position = tilesToBreak[i];
        Rectangle tileBounds = GetTileBounds(position.x, position.y);

        Vector2 tileCenter = {
            tileBounds.x + tileBounds.width / 2.0f,
            tileBounds.y + tileBounds.height / 2.0f
        };

        ParticleSystem_SpawnBlockBreak(particleSystem, tileBounds,
                                       Tile_GetColor(position.type));
        DroppedItemSystem_Spawn(droppedItemSystem,
                                Item_FromTileType(position.type), 1,
                                tileCenter);
    }
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

    if (tileType == TILE_WOOD)
    {
        BreakConnectedTree(world, interaction->targetTileX,
                           interaction->targetTileY, particleSystem,
                           droppedItemSystem);
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

static void ResetMining(Interaction *interaction)
{
    interaction->isMining = false;
    interaction->miningProgress = 0.0f;
}

static void UpdateMining(Interaction *interaction, World *world,
                         const Inventory *inventory,
                         ParticleSystem *particleSystem,
                         DroppedItemSystem *droppedItemSystem,
                         float deltaTime)
{
    TileType tileType = World_GetTile(world, interaction->targetTileX,
                                     interaction->targetTileY);
    if (tileType == TILE_AIR)
    {
        ResetMining(interaction);
        return;
    }

    bool targetChanged = !interaction->isMining ||
        interaction->miningTileX != interaction->targetTileX ||
        interaction->miningTileY != interaction->targetTileY;

    if (targetChanged)
    {
        interaction->miningTileX = interaction->targetTileX;
        interaction->miningTileY = interaction->targetTileY;
        interaction->miningProgress = 0.0f;
        interaction->isMining = true;
    }

    if (deltaTime > 0.05f)
    {
        deltaTime = 0.05f;
    }

    const ItemStack *selectedStack = Inventory_GetSelectedStack(inventory);
    float miningTime = Tile_GetMiningTime(tileType);
    float miningSpeed = Item_GetMiningSpeed(selectedStack->type, tileType);

    if (miningTime <= 0.0f)
    {
        ResetMining(interaction);
        return;
    }

    interaction->miningProgress += deltaTime * miningSpeed / miningTime;
    if (interaction->miningProgress >= 1.0f)
    {
        TryMineBlock(interaction, world, particleSystem, droppedItemSystem);
        ResetMining(interaction);
    }
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
    interaction->miningTileX = 0;
    interaction->miningTileY = 0;
    interaction->miningProgress = 0.0f;
    interaction->isMining = false;
}

void Interaction_Update(Interaction *interaction, World *world,
                        const Player *player, Camera2D camera,
                        InteractionInput input,
                        ParticleSystem *particleSystem,
                        DroppedItemSystem *droppedItemSystem,
                        Inventory *inventory, float deltaTime)
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
        ResetMining(interaction);
        return;
    }

    if (input.mineHeld)
    {
        UpdateMining(interaction, world, inventory, particleSystem,
                     droppedItemSystem, deltaTime);
    }
    else
    {
        ResetMining(interaction);
        if (input.placePressed)
        {
            TryPlaceBlock(interaction, world, player, inventory);
        }
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

    if (interaction->isMining)
    {
        const float barHeight = 3.0f;
        Rectangle barBackground = {
            tileBounds.x,
            tileBounds.y - barHeight - 2.0f,
            tileBounds.width,
            barHeight
        };
        Rectangle barFill = barBackground;
        barFill.width *= interaction->miningProgress;

        DrawRectangleRec(barBackground, (Color){ 25, 25, 30, 220 });
        DrawRectangleRec(barFill, GOLD);
        DrawRectangleLinesEx(barBackground, 0.5f, BLACK);
    }
}
