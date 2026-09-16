#ifndef INPUT_H
#define INPUT_H

#include <stdbool.h>
#include "raylib.h"

/*
    input.h

    Purpose:
    Converts Raylib keyboard state into a small set of commands the player
    understands. This keeps key choices out of the movement and collision
    code, making controls easy to change later.
*/

typedef struct PlayerInput
{
    float moveDirection; /* -1 for left, 0 for still, 1 for right */
    bool jumpPressed;    /* true only on the frame jump was pressed */
} PlayerInput;

typedef struct InteractionInput
{
    Vector2 mouseScreenPosition;
    bool minePressed;
    bool placePressed;
} InteractionInput;

typedef struct InventoryInput
{
    int slotChange;      /* -1 or 1 when the mouse wheel changes slots */
    int directSlot;      /* 0-9 from number keys, or -1 when unused */
} InventoryInput;

/* Reads the keyboard once and returns the commands for this frame. */
PlayerInput Input_GetPlayerInput(void);

/* Reads mouse position and block-interaction buttons for this frame. */
InteractionInput Input_GetInteractionInput(void);

/* Reads hotbar selection controls. Ctrl + wheel is reserved for zoom. */
InventoryInput Input_GetInventoryInput(void);

#endif /* INPUT_H */
