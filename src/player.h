#pragma once

#include "raylib.h"
#include "raymath.h"
#include "placementIndicator.h"

struct Player {
    Vector3 position;
    Matrix rotation;

    PlacementIndicator placementIndicator;
    

    Camera3D GetCamera() const;
    Vector3 getRelPos(Vector3 pos) const;

    Matrix getPlacementIndicatorOrientation() const;

    void update();

    void drawPlacementIndicator() const;
};
