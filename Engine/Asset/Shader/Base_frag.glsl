#version 450 core

// input
layout(location = 0) in vec3 v_worldPos;
layout(location = 1) in vec3 v_normal;
layout(location = 2) in vec2 v_uv0;

// output
layout(location = 0) out vec4 o_color;

// sampler
layout(binding = 0) uniform sampler2D s_albedo;
layout(binding = 1) uniform sampler2D s_normal;
layout(binding = 2) uniform sampler2D s_emissive;
layout(binding = 3) uniform sampler2D s_ORM;

// Uniform
layout(location = 1) uniform bool u_useAlbedoTexture;
layout(location = 2) uniform bool u_useNormalTexture;
layout(location = 3) uniform bool u_useEmissiveTexture;
layout(location = 4) uniform bool u_useOcclusionTexture;
layout(location = 5) uniform bool u_useRoughnessTexture;
layout(location = 6) uniform bool u_useMetallicTexture;
layout(location = 7) uniform vec3 u_albedoFactor;
layout(location = 8) uniform vec3 u_normalFactor;
layout(location = 9) uniform vec3 u_emissiveFactor;
layout(location = 10) uniform float u_occlusionFactor;
layout(location = 11) uniform float u_roughnessFactor;
layout(location = 12) uniform float u_metallicFactor;
// TODO: Tilling uniforms

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
};
Material GetMaterial()
{
	Material material;
	material.albedo = u_albedoFactor;
	material.normal = u_normalFactor;
	material.emissive = u_emissiveFactor;
	material.occlusion = u_occlusionFactor;
	material.roughness = u_roughnessFactor;
	material.metallic = u_metallicFactor;

	if (u_useAlbedoTexture)
	{
		material.albedo *= SampleAlbedoTexture(v_uv0);
	}
	if (u_useNormalTexture)
	{
		material.normal *= SampleNormalTexture(v_uv0);
	}
	if (u_useEmissiveTexture)
	{
		material.emissive *= SampleEmissiveTexture(v_uv0);
	}
	vec3 orm = SampleORMTexture(v_uv0);
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

	return material;
}

void main()
{
	Material material = GetMaterial();
	vec3 finalColor = material.albedo;

	// Post processing
	finalColor = pow(finalColor, vec3(1.0 / 2.2));
	o_color = vec4(finalColor, 1.0);
}
