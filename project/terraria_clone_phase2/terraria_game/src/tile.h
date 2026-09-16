#ifndef TILE_H
#define TILE_H

#include "raylib.h"

/*
    tile.h

    Purpose:
    Defines what a "tile" is in this game. For now a tile is nothing
    more than a type (Air, Dirt, Grass, Stone), but keeping it in its
    own module means later phases can extend it (durability, light
    level, variants, etc.) without touching world.c's rendering logic.

    This module knows nothing about the World's 2D array or the
    Camera — it only knows about a single tile's properties.
*/

typedef enum TileType
{
    TILE_AIR = 0,   /* empty space - nothing is drawn or collided with */
    TILE_DIRT,
    TILE_GRASS,
    TILE_STONE,

    TILE_TYPE_COUNT /* not a real tile - used to know how many types exist */
} TileType;

/* Returns true if the tile blocks movement (used by collision in Phase 3). */
bool Tile_IsSolid(TileType type);

/* Returns a placeholder color for a tile type. This stands in for real
   textures until texture_manager is introduced in a later phase. */
Color Tile_GetColor(TileType type);

#endif /* TILE_H */
