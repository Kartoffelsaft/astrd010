#version 330

in vec2 fragTexCoord;
in vec3 fragPosition;
uniform sampler2D texture0;

out vec4 finalColor;

void main() {
    finalColor = vec4(vec3(fragPosition.z * 0.5 + 0.5), 1.0);
}
