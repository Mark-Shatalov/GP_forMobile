#ifndef PLAYER_H
#define PLAYER_H

#include <stdbool.h>
#include "raylib.h"
#include "input.h"
#include "world.h"

/*
    player.h

    Purpose:
    Stores the player's physical state and provides the movement, collision,
    and drawing behavior for the player. Position is the top-left corner of
    the player's collision rectangle, measured in world pixels.
*/

typedef struct Player
{
    Vector2 position;
    Vector2 velocity;
    float width;
    float height;
    bool isOnGround;
} Player;

/* Places the player at the given world position with no velocity. */
void Player_Init(Player *player, Vector2 spawnPosition);

/* Applies input, gravity, and collision for one frame. */
void Player_Update(Player *player, const World *world,
                   PlayerInput input, float deltaTime);

/* Draws a simple placeholder character in world space. */
void Player_Draw(const Player *player);

/* Returns the world-space point the camera should follow. */
Vector2 Player_GetCenter(const Player *player);

#endif /* PLAYER_H */
