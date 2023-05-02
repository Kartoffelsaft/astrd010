#pragma once

#include "raylib.h"

extern Shader defaultShader;
extern Shader indicatorShader;
extern Shader shadowShader;
extern Material shadowMaterial;

extern RenderTexture2D shadowMap;

extern int defaultShaderLightViewUniform;

void initShaders();

Shader loadShaderPreprocess(char const * vertexShaderFile, char const * fragmentShaderFile);
void updateLighting(Matrix lightView);
