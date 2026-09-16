#ifndef CAMERA_CONTROLLER_H
#define CAMERA_CONTROLLER_H

#include "raylib.h"

/*
    camera_controller.h

    Purpose:
    Wraps Raylib's built-in Camera2D struct and gives us a place to put
    all camera-related behavior (panning, zooming, and later, following
    the player). Keeping this separate from game.c means the rest of
    the game does not need to know HOW the camera moves, only that it
    exists and can be used to draw the world correctly.

    In Phase 1, the camera can be moved manually (WASD/arrow keys) and
    zoomed (mouse wheel) so we can test that Camera2D is working before
    Phase 3 makes it follow the player automatically.
*/

typedef struct CameraController
{
    Camera2D camera;     /* Raylib's camera: position, zoom, rotation, etc. */
    float moveSpeed;     /* How fast the camera pans, in pixels per second */
} CameraController;

/* Sets up the camera with sensible starting values. */
void CameraController_Init(CameraController *controller);

/* Reads player input and updates the camera's position/zoom.
   Called once per frame, before drawing. */
void CameraController_Update(CameraController *controller, float deltaTime);

#endif /* CAMERA_CONTROLLER_H */
