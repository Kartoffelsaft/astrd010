#include "constructpart.h"

PaintjobShaderInfo ConstructPart::paintInfo{};

void ConstructPart::draw() {
    paint.applyPaintjob(paintInfo);

    DrawMesh(placeableShapes[this->shape], placeableTextures[texture], where);
}

std::vector<ConstructPart> shipParts;
