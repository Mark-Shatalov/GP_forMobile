#ifndef INVENTORY_H
#define INVENTORY_H

#include <stdbool.h>
#include "input.h"
#include "item.h"
#include "constants.h"

/*
    inventory.h

    Purpose:
    Owns the ten-slot hotbar, stack quantities, and selected slot. Phase 5
    only needs a hotbar; a larger backpack can be added later without
    changing how ItemStack works.
*/

typedef struct ItemStack
{
    ItemType type;
    int quantity;
} ItemStack;

typedef struct Inventory
{
    ItemStack slots[HOTBAR_SLOT_COUNT];
    int selectedSlot;
} Inventory;

void Inventory_Init(Inventory *inventory);
void Inventory_UpdateSelection(Inventory *inventory, InventoryInput input);

/* Adds as much as possible and returns the quantity that did not fit. */
int Inventory_AddItem(Inventory *inventory, ItemType type, int quantity);

const ItemStack *Inventory_GetSelectedStack(const Inventory *inventory);

/* Removes one item from the selected stack. Returns false if it was empty. */
bool Inventory_RemoveOneSelected(Inventory *inventory);

/* Draws the hotbar in screen space, outside BeginMode2D. */
void Inventory_Draw(const Inventory *inventory);

#endif /* INVENTORY_H */
