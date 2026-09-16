#ifndef INTERACTION_H
#define INTERACTION_H

#include <stdbool.h>
#include "input.h"
#include "particle.h"
#include "player.h"
#include "world.h"

/*
    interaction.h

    Purpose:
    Handles block targeting, reach checks, mining, and placement. The current
    target is stored so it can also be outlined while the game is drawn.
*/

typedef struct Interaction
{
    int targetTileX;
    int targetTileY;
    bool targetIsInWorld;
    bool targetIsInReach;
} Interaction;

void Interaction_Init(Interaction *interaction);

/* Updates the target and performs a requested mining or placement action. */
void Interaction_Update(Interaction *interaction, World *world,
                        const Player *player, Camera2D camera,
                        InteractionInput input,
                        ParticleSystem *particleSystem);

/* Draws an outline over the tile under the mouse. */
void Interaction_Draw(const Interaction *interaction);

#endif /* INTERACTION_H */
