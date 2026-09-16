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

}

static void KeepCameraInsideWorld(CameraController *controller)
{
    float halfViewWidth = SCREEN_WIDTH / (2.0f * controller->camera.zoom);
    float halfViewHeight = SCREEN_HEIGHT / (2.0f * controller->camera.zoom);
    float worldWidth = WORLD_WIDTH_TILES * TILE_SIZE;
    float worldHeight = WORLD_HEIGHT_TILES * TILE_SIZE;

    if (controller->camera.target.x < halfViewWidth)
    {
        controller->camera.target.x = halfViewWidth;
    }
    if (controller->camera.target.x > worldWidth - halfViewWidth)
    {
        controller->camera.target.x = worldWidth - halfViewWidth;
    }
    if (controller->camera.target.y < halfViewHeight)
    {
        controller->camera.target.y = halfViewHeight;
    }
    if (controller->camera.target.y > worldHeight - halfViewHeight)
    {
        controller->camera.target.y = worldHeight - halfViewHeight;
    }
}

void CameraController_Update(CameraController *controller,
                             Vector2 followTarget, float deltaTime)
{
    const float followSpeed = 8.0f;
    float followAmount = followSpeed * deltaTime;

    if (followAmount > 1.0f)
    {
        followAmount = 1.0f;
    }

    /* A small amount of camera smoothing lets the player move away from
       screen center briefly, making horizontal motion easier to see. */
    controller->camera.target.x +=
        (followTarget.x - controller->camera.target.x) * followAmount;
    controller->camera.target.y +=
        (followTarget.y - controller->camera.target.y) * followAmount;

    bool controlIsDown = IsKeyDown(KEY_LEFT_CONTROL) ||
                         IsKeyDown(KEY_RIGHT_CONTROL);
    float wheelMove = GetMouseWheelMove();
    if (controlIsDown && wheelMove != 0.0f)
    {
        controller->camera.zoom += wheelMove * 0.1f;

        /* Clamp zoom so the world can't disappear or become huge */
        if (controller->camera.zoom < 0.5f) controller->camera.zoom = 0.5f;
        if (controller->camera.zoom > 4.0f) controller->camera.zoom = 4.0f;
    }

    KeepCameraInsideWorld(controller);
}
