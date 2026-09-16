#include "game.h"
#include "constants.h"
#include "raylib.h"

/*
    game.c

    Implementation of the Game module.

    Phase 1 scope:
    - Initialize the camera controller
    - Update the camera every frame
    - Draw a simple grid of 16x16 tiles so we can visually confirm that
      the coordinate system and camera are working correctly

    The grid-drawing function below is intentionally simple and will be
    REPLACED in Phase 2 by World_Draw(), which will only draw tiles that
    are actually visible on screen, using real tile data instead of
    empty grid squares.
*/

/* Draws a grid of TILE_SIZE squares, purely for visual testing.
   This is a temporary stand-in for the real world rendering that
   Phase 2 will introduce. Marked 'static' because it is only used
   inside this file. */
static void DrawTestGrid(void)
{
    Color lineColor = (Color){ 60, 60, 60, 255 };

    int gridWidthPixels  = TEST_GRID_COLUMNS * TILE_SIZE;
    int gridHeightPixels = TEST_GRID_ROWS * TILE_SIZE;

    /* Vertical lines */
    for (int col = 0; col <= TEST_GRID_COLUMNS; col++)
    {
        int x = col * TILE_SIZE;
        DrawLine(x, 0, x, gridHeightPixels, lineColor);
    }

    /* Horizontal lines */
    for (int row = 0; row <= TEST_GRID_ROWS; row++)
    {
        int y = row * TILE_SIZE;
        DrawLine(0, y, gridWidthPixels, y, lineColor);
    }

    /* Draw a small marker at world origin (0,0) so we can see where
       "the world starts" while testing the camera. */
    DrawCircle(0, 0, 4, RED);
}

void Game_Init(Game *game)
{
    CameraController_Init(&game->cameraController);
}

void Game_Update(Game *game, float deltaTime)
{
    CameraController_Update(&game->cameraController, deltaTime);
}

void Game_Draw(Game *game)
{
    (void)game; /* not used yet in Phase 1, but kept for a consistent function signature */
    DrawTestGrid();
}

void Game_Shutdown(Game *game)
{
    (void)game; /* nothing to free yet in Phase 1 */
}
