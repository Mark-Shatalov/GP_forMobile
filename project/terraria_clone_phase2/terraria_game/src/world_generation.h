#ifndef WORLD_GENERATION_H
#define WORLD_GENERATION_H

#include "world.h"

/*
    world_generation.h

    Purpose:
    Builds the initial tile layout. Keeping generation separate means the
    World module can focus on tile storage, lookup, and visible rendering.
*/

void WorldGeneration_Generate(World *world);

#endif /* WORLD_GENERATION_H */
