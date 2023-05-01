#version 330

// Input vertex attributes (from vertex shader)
in vec3 fragPosition;
in vec2 fragTexCoord;
//in vec4 fragColor;
in vec3 fragNormal;
in mat3 tangentSpace;
in vec3 shadowMapPos;

// Input uniform values
uniform sampler2D texture0; // diffuse
uniform sampler2D texture1; // normal
uniform sampler2D texture2; // lightmap
uniform vec4 colDiffuse;
uniform mat4 lightView;

// Output fragment color
out vec4 finalColor;

#include "textures.fsh"
#include "shadows.vsh"

#define AMBIENT_BRIGHTNESS 0.05

float getAlignment(vec3 normal) {
    vec3 lightdir = normalize((lightView * vec4(0.0, 0.0, -1.0, 1.0) - lightView * vec4(0.0, 0.0, 0.0, 1.0)).xyz);
    vec3 normaldir = normalize(tangentSpace * (texture2D(texture1, fragTexCoord).rgb * 2.0 - 1.0));
    return dot(normaldir, lightdir);
}

float getBrightnessByAlignment(float alignment) {
    if (alignment < -0.1) { return AMBIENT_BRIGHTNESS; }
    else {
        return clamp(alignment + 0.1, AMBIENT_BRIGHTNESS, 1.0);
    }
}

float getBrightnessByShadow() {
    if (texture(texture2, shadowMapPos.xy).r < shadowMapPos.z + SHADOW_BIAS) {
        return 1.0;
    } else {
        return 0.0;
    }
}

void main() {
    float brightness = clamp(
        getBrightnessByShadow()
      * getBrightnessByAlignment(getAlignment(fragNormal)),
    AMBIENT_BRIGHTNESS, 1.0);

    finalColor = vec4(getColor() * brightness, 1.0);
}
