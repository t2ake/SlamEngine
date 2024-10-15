#include "ModelImporter.h"

#include "Core/Log.h"
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

void ProcessMesh(const aiMesh *pMesh)
{
	SL_ASSERT(pMesh->mPrimitiveTypes & aiPrimitiveType::aiPrimitiveType_TRIANGLE);
	SL_ASSERT(!(pMesh->mPrimitiveTypes & ~aiPrimitiveType::aiPrimitiveType_TRIANGLE));
	SL_ASSERT(pMesh->mVertices && pMesh->mNormals && pMesh->mTangents && pMesh->mBitangents);
	SL_ASSERT(pMesh->mTextureCoords[0] && pMesh->mNumUVComponents[0] == 2);

	// 1. Vertex
	sl::VertexLayout layout
	{
		{ "Position", sl::AttribType::Float, 3 },
		{ "Normal", sl::AttribType::Float, 3 },
		{ "Tangent", sl::AttribType::Float, 3 },
		{ "UV0", sl::AttribType::Float, 2 },
	};
	uint32_t vertexCount = pMesh->mNumVertices;

	std::vector<float> vertices;
	vertices.resize(vertexCount * layout.GetStride() / sizeof(float));
	for (size_t i = 0; i < vertexCount; ++i)
	{
		memcpy(&vertices[i] + 0, &pMesh->mVertices[i], 3 * sizeof(float));
		memcpy(&vertices[i] + 3, &pMesh->mNormals[i], 3 * sizeof(float));
		memcpy(&vertices[i] + 6, &pMesh->mTangents[i], 3 * sizeof(float));
		memcpy(&vertices[i] + 9, &pMesh->mTextureCoords[0][i], 2 * sizeof(float));
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
	pMeshResource->SetStatus(sl::ResourceStatus::Uploading);
	pMeshResource->SetVertexRowData(std::move(vertices));
	pMeshResource->SetIndexRowData(std::move(indices));
	pMeshResource->SetLayout(std::move(layout));
	sl::ResourceManager::AddMeshResource(pMesh->mName.C_Str(), std::move(pMeshResource));

	// 4. Entity and component
	sl::Entity entity = sl::ECSWorld::CreateEntity(pMesh->mName.C_Str());
	auto &rendering = entity.AddComponent<sl::RenderingComponent>();
	rendering.m_optMeshResourceName = pMesh->mName.C_Str();

	// 5. Log
	SL_LOG_TRACE("\t\tMesh name: {}", pMesh->mName.C_Str());
	SL_LOG_TRACE("\t\tVertex count: {}", vertexCount);
	SL_LOG_TRACE("\t\tIndex count: {}", indexCount);

	// TODO: AABB
}

void ProcessNode(const aiNode *pNode, const aiScene *pScene)
{
	SL_LOG_TRACE("\tNode name: {}", pNode->mName.C_Str());
	SL_LOG_TRACE("\tParent: {}", pNode->mParent ? pNode->mParent->mName.C_Str() : "nullptr");

	uint32_t meshCount = pNode->mNumMeshes;
	for (size_t i = 0; i < meshCount; ++i)
	{
		int meshIndex = pNode->mMeshes[i];
		const aiMesh *pMesh = pScene->mMeshes[meshIndex];
		ProcessMesh(pMesh);
	}
	SL_LOG_TRACE("");

	uint32_t childCount = pNode->mNumChildren;
	for (size_t i = 0; i < childCount; ++i)
	{
		const aiNode *pChildNode = pNode->mChildren[i];
		ProcessNode(pChildNode, pScene);
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
	SL_LOG_TRACE("");

	ProcessNode(pScene->mRootNode, pScene);
}

} // namespace sl
