uniform vec3 primaryColor;
uniform vec3 secondaryColor;
uniform vec3 tertiaryColor;

vec3 getColor() {
    vec3 base = texture(texture0, fragTexCoord).rgb;
    base = base / (base.r + base.g + base.b);

    return mat3(primaryColor, secondaryColor, tertiaryColor) * base;
}
