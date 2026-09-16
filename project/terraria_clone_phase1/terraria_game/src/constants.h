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

/* Temporary test-grid dimensions used only in Phase 1 to visually verify
   that the camera and coordinate system work before the real world
   (Phase 2) exists. */
#define TEST_GRID_COLUMNS  60
#define TEST_GRID_ROWS     40

#endif /* CONSTANTS_H */
