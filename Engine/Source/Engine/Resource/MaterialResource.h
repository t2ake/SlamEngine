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

	int m_textureSlot = SL_ALBEDO_SLOT;
	int m_useTextureLocation = SL_USE_ALBEDO_TEXTURE_LOCATION;
	int m_factorLocation = SL_ALBEDO_FACTOR_LOCATION;
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

	int m_textureSlot = SL_NORMAL_SLOT;
	int m_useTextureLocation = SL_USE_NORMAL_TEXTURE_LOCATION;
	int m_factorLocation = SL_NORMAL_FACTOR_LOCATION;
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

	int m_textureSlot = SL_EMISSIVE_SLOT;
	int m_useTextureLocation = SL_USE_EMISSIVE_TEXTURE_LOCATION;
	int m_factorLocation = SL_EMISSIVE_FACTOR_LOCATION;
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

	int m_textureSlot = SL_ORM_SLOT;
	int m_useTextureLocation = SL_USE_OCCLUSION_TEXTURE_LOCATION;
	int m_factorLocation = SL_OCCLUSION_FACTOR_LOCATION;
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

	int m_textureSlot = SL_ORM_SLOT;
	int m_useTextureLocation = SL_USE_ROUGHNESS_TEXTURE_LOCATION;
	int m_factorLocation = SL_ROUGHNESS_FACTOR_LOCATION;
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

	int m_textureSlot = SL_ORM_SLOT;
	int m_useTextureLocation = SL_USE_METALLIC_TEXTURE_LOCATION;
	int m_factorLocation = SL_METALLIC_FACTOR_LOCATION;
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

	int m_reflectanceLocation = SL_REFLECTANCE_LOCATION;
	//int m_twoSideLocation = SL_TWO_SIDE_LOCATION;
};

} // namespace sl
