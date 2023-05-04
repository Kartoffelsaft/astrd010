#version 330

// Input vertex attributes
in vec3 vertexPosition;
in vec2 vertexTexCoord;
in vec3 vertexNormal;
in vec4 vertexColor;

// Input uniform values
uniform mat4 mvp;
uniform mat4 matModel;
uniform mat4 matNormal;
uniform mat4 lightView;

// Output vertex attributes (to fragment shader)
out vec3 fragPosition;
out vec2 fragTexCoord;
out vec4 fragColor;
out vec3 fragNormal;
out mat3 tangentSpace;
out vec3 shadowMapPos;

#include "shadows.vsh"

void main()
{
    // Send vertex attributes to fragment shader
    fragPosition = vec3(matModel*vec4(vertexPosition, 1.0));
    fragTexCoord = vertexTexCoord;
    fragColor = vertexColor;
    fragNormal = normalize(vec3(matNormal*vec4(vertexNormal, 1.0)));
    
    // unit vector that is perpendicular to the normal
    vec3 tangentNormal = normalize(cross(fragNormal, fragNormal + fragNormal.zyx + vec3(0, 0, -5)));
    tangentSpace = mat3(
        tangentNormal,
        cross(tangentNormal, fragNormal),
        fragNormal
    );

    shadowMapPos = containToLightmap(lightView*matModel*vec4(vertexPosition, 1.0)).xyz;

    // Calculate final vertex position
    gl_Position = mvp*vec4(vertexPosition, 1.0);
}
