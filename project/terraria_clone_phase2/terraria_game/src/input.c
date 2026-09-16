#include "input.h"
#include "raylib.h"

/*
    input.c

    Implementation of the input mapping described in input.h.
*/

PlayerInput Input_GetPlayerInput(void)
{
    PlayerInput input = { 0 };

    if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT))
    {
        input.moveDirection -= 1.0f;
    }
    if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT))
    {
        input.moveDirection += 1.0f;
    }

    input.jumpPressed = IsKeyPressed(KEY_SPACE);

    return input;
}

InteractionInput Input_GetInteractionInput(void)
{
    InteractionInput input = { 0 };

    input.mouseScreenPosition = GetMousePosition();
    input.minePressed = IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
    input.placePressed = IsMouseButtonPressed(MOUSE_BUTTON_RIGHT);

    return input;
}

InventoryInput Input_GetInventoryInput(void)
{
    InventoryInput input = { 0 };
    input.directSlot = -1;

    bool controlIsDown = IsKeyDown(KEY_LEFT_CONTROL) ||
                         IsKeyDown(KEY_RIGHT_CONTROL);
    float wheelMove = GetMouseWheelMove();

    if (!controlIsDown)
    {
        if (wheelMove > 0.0f) input.slotChange = -1;
        if (wheelMove < 0.0f) input.slotChange = 1;
    }

    if (IsKeyPressed(KEY_ONE))   input.directSlot = 0;
    if (IsKeyPressed(KEY_TWO))   input.directSlot = 1;
    if (IsKeyPressed(KEY_THREE)) input.directSlot = 2;
    if (IsKeyPressed(KEY_FOUR))  input.directSlot = 3;
    if (IsKeyPressed(KEY_FIVE))  input.directSlot = 4;
    if (IsKeyPressed(KEY_SIX))   input.directSlot = 5;
    if (IsKeyPressed(KEY_SEVEN)) input.directSlot = 6;
    if (IsKeyPressed(KEY_EIGHT)) input.directSlot = 7;
    if (IsKeyPressed(KEY_NINE))  input.directSlot = 8;
    if (IsKeyPressed(KEY_ZERO))  input.directSlot = 9;

    return input;
}
