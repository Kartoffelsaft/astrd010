#version 330

// Input vertex attributes
in vec3 vertexPosition;
in vec2 vertexTexCoord;
in vec3 vertexNormal;
in vec4 vertexColor;

// Input uniform values
uniform mat4 mvp;

// Output vertex attributes (to fragment shader)
out vec3 fragPosition;
out vec2 fragTexCoord;

#include "shadows.vsh"

void main()
{
    fragTexCoord = vec2(0.0);

    vec4 outpos = containToLightView(mvp*vec4(vertexPosition, 1.0));
    fragPosition = outpos.xyz;
    gl_Position = vec4(outpos.xy, -outpos.z, 1.0);
}
