#include "tile.h"

/*
    tile.c

    Implementation of per-tile-type properties. As we add more tile
    types later (ores, wood, etc.), these two functions are the only
    places that need a new case added.
*/

bool Tile_IsSolid(TileType type)
{
    switch (type)
    {
        case TILE_AIR:
            return false;

        case TILE_DIRT:
        case TILE_GRASS:
        case TILE_STONE:
            return true;

        default:
            return false;
    }
}

Color Tile_GetColor(TileType type)
{
    switch (type)
    {
        case TILE_DIRT:  return (Color){ 133, 94, 66, 255 };   /* brown */
        case TILE_GRASS: return (Color){ 86, 158, 62, 255 };   /* green */
        case TILE_STONE: return (Color){ 120, 120, 120, 255 }; /* gray */

        case TILE_AIR:
        default:
            return BLANK; /* air is never actually drawn, see World_Draw */
    }
}
