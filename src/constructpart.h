#pragma once

#include "placementIndicator.h"

struct ConstructPart {
    PlaceableShape shape;
    PlaceableTexture texture;
    Paintjob paint;
    static PaintjobShaderInfo paintInfo;

    Matrix where;

    void draw();
};

extern std::vector<ConstructPart> shipParts;

