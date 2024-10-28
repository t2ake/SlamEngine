// sampler
layout(binding = SL_ALBEDO_SLOT) uniform sampler2D s_albedo;
layout(binding = SL_NORMAL_SLOT) uniform sampler2D s_normal;
layout(binding = SL_EMISSIVE_SLOT) uniform sampler2D s_emissive;
layout(binding = SL_ORM_SLOT) uniform sampler2D s_ORM;

// Uniform
layout(location = SL_USE_ALBEDO_TEXTURE_LOCATION) uniform bool u_useAlbedoTexture;
layout(location = SL_USE_NORMAL_TEXTURE_LOCATION) uniform bool u_useNormalTexture;
layout(location = SL_USE_EMISSIVE_TEXTURE_LOCATION) uniform bool u_useEmissiveTexture;
layout(location = SL_USE_OCCLUSION_TEXTURE_LOCATION) uniform bool u_useOcclusionTexture;
layout(location = SL_USE_ROUGHNESS_TEXTURE_LOCATION) uniform bool u_useRoughnessTexture;
layout(location = SL_USE_METALLIC_TEXTURE_LOCATION) uniform bool u_useMetallicTexture;
layout(location = SL_ALBEDO_FACTOR_LOCATION) uniform vec3 u_albedoFactor;
layout(location = SL_NORMAL_FACTOR_LOCATION) uniform vec3 u_normalFactor;
layout(location = SL_EMISSIVE_FACTOR_LOCATION) uniform vec3 u_emissiveFactor;
layout(location = SL_OCCLUSION_FACTOR_LOCATION) uniform float u_occlusionFactor;
layout(location = SL_ROUGHNESS_FACTOR_LOCATION) uniform float u_roughnessFactor;
layout(location = SL_METALLIC_FACTOR_LOCATION) uniform float u_metallicFactor;
// TODO: Tilling uniforms

layout(location = SL_REFLECTANCE_LOCATION) uniform float u_reflectance;

vec3 SampleAlbedoTexture(vec2 uv)
{
    // TODO: Compile albedo textures to linear space before rendering.
    return pow(texture(s_albedo, uv).xyz, vec3(2.2));
}
vec3 SampleNormalTexture(vec2 uv)
{
    return texture(s_normal, uv).xyz;
}
vec3 SampleEmissiveTexture(vec2 uv)
{
    return texture(s_emissive, uv).xyz;
}
vec3 SampleORMTexture(vec2 uv)
{
    return texture(s_ORM, uv).xyz;
}

struct Material
{
    vec3 albedo;
    vec3 normal;
    vec3 emissive;
    float occlusion;
    float roughness;
    float metallic;
    vec3 F0;
};

Material GetMaterial(vec2 uv)
{
    Material material;
    material.albedo = u_albedoFactor;
    material.normal = u_normalFactor;
    material.emissive = u_emissiveFactor;
    material.occlusion = u_occlusionFactor;
    material.roughness = u_roughnessFactor;
    material.metallic = u_metallicFactor;
    material.F0 = vec3(0.16 * u_reflectance * u_reflectance);
    
    if (u_useAlbedoTexture)
    {
    	material.albedo *= SampleAlbedoTexture(uv);
    }
    if (u_useNormalTexture)
    {
    	material.normal *= SampleNormalTexture(uv);
    }
    if (u_useEmissiveTexture)
    {
    	material.emissive *= SampleEmissiveTexture(uv);
    }
    vec3 orm = SampleORMTexture(uv);
    if (u_useOcclusionTexture)
    {
    	material.occlusion *= orm.x;
    }
    if (u_useRoughnessTexture)
    {
    	material.roughness *= orm.y;
    }
    if (u_useMetallicTexture)
    {
    	material.metallic *= orm.z;
    }
    
    material.F0 = mix(material.F0, material.albedo, vec3(material.metallic));
    
    return material;
}
