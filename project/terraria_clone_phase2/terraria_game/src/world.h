#ifndef WORLD_H
#define WORLD_H

#include "raylib.h"
#include "tile.h"
#include "constants.h"

/*
    world.h

    Purpose:
    Owns the 2D grid of tiles that makes up the game world, and knows
    how to draw only the portion of that grid currently visible on
    screen. This module does NOT decide what tiles look like (that's
    tile.c) and does NOT know about the player or camera controls —
    it only needs a Camera2D to figure out what's visible when drawing.

    In Phase 2, World_Init fills the grid with a simple fixed layout
    (sky / grass / dirt / stone) just so we have real tile data to
    render. Phase 6 will replace World_Init's contents with proper
    procedural generation (caves, ores, trees) without needing to
    change how the world is stored or drawn.
*/

typedef struct World
{
    TileType tiles[WORLD_HEIGHT_TILES][WORLD_WIDTH_TILES];
} World;

/* Fills the world with a simple test layout: air above ground level,
   a grass surface row, a few rows of dirt, then stone underneath. */
void World_Init(World *world);

/* Returns the tile at (tileX, tileY). Returns TILE_AIR for any
   out-of-bounds coordinate instead of crashing, so callers don't need
   to bounds-check before every lookup. */
TileType World_GetTile(const World *world, int tileX, int tileY);

/* Sets the tile at (tileX, tileY). Does nothing if out of bounds. */
void World_SetTile(World *world, int tileX, int tileY, TileType type);

/* Draws every non-air tile that is currently visible through the
   given camera. Tiles outside the camera's view are skipped entirely,
   so performance does not depend on how large the world is. */
void World_Draw(const World *world, Camera2D camera);

#endif /* WORLD_H */
