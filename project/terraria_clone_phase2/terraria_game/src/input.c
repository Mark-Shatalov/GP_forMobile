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
