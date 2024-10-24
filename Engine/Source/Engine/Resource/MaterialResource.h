#pragma once

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
};

struct NormalPropertyGroup
{
	std::string m_texture;
	bool m_useTexture = false;
	glm::vec3 m_factor{ 1.0f, 1.0f , 1.0f };

	glm::vec2 m_offset{ 0.0f, 0.0f };
	glm::vec2 m_scale{ 1.0f, 1.0f };
	float m_rotation = 0.0f;
};

struct EmissivePropertyGroup
{
	std::string m_texture;
	bool m_useTexture = false;
	glm::vec3 m_factor{ 1.0f, 1.0f , 1.0f };

	glm::vec2 m_offset{ 0.0f, 0.0f };
	glm::vec2 m_scale{ 1.0f, 1.0f };
	float m_rotation = 0.0f;
};

struct OcclusionPropertyGroup
{
	std::string m_texture;
	bool m_useTexture = false;
	float m_factor = 1.0f;

	glm::vec2 m_offset{ 0.0f, 0.0f };
	glm::vec2 m_scale{ 1.0f, 1.0f };
	float m_rotation = 0.0f;
};

struct RoughnessPropertyGroup
{
	std::string m_texture;
	bool m_useTexture = false;
	float m_factor = 1.0f;

	glm::vec2 m_offset{ 0.0f, 0.0f };
	glm::vec2 m_scale{ 1.0f, 1.0f };
	float m_rotation = 0.0f;
};

struct MetallicPropertyGroup
{
	std::string m_texture;
	bool m_useTexture = false;
	float m_factor = 1.0f;

	glm::vec2 m_offset{ 0.0f, 0.0f };
	glm::vec2 m_scale{ 1.0f, 1.0f };
	float m_rotation = 0.0f;
};

class MaterialResource : public Resource
{
public:
	MaterialResource(std::string_view path);
	virtual ~MaterialResource() override;

	virtual void OnImport() override;
	virtual void OnBuild() override;
	virtual void OnLoad() override;
	virtual void OnUpload() override;
	virtual void OnReady() override;
	virtual void OnDestroy() override;
	virtual void DestroyCPUData() override;

	std::string m_assetPath;

	// TODO: Only support PBR for now.
	AlbedoPropertyGroup m_albedoPropertyGroup;
	NormalPropertyGroup m_normalPropertyGroup;
	EmissivePropertyGroup m_emissivePropertyGroup;
	OcclusionPropertyGroup m_occlusionPropertyGroup;
	RoughnessPropertyGroup m_roughnessPropertyGroup;
	MetallicPropertyGroup m_metallicPropertyGroup;

	float m_specular = 0.5f;
	bool m_twoSide = false;
};

} // namespace sl
