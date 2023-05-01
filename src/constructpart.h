#pragma once

#include "placementIndicator.h"

struct ConstructPart {
    PlaceableShape shape;
    PlaceableTexture texture;
    Paintjob paint;
    static PaintjobShaderInfo paintInfo;

    Matrix where;

    void draw();
    void drawShadow();
};

extern std::vector<ConstructPart> shipParts;

