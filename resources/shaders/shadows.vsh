#define SHADOW_BIAS 0.003
#define SHADOWMAP_PRESCALE 0.02

vec4 containToLightView(vec4 pos) {
    return vec4(
        tanh(pos.x * SHADOWMAP_PRESCALE),
        tanh(pos.y * SHADOWMAP_PRESCALE),
        tanh(pos.z * SHADOWMAP_PRESCALE),
        pos.w
    );
}

vec4 containToLightmap(vec4 pos) {
    return vec4(
        tanh(pos.x * SHADOWMAP_PRESCALE) * 0.5 + 0.5,
        tanh(pos.y * SHADOWMAP_PRESCALE) * 0.5 + 0.5,
        tanh(pos.z * SHADOWMAP_PRESCALE) * 0.5 + 0.5,
        pos.w
    );
}