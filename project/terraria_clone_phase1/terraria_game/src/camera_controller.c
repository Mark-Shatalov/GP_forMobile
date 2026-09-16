#include "camera_controller.h"
#include "constants.h"

/*
    camera_controller.c

    Implementation of the camera wrapper described in camera_controller.h.
    Everything here only touches the CameraController struct — it does not
    know anything about tiles, players, or the rest of the game.
*/

void CameraController_Init(CameraController *controller)
{
    controller->camera.target   = (Vector2){ 0.0f, 0.0f };   /* world point the camera looks at */
    controller->camera.offset   = (Vector2){ SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f }; /* screen point that target is drawn at (center of screen) */
    controller->camera.rotation = 0.0f;
    controller->camera.zoom     = 2.0f; /* start zoomed in a bit since tiles are only 16px */

    controller->moveSpeed = 300.0f;
}

void CameraController_Update(CameraController *controller, float deltaTime)
{
    /* --- Manual panning (temporary, for Phase 1 testing only) ---
       Once Phase 3 adds the player, the camera will follow the player
       instead of being moved by these keys. */
    float moveAmount = controller->moveSpeed * deltaTime;

    if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT))  controller->camera.target.x -= moveAmount;
    if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) controller->camera.target.x += moveAmount;
    if (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP))    controller->camera.target.y -= moveAmount;
    if (IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN))  controller->camera.target.y += moveAmount;

    /* --- Zoom with mouse wheel --- */
    float wheelMove = GetMouseWheelMove();
    if (wheelMove != 0.0f)
    {
        controller->camera.zoom += wheelMove * 0.1f;

        /* Clamp zoom so the world can't disappear or become huge */
        if (controller->camera.zoom < 0.5f) controller->camera.zoom = 0.5f;
        if (controller->camera.zoom > 4.0f) controller->camera.zoom = 4.0f;
    }
}
