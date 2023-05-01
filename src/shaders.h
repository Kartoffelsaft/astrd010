#pragma once

#include "raylib.h"

extern Shader shadowShader;
extern Material shadowMaterial;

extern RenderTexture2D shadowMap;

extern int defaultShaderLightViewUniform;

Shader loadShadowShader();

Shader loadShaderPreprocess(char const * vertexShaderFile, char const * fragmentShaderFile);
void updateLighting(Matrix lightView);
