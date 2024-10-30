#pragma once

#include "Asset/Shader/Shared/PBRMaterial.h"
#include "Resource/Resource.h"

#include <glm/glm.hpp>

#include <memory>
#include <string>

namespace sl
{

struct AlbedoPropertyGroup
{
	std::string m_texture;
	bool m_useTexture = false;
	glm::vec3 m_factor{ 1.0f, 1.0f , 1.0f };

	glm::vec2 m_offset{ 0.0f, 0.0f };
	glm::vec2 m_scale{ 1.0f, 1.0f };
	float m_rotation = 0.0f;

	int m_textureSlot = SL_SLOT_ALBEDO;
	int m_useTextureLocation = SL_LOCATION_USE_ALBEDO_TEXTURE;
	int m_factorLocation = SL_LOCATION_ALBEDO_FACTOR;
	int m_tillingLocation;
};

struct NormalPropertyGroup
{
	std::string m_texture;
	bool m_useTexture = false;
	glm::vec3 m_factor{ 1.0f, 1.0f , 1.0f };

	glm::vec2 m_offset{ 0.0f, 0.0f };
	glm::vec2 m_scale{ 1.0f, 1.0f };
	float m_rotation = 0.0f;

	int m_textureSlot = SL_SLOT_NORMAL;
	int m_useTextureLocation = SL_LOCATION_USE_NORMAL_TEXTURE;
	int m_factorLocation = SL_LOCATION_NORMAL_FACTOR;
	int m_tillingLocation;
};

struct EmissivePropertyGroup
{
	std::string m_texture;
	bool m_useTexture = false;
	glm::vec3 m_factor{ 1.0f, 1.0f , 1.0f };

	glm::vec2 m_offset{ 0.0f, 0.0f };
	glm::vec2 m_scale{ 1.0f, 1.0f };
	float m_rotation = 0.0f;

	int m_textureSlot = SL_SLOT_EMISSIVE;
	int m_useTextureLocation = SL_LOCATION_USE_EMISSIVE_TEXTURE;
	int m_factorLocation = SL_LOCATION_EMISSIVE_FACTOR;
	int m_tillingLocation;
};

struct OcclusionPropertyGroup
{
	std::string m_texture;
	bool m_useTexture = false;
	float m_factor = 1.0f;

	glm::vec2 m_offset{ 0.0f, 0.0f };
	glm::vec2 m_scale{ 1.0f, 1.0f };
	float m_rotation = 0.0f;

	int m_textureSlot = SL_SLOT_ORM;
	int m_useTextureLocation = SL_LOCATION_USE_OCCLUSION_TEXTURE;
	int m_factorLocation = SL_LOCATION_OCCLUSION_FACTOR;
	int m_tillingLocation;
};

struct RoughnessPropertyGroup
{
	std::string m_texture;
	bool m_useTexture = false;
	float m_factor = 1.0f;

	glm::vec2 m_offset{ 0.0f, 0.0f };
	glm::vec2 m_scale{ 1.0f, 1.0f };
	float m_rotation = 0.0f;

	int m_textureSlot = SL_SLOT_ORM;
	int m_useTextureLocation = SL_LOCATION_USE_ROUGHNESS_TEXTURE;
	int m_factorLocation = SL_LOCATION_ROUGHNESS_FACTOR;
	int m_tillingLocation;
};

struct MetallicPropertyGroup
{
	std::string m_texture;
	bool m_useTexture = false;
	float m_factor = 1.0f;

	glm::vec2 m_offset{ 0.0f, 0.0f };
	glm::vec2 m_scale{ 1.0f, 1.0f };
	float m_rotation = 0.0f;

	int m_textureSlot = SL_SLOT_ORM;
	int m_useTextureLocation = SL_LOCATION_USE_METALLIC_TEXTURE;
	int m_factorLocation = SL_LOCATION_METALLIC_FACTOR;
	int m_tillingLocation;
};

class MaterialResource : public Resource
{
public:
	MaterialResource() = default;
	virtual ~MaterialResource() override;

	virtual void OnImport() override;
	virtual void OnBuild() override;
	virtual void OnLoad() override;
	virtual void OnUpload() override;
	virtual void OnReady() override;
	virtual void OnDestroy() override;
	virtual void DestroyCPUData() override;

	// TODO: Only support PBR for now.
	AlbedoPropertyGroup m_albedoPropertyGroup;
	NormalPropertyGroup m_normalPropertyGroup;
	EmissivePropertyGroup m_emissivePropertyGroup;
	OcclusionPropertyGroup m_occlusionPropertyGroup;
	RoughnessPropertyGroup m_roughnessPropertyGroup;
	MetallicPropertyGroup m_metallicPropertyGroup;

	float m_reflectance = 0.5f;
	bool m_twoSide = false;

	int m_reflectanceLocation = SL_LOCATION_REFLECTANCE;
	int m_twoSideLocation = SL_LOCATION_TWO_SIDE;
};

} // namespace sl
