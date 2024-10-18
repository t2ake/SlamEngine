#include "SandboxLayer.h"

#include "Core/Path.hpp"
#include "RenderCore/RenderCore.h"
#include "Resource/ModelImporter.h"
#include "Resource/ResourceManager.h"
#include "Scene/ECSWorld.h"
#include "Utils/ProfilerCPU.h"

#include <glm/gtc/matrix_transform.hpp>

SandboxLayer::SandboxLayer()
{
	SL_PROFILE;

	auto &transform = sl::ECSWorld::GetMainCameraEntity().GetComponents<sl::TransformComponent>();
	transform.m_position = glm::vec3{ 0.0f, 0.0f, 5.0f };
	transform.m_rotation = glm::vec3{ 0.0f, -90.0f, 0.0f };
	
	auto entity = sl::ECSWorld::CreateEntity("Square Entity");
	auto &rendering = entity.AddComponent<sl::RenderingComponent>();

	std::vector<float> vertices =
	{
		// position,        normal,           tangent,          uv0
		-0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		 0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
		 0.5f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
		-0.5f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
	};
	std::vector<uint32_t> indices = { 0, 1, 3, 1, 2, 3 };

	auto pMeshResource = std::make_unique<sl::MeshResource>("TODO");
	pMeshResource->SetStatus(sl::ResourceStatus::Uploading);
	pMeshResource->SetVertexRowData(std::move(vertices));
	pMeshResource->SetIndexRowData(std::move(indices));
	pMeshResource->SetLayout(sl::VertexLayout
	{
		{ "Position", sl::AttribType::Float, 3 },
		{ "Normal", sl::AttribType::Float, 3 },
		{ "Tangent", sl::AttribType::Float, 3 },
		{ "UV0", sl::AttribType::Float, 2 },
	});

	sl::ResourceManager::AddMeshResource("SquareMesh", std::move(pMeshResource));
	rendering.m_optMeshResourceName = "SquareMesh";

	auto pTextureResource = std::make_unique<sl::TextureResource>(
		sl::Path::FromeAsset("Texture/jc.png"), SL_SAMPLER_REPEAT | SL_SAMPLER_TRILINEAR);
	sl::ResourceManager::AddTextureResource("JCTexture", std::move(pTextureResource));

	auto pMaterialResource = std::make_unique<sl::MaterialResource>("TODO");
	pMaterialResource->m_albedoPropertyGroup.m_texture = "JCTexture";
	pMaterialResource->m_albedoPropertyGroup.m_useTexture = true;
	sl::ResourceManager::AddMaterialResource("AlbedoMaterial", std::move(pMaterialResource));
	rendering.m_optMaterialResourceName = "AlbedoMaterial";

	sl::ModelImporter::Import(0, "D:/Works/Model/venice_mask/scene.gltf");
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
