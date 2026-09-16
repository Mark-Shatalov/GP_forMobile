#include "world.h"
#include "world_generation.h"
#include <math.h>

/*
    world.c

    Implementation of the World module described in world.h.
*/

bool World_IsInBounds(int tileX, int tileY)
{
    return tileX >= 0 && tileX < WORLD_WIDTH_TILES &&
           tileY >= 0 && tileY < WORLD_HEIGHT_TILES;
}

void World_Init(World *world)
{
    WorldGeneration_Generate(world);
}

TileType World_GetTile(const World *world, int tileX, int tileY)
{
    if (!World_IsInBounds(tileX, tileY))
    {
        return TILE_AIR;
    }

    return world->tiles[tileY][tileX];
}

void World_SetTile(World *world, int tileX, int tileY, TileType type)
{
    if (!World_IsInBounds(tileX, tileY))
    {
        return;
    }

    world->tiles[tileY][tileX] = type;
}

int World_FindSurfaceY(const World *world, int tileX)
{
    if (tileX < 0 || tileX >= WORLD_WIDTH_TILES)
    {
        return GROUND_LEVEL_TILE;
    }

    for (int tileY = 0; tileY < WORLD_HEIGHT_TILES; tileY++)
    {
        if (World_GetTile(world, tileX, tileY) == TILE_GRASS)
        {
            return tileY;
        }
    }

    return GROUND_LEVEL_TILE;
}

void World_Draw(const World *world, Camera2D camera)
{
    /* Figure out which part of the world is actually visible by
       converting the four corners of the screen into world-space
       coordinates, then converting those into tile indices. This is
       the key trick that lets us have a huge world but only ever
       draw a screen's worth of tiles. */
    Vector2 topLeftWorld     = GetScreenToWorld2D((Vector2){ 0, 0 }, camera);
    Vector2 bottomRightWorld = GetScreenToWorld2D((Vector2){ SCREEN_WIDTH, SCREEN_HEIGHT }, camera);

    int firstCol = (int)floorf(topLeftWorld.x / TILE_SIZE) - 1;
    int lastCol  = (int)ceilf(bottomRightWorld.x / TILE_SIZE) + 1;
    int firstRow = (int)floorf(topLeftWorld.y / TILE_SIZE) - 1;
    int lastRow  = (int)ceilf(bottomRightWorld.y / TILE_SIZE) + 1;

    /* Clamp the range to the actual world size so we never read or
       draw outside the array. */
    if (firstCol < 0) firstCol = 0;
    if (firstRow < 0) firstRow = 0;
    if (lastCol >= WORLD_WIDTH_TILES)  lastCol = WORLD_WIDTH_TILES - 1;
    if (lastRow >= WORLD_HEIGHT_TILES) lastRow = WORLD_HEIGHT_TILES - 1;

    for (int y = firstRow; y <= lastRow; y++)
    {
        for (int x = firstCol; x <= lastCol; x++)
        {
            TileType type = world->tiles[y][x];

            if (type == TILE_AIR)
            {
                continue; /* nothing to draw for empty space */
            }

            Rectangle tileRect = {
                (float)(x * TILE_SIZE),
                (float)(y * TILE_SIZE),
                (float)TILE_SIZE,
                (float)TILE_SIZE
            };

            DrawRectangleRec(tileRect, Tile_GetColor(type));

            /* Until textures are added, subtle borders make movement across
               otherwise identical neighboring tiles visible. */
            DrawRectangleLinesEx(tileRect, 0.5f,
                                 (Color){ 25, 25, 30, 70 });
        }
    }

    /* Draw the world's outer boundary so it's easy to see where the
       world ends while testing the camera. */
    Rectangle worldBounds = {
        0, 0,
        (float)(WORLD_WIDTH_TILES * TILE_SIZE),
        (float)(WORLD_HEIGHT_TILES * TILE_SIZE)
    };
    DrawRectangleLinesEx(worldBounds, 2.0f, RED);
}
