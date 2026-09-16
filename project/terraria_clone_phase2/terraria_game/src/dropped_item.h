#ifndef DROPPED_ITEM_H
#define DROPPED_ITEM_H

#include <stdbool.h>
#include "inventory.h"
#include "item.h"
#include "player.h"
#include "world.h"

/*
    dropped_item.h

    Purpose:
    Manages collectible item entities that exist in the world after a block
    is mined. Items fall onto solid tiles and enter the hotbar when the player
    touches them and inventory space is available.
*/

typedef struct DroppedItem
{
    Vector2 position;
    Vector2 velocity;
    ItemType type;
    int quantity;
    float pickupDelay;
    bool isActive;
} DroppedItem;

typedef struct DroppedItemSystem
{
    DroppedItem items[MAX_DROPPED_ITEMS];
} DroppedItemSystem;

void DroppedItemSystem_Init(DroppedItemSystem *system);
void DroppedItemSystem_Spawn(DroppedItemSystem *system, ItemType type,
                             int quantity, Vector2 centerPosition);
void DroppedItemSystem_Update(DroppedItemSystem *system, const World *world,
                              const Player *player, Inventory *inventory,
                              float deltaTime);
void DroppedItemSystem_Draw(const DroppedItemSystem *system);

#endif /* DROPPED_ITEM_H */
