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

    World_Init asks the separate world_generation module to build the tile
    layout. Generation can change without changing how tiles are stored,
    queried, edited, or drawn here.
*/

typedef struct World
{
    TileType tiles[WORLD_HEIGHT_TILES][WORLD_WIDTH_TILES];
} World;

/* Builds the initial world using the procedural generator. */
void World_Init(World *world);

/* Returns true when a tile coordinate is inside the world array. */
bool World_IsInBounds(int tileX, int tileY);

/* Returns the tile at (tileX, tileY). Returns TILE_AIR for any
   out-of-bounds coordinate instead of crashing, so callers don't need
   to bounds-check before every lookup. */
TileType World_GetTile(const World *world, int tileX, int tileY);

/* Sets the tile at (tileX, tileY). Does nothing if out of bounds. */
void World_SetTile(World *world, int tileX, int tileY, TileType type);

/* Finds the grass surface in a column, used to choose a safe spawn height. */
int World_FindSurfaceY(const World *world, int tileX);

/* Draws every non-air tile that is currently visible through the
   given camera. Tiles outside the camera's view are skipped entirely,
   so performance does not depend on how large the world is. */
void World_Draw(const World *world, Camera2D camera);

#endif /* WORLD_H */
