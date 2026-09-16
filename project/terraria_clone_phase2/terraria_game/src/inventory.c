#include "inventory.h"
#include "raylib.h"

/*
    inventory.c

    Adding items uses two passes: existing matching stacks are filled first,
    then empty slots are used. This prevents one item type from being spread
    across the hotbar unnecessarily.
*/

void Inventory_Init(Inventory *inventory)
{
    *inventory = (Inventory){ 0 };
    inventory->selectedSlot = 0;
}

void Inventory_UpdateSelection(Inventory *inventory, InventoryInput input)
{
    if (input.directSlot >= 0 && input.directSlot < HOTBAR_SLOT_COUNT)
    {
        inventory->selectedSlot = input.directSlot;
        return;
    }

    inventory->selectedSlot += input.slotChange;

    if (inventory->selectedSlot < 0)
    {
        inventory->selectedSlot = HOTBAR_SLOT_COUNT - 1;
    }
    if (inventory->selectedSlot >= HOTBAR_SLOT_COUNT)
    {
        inventory->selectedSlot = 0;
    }
}

int Inventory_AddItem(Inventory *inventory, ItemType type, int quantity)
{
    if (type == ITEM_NONE || quantity <= 0)
    {
        return quantity;
    }

    for (int i = 0; i < HOTBAR_SLOT_COUNT && quantity > 0; i++)
    {
        ItemStack *stack = &inventory->slots[i];
        if (stack->type != type || stack->quantity >= ITEM_MAX_STACK)
        {
            continue;
        }

        int availableSpace = ITEM_MAX_STACK - stack->quantity;
        int amountToAdd = quantity < availableSpace ? quantity : availableSpace;
        stack->quantity += amountToAdd;
        quantity -= amountToAdd;
    }

    for (int i = 0; i < HOTBAR_SLOT_COUNT && quantity > 0; i++)
    {
        ItemStack *stack = &inventory->slots[i];
        if (stack->type != ITEM_NONE)
        {
            continue;
        }

        int amountToAdd = quantity < ITEM_MAX_STACK ? quantity : ITEM_MAX_STACK;
        stack->type = type;
        stack->quantity = amountToAdd;
        quantity -= amountToAdd;
    }

    return quantity;
}

const ItemStack *Inventory_GetSelectedStack(const Inventory *inventory)
{
    return &inventory->slots[inventory->selectedSlot];
}

bool Inventory_RemoveOneSelected(Inventory *inventory)
{
    ItemStack *stack = &inventory->slots[inventory->selectedSlot];
    if (stack->type == ITEM_NONE || stack->quantity <= 0)
    {
        return false;
    }

    stack->quantity--;
    if (stack->quantity == 0)
    {
        stack->type = ITEM_NONE;
    }

    return true;
}

void Inventory_Draw(const Inventory *inventory)
{
    const int slotSize = 44;
    const int slotGap = 4;
    const int totalWidth = HOTBAR_SLOT_COUNT * slotSize +
                           (HOTBAR_SLOT_COUNT - 1) * slotGap;
    const int startX = (SCREEN_WIDTH - totalWidth) / 2;
    const int startY = SCREEN_HEIGHT - slotSize - 18;

    for (int i = 0; i < HOTBAR_SLOT_COUNT; i++)
    {
        int slotX = startX + i * (slotSize + slotGap);
        Rectangle slotBounds = {
            (float)slotX, (float)startY,
            (float)slotSize, (float)slotSize
        };
        Color borderColor = i == inventory->selectedSlot ? YELLOW : GRAY;

        DrawRectangleRec(slotBounds, (Color){ 35, 35, 45, 230 });
        DrawRectangleLinesEx(slotBounds, i == inventory->selectedSlot ? 3.0f : 1.0f,
                             borderColor);

        int displayedNumber = (i + 1) % 10;
        DrawText(TextFormat("%d", displayedNumber), slotX + 3, startY + 2,
                 10, LIGHTGRAY);

        const ItemStack *stack = &inventory->slots[i];
        if (stack->type == ITEM_NONE)
        {
            continue;
        }

        DrawRectangle(slotX + 12, startY + 12, 22, 22,
                      Item_GetColor(stack->type));
        DrawRectangleLines(slotX + 12, startY + 12, 22, 22, BLACK);

        const char *quantityText = TextFormat("%d", stack->quantity);
        int quantityWidth = MeasureText(quantityText, 14);
        DrawText(quantityText, slotX + slotSize - quantityWidth - 3,
                 startY + slotSize - 16, 14, RAYWHITE);
    }

    const ItemStack *selectedStack = Inventory_GetSelectedStack(inventory);
    const char *selectedName = Item_GetName(selectedStack->type);
    int nameWidth = MeasureText(selectedName, 16);
    DrawText(selectedName, (SCREEN_WIDTH - nameWidth) / 2,
             startY - 21, 16, RAYWHITE);
}
