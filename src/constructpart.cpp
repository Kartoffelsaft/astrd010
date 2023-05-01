#include "constructpart.h"
#include "shaders.h"

PaintjobShaderInfo ConstructPart::paintInfo{};

void ConstructPart::draw() {
    paint.applyPaintjob(paintInfo);

    DrawMesh(placeableShapes[this->shape], placeableTextures[texture], where);
}

void ConstructPart::drawShadow() {
    DrawMesh(placeableShapes[this->shape], shadowMaterial, this->where);
}

std::vector<ConstructPart> shipParts;
