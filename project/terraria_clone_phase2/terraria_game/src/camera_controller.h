#ifndef CAMERA_CONTROLLER_H
#define CAMERA_CONTROLLER_H

#include "raylib.h"

/*
    camera_controller.h

    Purpose:
    Wraps Raylib's built-in Camera2D struct and gives us a place to put
    all camera-related behavior (following the player and zooming).
    Keeping this separate from game.c means the rest of
    the game does not need to know HOW the camera moves, only that it
    exists and can be used to draw the world correctly.

    The camera follows the center of the player. Ctrl + mouse wheel controls
    zoom because the unmodified wheel selects hotbar slots in Phase 5.
*/

typedef struct CameraController
{
    Camera2D camera; /* Raylib's camera: position, zoom, rotation, etc. */
} CameraController;

/* Sets up the camera with sensible starting values. */
void CameraController_Init(CameraController *controller);

/* Smoothly follows a point in world space and handles mouse-wheel zoom. */
void CameraController_Update(CameraController *controller,
                             Vector2 followTarget, float deltaTime);

#endif /* CAMERA_CONTROLLER_H */
