#version 330

in vec2 fragTexCoord;
uniform sampler2D texture0;

out vec4 finalColor;

#include "textures.fsh"

void main() {
    finalColor = vec4(getColor(), int(gl_FragCoord.x + gl_FragCoord.y) % 2);
}
