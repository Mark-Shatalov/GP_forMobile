#ifndef PARTICLE_H
#define PARTICLE_H

#include <stdbool.h>
#include "raylib.h"
#include "constants.h"

/*
    particle.h

    Purpose:
    Owns a small fixed-size pool of visual particles. Phase 4 uses the pool
    for block-breaking debris. A fixed array keeps memory management simple
    and avoids allocating memory while the game is running.
*/

typedef struct Particle
{
    Vector2 position;
    Vector2 velocity;
    Color color;
    float size;
    float lifeRemaining;
    float startingLife;
    bool isActive;
} Particle;

typedef struct ParticleSystem
{
    Particle particles[MAX_BLOCK_PARTICLES];
} ParticleSystem;

/* Marks every particle slot as unused. */
void ParticleSystem_Init(ParticleSystem *system);

/* Creates a small burst using the color and area of a broken tile. */
void ParticleSystem_SpawnBlockBreak(ParticleSystem *system,
                                    Rectangle tileBounds, Color color);

/* Moves active particles and expires old ones. */
void ParticleSystem_Update(ParticleSystem *system, float deltaTime);

/* Draws all active particles in world space. */
void ParticleSystem_Draw(const ParticleSystem *system);

#endif /* PARTICLE_H */
