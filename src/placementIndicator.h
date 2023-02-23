#pragma once

#include <vector>

#include "paintjob.h"

extern std::vector<Mesh> placeableShapes;
extern std::vector<std::vector<Vector3>> placeableSnapPoints;
extern std::vector<Material> placeableTextures;

enum PlaceableShape {
    TUBE = 0,
    CUBE,

    PLACEABLE_SHAPES_COUNT
};

enum PlaceableTexture {
    ERODED_METAL = 0,

    PLACEABLE_TEXTURES_COUNT
};


struct PlacementIndicator {
    float distance = 3.0f;

    int placeableSelection;

    int rotSnapIndexX = 0;
    int rotSnapIndexY = 0;
    int rotSnapIndexZ = 0;
    
    Paintjob paint = Paintjob{
        .primaryColor = Vector3{0.87f, 0.05f, 0.03f},
        .secondaryColor = Vector3{0.79f, 0.75f, 0.71f},
        .tertiaryColor = Vector3{0.02f, 0.03f, 0.07f},
    };
    PaintjobShaderInfo paintInfo;
    
    bool selectionMenuIsOpen;

    void update();
    Matrix getRotationMatrix() const;
    void drawAt(Matrix const mtx) const;
    void drawSelectionMenu();
};
