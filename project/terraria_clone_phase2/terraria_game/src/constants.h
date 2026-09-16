#ifndef CONSTANTS_H
#define CONSTANTS_H

/*
    constants.h

    Purpose:
    This file holds every "magic number" used in the game in one place.
    Instead of writing 800 or 16 directly inside game logic, we give
    those numbers a name. This makes the code easier to read and easier
    to tune later (for example, changing the tile size in one place
    instead of hunting through every file).

    Nothing in this file does anything by itself — it only defines values.
*/

/* ---------- Window settings ---------- */
#define SCREEN_WIDTH   1280
#define SCREEN_HEIGHT  720
#define WINDOW_TITLE   "Terraria-Style Sandbox (C + Raylib)"
#define TARGET_FPS     60

/* ---------- World / tile settings ---------- */
/* Tile size is defined here already because later phases (world, tile,
   player) will all need to agree on how big one tile is in pixels. */
#define TILE_SIZE      16

/* Size of the world, measured in tiles (not pixels). */
#define WORLD_WIDTH_TILES   200
#define WORLD_HEIGHT_TILES  100

/* Average row where procedural surface generation begins. */
#define GROUND_LEVEL_TILE   25

/* ---------- Procedural world generation ---------- */
#define WORLD_GENERATION_SEED     1337
#define SURFACE_MIN_TILE          18
#define SURFACE_MAX_TILE          32
#define MIN_DIRT_LAYER_THICKNESS  4
#define MAX_DIRT_LAYER_THICKNESS  7
#define CAVE_COUNT                34
#define ORE_VEIN_COUNT            60

/* ---------- Player settings ---------- */
/* The player is slightly narrower than one tile and a little shorter
   than two tiles. This leaves enough room to walk through simple gaps. */
#define PLAYER_WIDTH       12.0f
#define PLAYER_HEIGHT      28.0f
#define PLAYER_MOVE_SPEED  140.0f
#define PLAYER_JUMP_SPEED  320.0f
#define PLAYER_GRAVITY     900.0f
#define PLAYER_MAX_FALL_SPEED 500.0f

/* ---------- Block interaction settings ---------- */
#define BLOCK_REACH_TILES       5.0f
#define MAX_BLOCK_PARTICLES     128
#define BLOCK_BREAK_PARTICLES   10
#define PARTICLE_GRAVITY        300.0f

/* ---------- Inventory / dropped item settings ---------- */
#define HOTBAR_SLOT_COUNT       10
#define ITEM_MAX_STACK          99
#define MAX_DROPPED_ITEMS       128
#define DROPPED_ITEM_SIZE       8.0f
#define DROPPED_ITEM_GRAVITY    600.0f
#define ITEM_PICKUP_DELAY       0.25f

#endif /* CONSTANTS_H */
