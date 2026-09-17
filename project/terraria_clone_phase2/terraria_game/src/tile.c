#include "tile.h"
#include "constants.h"

/*
    tile.c

    Implementation of per-tile-type properties. As we add more tile
    types later (ores, wood, etc.), these functions are the only
    places that need a new case added.
*/

bool Tile_IsSolid(TileType type)
{
    switch (type)
    {
        case TILE_AIR:
        case TILE_WOOD:
        case TILE_LEAVES:
            return false;

        case TILE_DIRT:
        case TILE_GRASS:
        case TILE_STONE:
        case TILE_COPPER_ORE:
            return true;

        default:
            return false;
    }
}

float Tile_GetMiningTime(TileType type)
{
    switch (type)
    {
        case TILE_DIRT:       return DIRT_MINING_TIME;
        case TILE_GRASS:      return GRASS_MINING_TIME;
        case TILE_STONE:      return STONE_MINING_TIME;
        case TILE_WOOD:       return WOOD_MINING_TIME;
        case TILE_LEAVES:     return LEAVES_MINING_TIME;
        case TILE_COPPER_ORE: return COPPER_MINING_TIME;
        case TILE_AIR:
        default:              return 0.0f;
    }
}

Color Tile_GetColor(TileType type)
{
    switch (type)
    {
        case TILE_DIRT:  return (Color){ 133, 94, 66, 255 };   /* brown */
        case TILE_GRASS: return (Color){ 86, 158, 62, 255 };   /* green */
        case TILE_STONE: return (Color){ 120, 120, 120, 255 }; /* gray */
        case TILE_WOOD:  return (Color){ 112, 74, 42, 255 };   /* dark brown */
        case TILE_LEAVES: return (Color){ 52, 125, 55, 255 };  /* dark green */
        case TILE_COPPER_ORE:
            return (Color){ 184, 105, 64, 255 };               /* copper */

        case TILE_AIR:
        default:
            return BLANK; /* air is never actually drawn, see World_Draw */
    }
}
