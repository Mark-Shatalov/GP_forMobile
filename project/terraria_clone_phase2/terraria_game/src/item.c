#include "item.h"
#include "constants.h"

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
        case ITEM_WOOD:  return "Wood";
        case ITEM_LEAVES: return "Leaves";
        case ITEM_COPPER_ORE: return "Copper Ore";
        case ITEM_WOODEN_PICKAXE: return "Wooden Pickaxe";
        case ITEM_WOODEN_AXE: return "Wooden Axe";
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
        case ITEM_WOOD:  return (Color){ 112, 74, 42, 255 };
        case ITEM_LEAVES: return (Color){ 52, 125, 55, 255 };
        case ITEM_COPPER_ORE: return (Color){ 184, 105, 64, 255 };
        case ITEM_WOODEN_PICKAXE: return (Color){ 196, 171, 121, 255 };
        case ITEM_WOODEN_AXE: return (Color){ 191, 132, 76, 255 };
        case ITEM_NONE:
        default:         return BLANK;
    }
}

int Item_GetMaxStack(ItemType type)
{
    if (type == ITEM_WOODEN_PICKAXE || type == ITEM_WOODEN_AXE)
    {
        return 1;
    }

    return ITEM_MAX_STACK;
}

float Item_GetMiningSpeed(ItemType itemType, TileType tileType)
{
    if (itemType == ITEM_WOODEN_PICKAXE &&
        (tileType == TILE_STONE || tileType == TILE_COPPER_ORE))
    {
        return WOODEN_PICKAXE_SPEED;
    }

    if (itemType == ITEM_WOODEN_AXE &&
        (tileType == TILE_WOOD || tileType == TILE_LEAVES))
    {
        return WOODEN_AXE_SPEED;
    }

    return 1.0f;
}

ItemType Item_FromTileType(TileType tileType)
{
    switch (tileType)
    {
        case TILE_DIRT:  return ITEM_DIRT;
        case TILE_GRASS: return ITEM_GRASS;
        case TILE_STONE: return ITEM_STONE;
        case TILE_WOOD:  return ITEM_WOOD;
        case TILE_LEAVES: return ITEM_LEAVES;
        case TILE_COPPER_ORE: return ITEM_COPPER_ORE;
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
        case ITEM_WOOD:  return TILE_WOOD;
        case ITEM_LEAVES: return TILE_LEAVES;
        case ITEM_COPPER_ORE: return TILE_COPPER_ORE;
        case ITEM_WOODEN_PICKAXE:
        case ITEM_WOODEN_AXE:
        case ITEM_NONE:
        default:         return TILE_AIR;
    }
}
