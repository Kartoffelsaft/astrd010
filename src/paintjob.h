#pragma once

#include "raylib.h"
#include "raymath.h"

struct PaintjobShaderInfo {
    Shader shader;
    
    int primaryColorUniform;
    int secondaryColorUniform;
    int tertiaryColorUniform;
    
    void load(Shader shader);
};

struct Paintjob {
    Vector3 primaryColor;
    Vector3 secondaryColor;
    Vector3 tertiaryColor;

    void applyPaintjob(PaintjobShaderInfo info) const;
};
