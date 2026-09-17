#include "world_generation.h"
#include "constants.h"
#include "raylib.h"
#include <stdlib.h>

/*
    world_generation.c

    Generation happens in layers:
    1. A gently changing surface with grass, dirt, and stone.
    2. Random-walk cave tunnels carved out of the underground.
    3. Copper veins placed only inside remaining stone.
    4. Trees placed on grass with a clear area around the player spawn.

    Raylib seeds its random-number generator when the window opens, so every
    new game starts with a different world.
*/

static int ClampInt(int value, int minimum, int maximum)
{
    if (value < minimum) return minimum;
    if (value > maximum) return maximum;
    return value;
}

static void GenerateTerrainLayers(World *world, int surfaceHeights[])
{
    int surfaceY = GROUND_LEVEL_TILE;

    for (int x = 0; x < WORLD_WIDTH_TILES; x++)
    {
        /* Changing height only every few columns makes rolling hills rather
           than noisy one-tile spikes. */
        if (x > 0 && x % 3 == 0)
        {
            surfaceY += GetRandomValue(-1, 1);
            surfaceY = ClampInt(surfaceY,
                                SURFACE_MIN_TILE, SURFACE_MAX_TILE);
        }

        surfaceHeights[x] = surfaceY;
        int dirtDepth = GetRandomValue(MIN_DIRT_LAYER_THICKNESS,
                                       MAX_DIRT_LAYER_THICKNESS);

        for (int y = 0; y < WORLD_HEIGHT_TILES; y++)
        {
            if (y < surfaceY)
            {
                world->tiles[y][x] = TILE_AIR;
            }
            else if (y == surfaceY)
            {
                world->tiles[y][x] = TILE_GRASS;
            }
            else if (y <= surfaceY + dirtDepth)
            {
                world->tiles[y][x] = TILE_DIRT;
            }
            else
            {
                world->tiles[y][x] = TILE_STONE;
            }
        }
    }
}

static void CarveCircle(World *world, const int surfaceHeights[],
                        int centerX, int centerY, int radius)
{
    for (int offsetY = -radius; offsetY <= radius; offsetY++)
    {
        for (int offsetX = -radius; offsetX <= radius; offsetX++)
        {
            if (offsetX * offsetX + offsetY * offsetY > radius * radius)
            {
                continue;
            }

            int tileX = centerX + offsetX;
            int tileY = centerY + offsetY;
            if (!World_IsInBounds(tileX, tileY))
            {
                continue;
            }

            /* Preserve the surface and its first few supporting rows. */
            if (tileY <= surfaceHeights[tileX] + 3 ||
                tileY >= WORLD_HEIGHT_TILES - 1)
            {
                continue;
            }

            World_SetTile(world, tileX, tileY, TILE_AIR);
        }
    }
}

static void GenerateCaves(World *world, const int surfaceHeights[])
{
    for (int cave = 0; cave < CAVE_COUNT; cave++)
    {
        int caveX = GetRandomValue(4, WORLD_WIDTH_TILES - 5);
        int caveY = GetRandomValue(SURFACE_MAX_TILE + 8,
                                   WORLD_HEIGHT_TILES - 8);
        int direction = GetRandomValue(0, 1) == 0 ? -1 : 1;
        int caveLength = GetRandomValue(20, 55);

        for (int step = 0; step < caveLength; step++)
        {
            int radius = GetRandomValue(1, 3);
            CarveCircle(world, surfaceHeights, caveX, caveY, radius);

            caveX += direction + GetRandomValue(-1, 1);
            caveY += GetRandomValue(-1, 1);

            if (caveX < 3 || caveX > WORLD_WIDTH_TILES - 4)
            {
                direction = -direction;
                caveX = ClampInt(caveX, 3, WORLD_WIDTH_TILES - 4);
            }
            caveY = ClampInt(caveY, SURFACE_MAX_TILE + 5,
                             WORLD_HEIGHT_TILES - 4);
        }
    }
}

static void GenerateOreVeins(World *world)
{
    for (int vein = 0; vein < ORE_VEIN_COUNT; vein++)
    {
        int oreX = GetRandomValue(3, WORLD_WIDTH_TILES - 4);
        int oreY = GetRandomValue(SURFACE_MAX_TILE + 6,
                                  WORLD_HEIGHT_TILES - 5);
        int veinSize = GetRandomValue(4, 10);

        for (int block = 0; block < veinSize; block++)
        {
            if (World_GetTile(world, oreX, oreY) == TILE_STONE)
            {
                World_SetTile(world, oreX, oreY, TILE_COPPER_ORE);
            }

            /* A random walk makes an irregular connected cluster. */
            oreX += GetRandomValue(-1, 1);
            oreY += GetRandomValue(-1, 1);
            oreX = ClampInt(oreX, 1, WORLD_WIDTH_TILES - 2);
            oreY = ClampInt(oreY, SURFACE_MAX_TILE + 4,
                            WORLD_HEIGHT_TILES - 2);
        }
    }
}

static void PlaceTree(World *world, int tileX, int surfaceY)
{
    int trunkHeight = GetRandomValue(4, 7);
    int treeTopY = surfaceY - trunkHeight;

    /* Build the leaf canopy first so the trunk remains visible through its
       center. Leaves only replace air and never overwrite terrain. */
    for (int offsetY = -2; offsetY <= 1; offsetY++)
    {
        for (int offsetX = -2; offsetX <= 2; offsetX++)
        {
            if (abs(offsetX) + abs(offsetY) > 3)
            {
                continue;
            }

            int leafX = tileX + offsetX;
            int leafY = treeTopY + offsetY;
            if (World_GetTile(world, leafX, leafY) == TILE_AIR)
            {
                World_SetTile(world, leafX, leafY, TILE_LEAVES);
            }
        }
    }

    for (int y = surfaceY - 1; y >= treeTopY; y--)
    {
        World_SetTile(world, tileX, y, TILE_WOOD);
    }
}

static void GenerateTrees(World *world, const int surfaceHeights[])
{
    int spawnTileX = WORLD_WIDTH_TILES / 2;
    int treeX = GetRandomValue(4, 8);

    while (treeX < WORLD_WIDTH_TILES - 4)
    {
        /* Keep the initial player area open and easy to navigate. */
        if (abs(treeX - spawnTileX) >= 8)
        {
            int surfaceY = surfaceHeights[treeX];
            if (World_GetTile(world, treeX, surfaceY) == TILE_GRASS)
            {
                PlaceTree(world, treeX, surfaceY);
            }
        }

        treeX += GetRandomValue(7, 13);
    }
}

void WorldGeneration_Generate(World *world)
{
    int surfaceHeights[WORLD_WIDTH_TILES];

    GenerateTerrainLayers(world, surfaceHeights);
    GenerateCaves(world, surfaceHeights);
    GenerateOreVeins(world);
    GenerateTrees(world, surfaceHeights);
}
