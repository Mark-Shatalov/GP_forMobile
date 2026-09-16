#include "particle.h"

/*
    particle.c

    Block particles are deliberately simple colored squares. They can later
    be replaced by texture fragments without changing the interaction code.
*/

static Particle *FindUnusedParticle(ParticleSystem *system)
{
    for (int i = 0; i < MAX_BLOCK_PARTICLES; i++)
    {
        if (!system->particles[i].isActive)
        {
            return &system->particles[i];
        }
    }

    return NULL;
}

void ParticleSystem_Init(ParticleSystem *system)
{
    *system = (ParticleSystem){ 0 };
}

void ParticleSystem_SpawnBlockBreak(ParticleSystem *system,
                                    Rectangle tileBounds, Color color)
{
    for (int i = 0; i < BLOCK_BREAK_PARTICLES; i++)
    {
        Particle *particle = FindUnusedParticle(system);
        if (particle == NULL)
        {
            return;
        }

        particle->position = (Vector2){
            tileBounds.x + (float)GetRandomValue(2, (int)tileBounds.width - 2),
            tileBounds.y + (float)GetRandomValue(2, (int)tileBounds.height - 2)
        };
        particle->velocity = (Vector2){
            (float)GetRandomValue(-90, 90),
            (float)GetRandomValue(-150, -50)
        };
        particle->color = color;
        particle->size = (float)GetRandomValue(2, 4);
        particle->startingLife = GetRandomValue(40, 70) / 100.0f;
        particle->lifeRemaining = particle->startingLife;
        particle->isActive = true;
    }
}

void ParticleSystem_Update(ParticleSystem *system, float deltaTime)
{
    if (deltaTime > 0.05f)
    {
        deltaTime = 0.05f;
    }

    for (int i = 0; i < MAX_BLOCK_PARTICLES; i++)
    {
        Particle *particle = &system->particles[i];
        if (!particle->isActive)
        {
            continue;
        }

        particle->lifeRemaining -= deltaTime;
        if (particle->lifeRemaining <= 0.0f)
        {
            particle->isActive = false;
            continue;
        }

        particle->velocity.y += PARTICLE_GRAVITY * deltaTime;
        particle->position.x += particle->velocity.x * deltaTime;
        particle->position.y += particle->velocity.y * deltaTime;
    }
}

void ParticleSystem_Draw(const ParticleSystem *system)
{
    for (int i = 0; i < MAX_BLOCK_PARTICLES; i++)
    {
        const Particle *particle = &system->particles[i];
        if (!particle->isActive)
        {
            continue;
        }

        float lifePercent = particle->lifeRemaining / particle->startingLife;
        Color fadingColor = particle->color;
        fadingColor.a = (unsigned char)(255.0f * lifePercent);

        DrawRectangleV(particle->position,
                       (Vector2){ particle->size, particle->size },
                       fadingColor);
    }
}
