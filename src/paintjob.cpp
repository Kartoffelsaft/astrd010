#include "paintjob.h"

void PaintjobShaderInfo::load(Shader shader) {
    this->shader = shader;
    
    this->primaryColorUniform   = GetShaderLocation(shader, "primaryColor");
    this->secondaryColorUniform = GetShaderLocation(shader, "secondaryColor");
    this->tertiaryColorUniform  = GetShaderLocation(shader, "tertiaryColor");
}


void Paintjob::applyPaintjob(PaintjobShaderInfo info) const {
    SetShaderValue(info.shader, info.primaryColorUniform,   &this->primaryColor,   SHADER_UNIFORM_VEC3);
    SetShaderValue(info.shader, info.secondaryColorUniform, &this->secondaryColor, SHADER_UNIFORM_VEC3);
    SetShaderValue(info.shader, info.tertiaryColorUniform,  &this->tertiaryColor,  SHADER_UNIFORM_VEC3);
}

