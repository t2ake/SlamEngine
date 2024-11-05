#pragma once

#include "Scene/ECSWorld.h"

#include <glm/vec3.hpp>

#include <cstdint>
#include <string>

struct aiScene;
struct aiNode;
struct aiMesh;
struct aiMaterial;
struct aiTexture;

namespace sl
{

class ModelImporter final
{
public:
	ModelImporter(std::string_view sourcePath);

	void SetParentEntity(sl::Entity entity) { m_parentEntity = entity; }
	sl::Entity GetParentEntity() const { return m_parentEntity; }
	sl::Entity &GetParentEntity() { return m_parentEntity; }

	void SetGlobalScale(float scale) { m_globalScale = scale; }
	float GetGlobalScale() const { return m_globalScale; }
	float &GetGlobalScale() { return m_globalScale; }

	void Import();

private:
	void ProcessNode(const aiNode *pNode);
	void ProcessMesh(const aiMesh *pMesh);
	std::string ProcessMaterial(const aiMaterial *pMaterial);
	std::string ProcessTexture(const char *textureStr);
	const aiTexture *GetEmbeddedTexture(const char *textureStr);

	std::string m_sourcePath;

	sl::Entity m_parentEntity;
	float m_globalScale;

	const aiScene *m_pScene;
};

} // namespace sl
