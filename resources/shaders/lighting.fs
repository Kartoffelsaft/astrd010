#version 330

// Input vertex attributes (from vertex shader)
in vec3 fragPosition;
in vec2 fragTexCoord;
//in vec4 fragColor;
in vec3 fragNormal;
in mat3 tangentSpace;

// Input uniform values
uniform sampler2D texture0; // diffuse
uniform sampler2D texture1; // normal
uniform vec4 colDiffuse;

// Output fragment color
out vec4 finalColor;

#include "textures.fsh"

#define AMBIENT_BRIGHTNESS 0.05
#define LIGHT_DIR vec3(0.71, 0.71, 0.0)

float getAlignment(vec3 normal) {
    return dot(normalize(tangentSpace * (texture2D(texture1, fragTexCoord).rgb * 2.0 - 1.0)), LIGHT_DIR);
}

float getBrightnessByAlignment(float alignment) {
    if (alignment < -0.1) { return AMBIENT_BRIGHTNESS; }
    else {
        return clamp(alignment + 0.1, AMBIENT_BRIGHTNESS, 1.0);
    }
}

void main() {
    float brightness = getBrightnessByAlignment(getAlignment(fragNormal));

    finalColor = vec4(getColor() * brightness, 1.0);
}
