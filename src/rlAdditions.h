#pragma once

#include "raylib.h"

void BeginViewProjectionMode(Matrix view, Matrix projection);
void EndViewProjectionMode();

RenderTexture2D LoadShadowMapTexture(int width, int height);