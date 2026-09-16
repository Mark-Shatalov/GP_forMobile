#include "item.h"

/*
    item.c

    Item properties live here so the inventory, dropped-item, and interaction
    modules do not each need their own copy of the same switch statements.
*/

const char *Item_GetName(ItemType type)
{
    switch (type)
    {
        case ITEM_DIRT:  return "Dirt";
        case ITEM_GRASS: return "Grass";
        case ITEM_STONE: return "Stone";
        case ITEM_NONE:
        default:         return "Empty";
    }
}

Color Item_GetColor(ItemType type)
{
    switch (type)
    {
        case ITEM_DIRT:  return (Color){ 133, 94, 66, 255 };
        case ITEM_GRASS: return (Color){ 86, 158, 62, 255 };
        case ITEM_STONE: return (Color){ 120, 120, 120, 255 };
        case ITEM_NONE:
        default:         return BLANK;
    }
}

ItemType Item_FromTileType(TileType tileType)
{
    switch (tileType)
    {
        case TILE_DIRT:  return ITEM_DIRT;
        case TILE_GRASS: return ITEM_GRASS;
        case TILE_STONE: return ITEM_STONE;
        case TILE_AIR:
        default:         return ITEM_NONE;
    }
}

TileType Item_GetPlacedTile(ItemType itemType)
{
    switch (itemType)
    {
        case ITEM_DIRT:  return TILE_DIRT;
        case ITEM_GRASS: return TILE_GRASS;
        case ITEM_STONE: return TILE_STONE;
        case ITEM_NONE:
        default:         return TILE_AIR;
    }
}
