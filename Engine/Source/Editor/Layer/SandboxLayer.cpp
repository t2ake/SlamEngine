#include "SandboxLayer.h"

#include "Core/Path.hpp"
#include "RenderCore/RenderCore.h"
#include "Resource/ModelImporter.h"
#include "Resource/ResourceManager.h"
#include "Scene/ECSWorld.h"
#include "Utils/ProfilerCPU.h"

#include <glm/glm.hpp>

SandboxLayer::SandboxLayer()
{
	SL_PROFILE;

	// Just some temporary code here.
	auto &transform = sl::ECSWorld::GetMainCameraEntity().GetComponents<sl::TransformComponent>();
	transform.m_position = glm::vec3{ 0.0f, 3.3f, 12.0f };
	transform.m_rotation = glm::vec3{ 0.0f, glm::radians(-90.0f), 0.0f};
	
	// auto entity = sl::ECSWorld::CreateEntity("Logo Entity");
	// auto &rendering = entity.AddComponent<sl::RenderingComponent>();
	// 
	// std::vector<float> vertices =
	// {
	// 	// position,        normal,           tangent,          uv0
	// 	-0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
	// 	 0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
	// 	 0.5f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
	// 	-0.5f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
	// };
	// std::vector<uint32_t> indices = { 0, 1, 3, 1, 2, 3 };
	// 
	// auto pMeshResource = std::make_unique<sl::MeshResource>();
	// pMeshResource->m_verticesRowData = std::move(vertices);
	// pMeshResource->m_indicesRowData = std::move(indices);
	// pMeshResource->m_layout = sl::VertexLayout
	// {
	// 	{ "Position", sl::AttribType::Float, 3 },
	// 	{ "Normal", sl::AttribType::Float, 3 },
	// 	{ "Tangent", sl::AttribType::Float, 3 },
	// 	{ "UV0", sl::AttribType::Float, 2 },
	// };
	// pMeshResource->m_vertexCount = 4;
	// pMeshResource->m_indexCount = 6;
	// 
	// sl::ResourceManager::AddMeshResource("SquareMesh", std::move(pMeshResource));
	// rendering.m_optMeshResourceName = "SquareMesh";
	// 
	// auto pTextureResource = std::make_unique<sl::TextureResource>(
	// 	sl::Path::FromeAsset("Texture/Logo.png"), SL_SAMPLER_REPEAT | SL_SAMPLER_TRILINEAR);
	// sl::ResourceManager::AddTextureResource("LogoTexture", std::move(pTextureResource));
	// 
	// auto pMaterialResource = std::make_unique<sl::MaterialResource>();
	// pMaterialResource->m_albedoPropertyGroup.m_texture = "LogoTexture";
	// pMaterialResource->m_albedoPropertyGroup.m_useTexture = true;
	// sl::ResourceManager::AddMaterialResource("AlbedoMaterial", std::move(pMaterialResource));
	// rendering.m_optMaterialResourceName = "AlbedoMaterial";

	sl::ModelImporter importer("D:/Works/Model/venice_mask/scene.gltf");
	importer.SetGlobalScale(glm::vec3{ 100.0f });
	importer.Import();
}

SandboxLayer::~SandboxLayer()
{

}

void SandboxLayer::OnAttach()
{

}

void SandboxLayer::OnDetach()
{

}

void SandboxLayer::OnEvent(sl::Event &event)
{

}

void SandboxLayer::BeginFrame()
{

}

void SandboxLayer::OnUpdate(float deltaTime)
{

}

void SandboxLayer::OnRender()
{

}

void SandboxLayer::EndFrame()
{

}
