#ifndef ITEM_H
#define ITEM_H

#include "raylib.h"
#include "tile.h"

/*
    item.h

    Purpose:
    Defines the item types that can be stored in an inventory or dropped in
    the world. It also owns the conversion between placeable block items and
    world tiles.
*/

typedef enum ItemType
{
    ITEM_NONE = 0,
    ITEM_DIRT,
    ITEM_GRASS,
    ITEM_STONE,
    ITEM_WOOD,
    ITEM_LEAVES,
    ITEM_COPPER_ORE,
    ITEM_WOODEN_PICKAXE,
    ITEM_WOODEN_AXE,

    ITEM_TYPE_COUNT
} ItemType;

const char *Item_GetName(ItemType type);
Color Item_GetColor(ItemType type);

/* Tools stack to one; ordinary blocks use the shared maximum stack size. */
int Item_GetMaxStack(ItemType type);

/* Returns how much faster the selected item mines this tile. */
float Item_GetMiningSpeed(ItemType itemType, TileType tileType);

/* Converts a mined tile into the item it drops. */
ItemType Item_FromTileType(TileType tileType);

/* Converts a placeable item into a tile. ITEM_NONE returns TILE_AIR. */
TileType Item_GetPlacedTile(ItemType itemType);

#endif /* ITEM_H */
