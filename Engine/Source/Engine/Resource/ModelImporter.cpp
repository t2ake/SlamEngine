#include "ModelImporter.h"

#include "Core/Log.h"
#include "Core/Path.hpp"
#include "RenderCore/VertexArray.h"
#include "Resource/MeshResource.h"
#include "Resource/ResourceManager.h"
#include "Scene/ECSWorld.h"
#include "Utils/ProfilerCPU.h"

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

namespace sl
{

namespace
{

void ProcessTexture(std::string_view path)
{
	if (sl::ResourceManager::GetTextureResource(path))
	{
		return;
	}

	// TODO: Support embedded texture.

	auto pTextureResource = std::make_unique<sl::TextureResource>(path, SL_SAMPLER_REPEAT | SL_SAMPLER_TRILINEAR);
	sl::ResourceManager::AddTextureResource(path, std::move(pTextureResource));
}

std::string ProcessMaterial(const aiMaterial *pMaterial, std::string_view path)
{
	auto pMaterialResource = std::make_unique<MaterialResource>("TODO");

	// Albedo
	if (aiString texture; pMaterial->Get(AI_MATKEY_TEXTURE(aiTextureType_BASE_COLOR, 0), texture) == AI_SUCCESS)
	{
		std::string fullPath = Path::Join(Path::Parent(path), texture.C_Str());
		ProcessTexture(fullPath);

		pMaterialResource->m_albedoPropertyGroup.m_texture = std::move(fullPath);
		pMaterialResource->m_albedoPropertyGroup.m_useTexture = true;
	}
	if (aiColor3D factor; pMaterial->Get(AI_MATKEY_BASE_COLOR, factor) == AI_SUCCESS)
	{
		pMaterialResource->m_albedoPropertyGroup.m_factor = glm::vec3{ factor.r, factor.g , factor.b };
	}
	if (aiUVTransform transform; pMaterial->Get(AI_MATKEY_UVTRANSFORM(aiTextureType_BASE_COLOR, 0), transform) == AI_SUCCESS)
	{
		pMaterialResource->m_albedoPropertyGroup.m_offset = glm::vec2{ transform.mTranslation.x, transform.mTranslation.y };
		pMaterialResource->m_albedoPropertyGroup.m_scale = glm::vec2{ transform.mScaling.x, transform.mScaling.y };
		pMaterialResource->m_albedoPropertyGroup.m_rotation = transform.mRotation;
	}

	// Normal
	if (aiString texture; pMaterial->Get(AI_MATKEY_TEXTURE(aiTextureType_NORMALS, 0), texture) == AI_SUCCESS)
	{
		std::string fullPath = Path::Join(Path::Parent(path), texture.C_Str());
		ProcessTexture(fullPath);

		pMaterialResource->m_normalPropertyGroup.m_texture = texture.C_Str();
		pMaterialResource->m_normalPropertyGroup.m_useTexture = true;
	}
	if (aiUVTransform transform; pMaterial->Get(AI_MATKEY_UVTRANSFORM(aiTextureType_NORMALS, 0), transform) == AI_SUCCESS)
	{
		pMaterialResource->m_normalPropertyGroup.m_offset = glm::vec2{ transform.mTranslation.x, transform.mTranslation.y };
		pMaterialResource->m_normalPropertyGroup.m_scale = glm::vec2{ transform.mScaling.x, transform.mScaling.y };
		pMaterialResource->m_normalPropertyGroup.m_rotation = transform.mRotation;
	}

	// Metallic
	if (aiString texture; pMaterial->Get(AI_MATKEY_TEXTURE(aiTextureType_METALNESS, 0), texture) == AI_SUCCESS)
	{
		std::string fullPath = Path::Join(Path::Parent(path), texture.C_Str());
		ProcessTexture(fullPath);

		pMaterialResource->m_metallicPropertyGroup.m_texture = texture.C_Str();
		pMaterialResource->m_metallicPropertyGroup.m_useTexture = true;
	}
	if (float factor; pMaterial->Get(AI_MATKEY_METALLIC_FACTOR, factor) == AI_SUCCESS)
	{
		pMaterialResource->m_metallicPropertyGroup.m_factor = factor;
	}
	if (aiUVTransform transform; pMaterial->Get(AI_MATKEY_UVTRANSFORM(aiTextureType_METALNESS, 0), transform) == AI_SUCCESS)
	{
		pMaterialResource->m_metallicPropertyGroup.m_offset = glm::vec2{ transform.mTranslation.x, transform.mTranslation.y };
		pMaterialResource->m_metallicPropertyGroup.m_scale = glm::vec2{ transform.mScaling.x, transform.mScaling.y };
		pMaterialResource->m_metallicPropertyGroup.m_rotation = transform.mRotation;
	}

	// Roughness
	if (aiString texture; pMaterial->Get(AI_MATKEY_TEXTURE(aiTextureType_DIFFUSE_ROUGHNESS, 0), texture) == AI_SUCCESS)
	{
		std::string fullPath = Path::Join(Path::Parent(path), texture.C_Str());
		ProcessTexture(fullPath);

		pMaterialResource->m_roughnessPropertyGroup.m_texture = texture.C_Str();
		pMaterialResource->m_roughnessPropertyGroup.m_useTexture = true;
	}
	if (float factor; pMaterial->Get(AI_MATKEY_ROUGHNESS_FACTOR, factor) == AI_SUCCESS)
	{
		pMaterialResource->m_roughnessPropertyGroup.m_factor = factor;
	}
	if (aiUVTransform transform; pMaterial->Get(AI_MATKEY_UVTRANSFORM(aiTextureType_DIFFUSE_ROUGHNESS, 0), transform) == AI_SUCCESS)
	{
		pMaterialResource->m_roughnessPropertyGroup.m_offset = glm::vec2{ transform.mTranslation.x, transform.mTranslation.y };
		pMaterialResource->m_roughnessPropertyGroup.m_scale = glm::vec2{ transform.mScaling.x, transform.mScaling.y };
		pMaterialResource->m_roughnessPropertyGroup.m_rotation = transform.mRotation;
	}

	// Occlusion
	if (aiString texture; pMaterial->Get(AI_MATKEY_TEXTURE(aiTextureType_AMBIENT_OCCLUSION, 0), texture) == AI_SUCCESS)
	{
		std::string fullPath = Path::Join(Path::Parent(path), texture.C_Str());
		ProcessTexture(fullPath);

		pMaterialResource->m_occlusionPropertyGroup.m_texture = texture.C_Str();
		pMaterialResource->m_occlusionPropertyGroup.m_useTexture = true;
	}
	if (aiUVTransform transform; pMaterial->Get(AI_MATKEY_UVTRANSFORM(aiTextureType_AMBIENT_OCCLUSION, 0), transform) == AI_SUCCESS)
	{
		pMaterialResource->m_occlusionPropertyGroup.m_offset = glm::vec2{ transform.mTranslation.x, transform.mTranslation.y };
		pMaterialResource->m_occlusionPropertyGroup.m_scale = glm::vec2{ transform.mScaling.x, transform.mScaling.y };
		pMaterialResource->m_occlusionPropertyGroup.m_rotation = transform.mRotation;
	}

	// Emissive
	if (aiString texture; pMaterial->Get(AI_MATKEY_TEXTURE(aiTextureType_EMISSION_COLOR, 0), texture) == AI_SUCCESS)
	{
		std::string fullPath = Path::Join(Path::Parent(path), texture.C_Str());
		ProcessTexture(fullPath);

		pMaterialResource->m_emissivePropertyGroup.m_texture = texture.C_Str();
		pMaterialResource->m_emissivePropertyGroup.m_useTexture = true;
	}
	if (aiColor3D factor; pMaterial->Get(AI_MATKEY_EMISSIVE_INTENSITY, factor) == AI_SUCCESS)
	{
		pMaterialResource->m_emissivePropertyGroup.m_factor = glm::vec3{ factor.r, factor.g , factor.b };
	}
	if (aiUVTransform transform; pMaterial->Get(AI_MATKEY_UVTRANSFORM(aiTextureType_EMISSION_COLOR, 0), transform) == AI_SUCCESS)
	{
		pMaterialResource->m_emissivePropertyGroup.m_offset = glm::vec2{ transform.mTranslation.x, transform.mTranslation.y };
		pMaterialResource->m_emissivePropertyGroup.m_scale = glm::vec2{ transform.mScaling.x, transform.mScaling.y };
		pMaterialResource->m_emissivePropertyGroup.m_rotation = transform.mRotation;
	}

	if (int twoSide; pMaterial->Get(AI_MATKEY_TWOSIDED, twoSide))
	{
		pMaterialResource->m_twoSide = (bool)twoSide;
	}

	std::string materialResourceName = path.data();
	materialResourceName += pMaterial->GetName().C_Str();
	sl::ResourceManager::AddMaterialResource(materialResourceName, std::move(pMaterialResource));

	return materialResourceName;
}

void ProcessMesh(const aiMesh *pMesh, const aiScene *pScene, std::string_view path)
{
	SL_ASSERT(pMesh->mPrimitiveTypes & aiPrimitiveType::aiPrimitiveType_TRIANGLE);
	SL_ASSERT(!(pMesh->mPrimitiveTypes & ~aiPrimitiveType::aiPrimitiveType_TRIANGLE));
	SL_ASSERT(pMesh->mVertices && pMesh->mNormals && pMesh->mTangents && pMesh->mBitangents);
	SL_ASSERT(pMesh->mTextureCoords[0] && pMesh->mNumUVComponents[0] == 2);

	const char *pMeshName = pMesh->mName.C_Str();
	SL_LOG_TRACE("\t\tMesh: {}", pMeshName);
	SL_LOG_TRACE("\t\t\tMaterial: {}", pScene->mMaterials[pMesh->mMaterialIndex]->GetName().C_Str());

	// 1. Vertex
	sl::VertexLayout layout
	{
		{ "Position", sl::AttribType::Float, 3 },
		{ "Normal", sl::AttribType::Float, 3 },
		{ "Tangent", sl::AttribType::Float, 3 },
		{ "UV0", sl::AttribType::Float, 2 },
	};
	uint32_t vertexCount = pMesh->mNumVertices;
	uint32_t floatCount = layout.GetStride() / sizeof(float);

	std::vector<float> vertices;
	vertices.resize(vertexCount * floatCount);
	for (size_t i = 0; i < vertexCount; ++i)
	{
		memcpy(&vertices[i * floatCount] + 0, &pMesh->mVertices[i], 3 * sizeof(float));
		memcpy(&vertices[i * floatCount] + 3, &pMesh->mNormals[i], 3 * sizeof(float));
		memcpy(&vertices[i * floatCount] + 6, &pMesh->mTangents[i], 3 * sizeof(float));
		memcpy(&vertices[i * floatCount] + 9, &pMesh->mTextureCoords[0][i], 2 * sizeof(float));
		// TODO: Multi chanel UV.
	}

	// 2. Index
	uint32_t indexCount = pMesh->mNumFaces;

	std::vector<uint32_t> indices;
	indices.resize(indexCount * 3);
	for (size_t i = 0; i < indexCount; ++i)
	{
		SL_ASSERT(pMesh->mFaces[i].mNumIndices == 3);

		memcpy(&indices[i * 3], pMesh->mFaces[i].mIndices, 3 * sizeof(uint32_t));
	}

	// 3. Mesh resource
	auto pMeshResource = std::make_unique<sl::MeshResource>("TODO");
	pMeshResource->m_state = sl::ResourceState::Uploading;
	pMeshResource->m_verticesRowData = std::move(vertices);
	pMeshResource->m_indicesRowData = std::move(indices);
	pMeshResource->m_layout = std::move(layout);

	std::string MeshResourceName = path.data();
	MeshResourceName += pMeshName;
	sl::ResourceManager::AddMeshResource(MeshResourceName, std::move(pMeshResource));

	// 4. Material resource
	std::string materialResourceName = ProcessMaterial(pScene->mMaterials[pMesh->mMaterialIndex], path);

	// 5. Entity and component
	sl::Entity entity = sl::ECSWorld::CreateEntity(pMeshName);
	auto &rendering = entity.AddComponent<sl::RenderingComponent>();
	rendering.m_optMeshResourceName = std::move(MeshResourceName);
	rendering.m_optMaterialResourceName = std::move(materialResourceName);

	// TODO: AABB
}

void ProcessNode(const aiNode *pNode, const aiScene *pScene, std::string_view path)
{
	SL_LOG_TRACE("\tNode: {}", pNode->mName.C_Str());
	SL_LOG_TRACE("\t\tParent: {}", pNode->mParent ? pNode->mParent->mName.C_Str() : "nullptr");

	uint32_t meshCount = pNode->mNumMeshes;
	for (size_t i = 0; i < meshCount; ++i)
	{
		int meshIndex = pNode->mMeshes[i];
		const aiMesh *pMesh = pScene->mMeshes[meshIndex];
		ProcessMesh(pMesh, pScene, path);
	}

	uint32_t childCount = pNode->mNumChildren;
	for (size_t i = 0; i < childCount; ++i)
	{
		const aiNode *pChildNode = pNode->mChildren[i];
		ProcessNode(pChildNode, pScene, path);
	}
}

} // namespace

void ModelImporter::Import(uint32_t entity, std::string_view path)
{
	// TODO: `entity` parameter for hierarchy.

	SL_PROFILE;
	SL_LOG_TRACE("Importing model: \"{}\"", path.data());

	constexpr int ImportFlags = aiProcess_Triangulate |
		aiProcess_GenSmoothNormals | aiProcess_CalcTangentSpace | aiProcess_GenUVCoords | aiProcess_GenBoundingBoxes |
		aiProcess_FindInvalidData | aiProcess_FixInfacingNormals | aiProcess_DropNormals |
		aiProcess_FindDegenerates | aiProcess_SortByPType |
		aiProcess_JoinIdenticalVertices | aiProcess_RemoveRedundantMaterials | aiProcess_FindInstances |
		aiProcess_ImproveCacheLocality | aiProcess_OptimizeMeshes |
		aiProcess_PreTransformVertices;

	Assimp::Importer importer;
	importer.SetPropertyInteger(AI_CONFIG_PP_SBP_REMOVE, aiPrimitiveType::aiPrimitiveType_LINE | aiPrimitiveType::aiPrimitiveType_POINT);

	const aiScene *pScene = importer.ReadFile(path.data(), ImportFlags);
	if (!pScene || !pScene->mRootNode || pScene->mFlags & AI_SCENE_FLAGS_INCOMPLETE)
	{
		SL_LOG_ERROR("Failed to importing model: \"{}\"!\n\t{}", path.data(), importer.GetErrorString());
		return;
	}

	SL_LOG_TRACE("\tScene name: {}", pScene->mName.C_Str());
	SL_LOG_TRACE("\tMesh count: {}", pScene->mNumMeshes);
	SL_LOG_TRACE("\tMaterial count: {}", pScene->mNumMaterials);
	SL_LOG_TRACE("\tAnimation count: {}", pScene->mNumTextures);
	SL_LOG_TRACE("\tTexture count: {}", pScene->mNumLights);
	SL_LOG_TRACE("\tLight count: {}", pScene->mNumCameras);
	SL_LOG_TRACE("\tCamera count: {}", pScene->mNumSkeletons);

	ProcessNode(pScene->mRootNode, pScene, path);

#if !defined(SL_FINAL)
	for (size_t i = 0; i < pScene->mNumMeshes; ++i)
	{
		aiMesh *pMesh = pScene->mMeshes[i];
		SL_LOG_TRACE("\tMesh: {}", pMesh->mName.C_Str());
		SL_LOG_TRACE("\t\tVertex count: {}", pMesh->mNumVertices);
		SL_LOG_TRACE("\t\tIndex count: {}", pMesh->mNumFaces * 3);
	}

	for (size_t i = 0; i < pScene->mNumMaterials; ++i)
	{
		aiMaterial *pMaterial = pScene->mMaterials[i];
		SL_LOG_TRACE("\tMaterial: {}", pMaterial->GetName().C_Str());

		if (aiString texture; pMaterial->Get(AI_MATKEY_TEXTURE(aiTextureType_BASE_COLOR, 0), texture) == AI_SUCCESS)
		{
			SL_LOG_TRACE("\t\tAlbedo: {}", texture.C_Str());
		}
		if (aiString texture; pMaterial->Get(AI_MATKEY_TEXTURE(aiTextureType_NORMALS, 0), texture) == AI_SUCCESS)
		{
			SL_LOG_TRACE("\t\tNormal: {}", texture.C_Str());
		}
		if (aiString texture; pMaterial->Get(AI_MATKEY_TEXTURE(aiTextureType_METALNESS, 0), texture) == AI_SUCCESS)
		{
			SL_LOG_TRACE("\t\tMetallic: {}", texture.C_Str());
		}
		if (aiString texture; pMaterial->Get(AI_MATKEY_TEXTURE(aiTextureType_DIFFUSE_ROUGHNESS, 0), texture) == AI_SUCCESS)
		{
			SL_LOG_TRACE("\t\tRoughness: {}", texture.C_Str());
		}
		if (aiString texture; pMaterial->Get(AI_MATKEY_TEXTURE(aiTextureType_AMBIENT_OCCLUSION, 0), texture) == AI_SUCCESS)
		{
			SL_LOG_TRACE("\t\tOcclusion: {}", texture.C_Str());
		}
		if (aiString texture; pMaterial->Get(AI_MATKEY_TEXTURE(aiTextureType_EMISSION_COLOR, 0), texture) == AI_SUCCESS)
		{
			SL_LOG_TRACE("\t\tEmission: {}", texture.C_Str());
		}
	}

	for (size_t i = 0; i < pScene->mNumTextures; ++i)
	{
		SL_LOG_TRACE("\tEmbedded Texture: {}", pScene->mTextures[i]->mFilename.C_Str());
	}
#endif
}

} // namespace sl
