// TODO: Uniform buffer object array

struct Light
{
    // 0. Point Light
    // 1. Directional Light
    // 2. Spot Light
    int type;
    vec3 color;
    float intensity;
    vec3 position;
    vec3 direction;
    float outer;
    float inner;
    float range;
};

struct DirectionalLight
{
    int type;
    vec3 color;
    float intensity;
    vec3 direction;
    float range;
};

struct PointLight
{
    int type;
    vec3 color;
    float intensity;
    vec3 position;
    float range;
};

struct SpotLight
{
    int type;
    vec3 color;
    float intensity;
    vec3 direction;
    vec3 position;
    float outer;
    float inner;
    float range;
};
