#ifndef INPUT_H
#define INPUT_H

#include <stdbool.h>

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

/* Reads the keyboard once and returns the commands for this frame. */
PlayerInput Input_GetPlayerInput(void);

#endif /* INPUT_H */
