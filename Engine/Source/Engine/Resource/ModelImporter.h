#pragma once

#include "Scene/ECSWorld.h"

#include <glm/glm.hpp>

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
	void SetGlobalScale(const glm::vec3 &scale) { m_globalScale = scale; }

	void Import();

private:
	void ProcessNode(const aiNode *pNode);
	void ProcessMesh(const aiMesh *pMesh);
	std::string ProcessMaterial(const aiMaterial *pMaterial);
	std::string ProcessTexture(const char *textureStr);
	const aiTexture *GetEmbeddedTexture(const char *textureStr);

	std::string m_sourcePath;

	sl::Entity m_parentEntity;
	glm::vec3 m_globalScale{ 1.0f };

	const aiScene *m_pScene;
};

} // namespace sl
