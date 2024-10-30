struct Light
{
    uint type;       // 0  4
    float intensity; // 4  4
    float range;     // 8  4
    float outer;     // 12 4
    float inner;     // 16 4
    vec4 color;      // 32 16
    vec4 position;   // 48 16
    vec4 direction;  // 64 16
};

layout(std140, binding = 1) uniform UBLights
{
    Light ub_lights[LIGHT_MAX_COUNT];
    uint ub_lightCount;
};
