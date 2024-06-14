#include "SandboxLayer.h"

#include "Core/Path.hpp"
#include "RenderCore/RenderCore.h"
#include "Resource/ResourceManager.h"
#include "Scene/ECSWorld.h"

#include <glm/gtc/matrix_transform.hpp>

SandboxLayer::SandboxLayer()
{
	auto &transform = sl::ECSWorld::GetEditorCameraEntity().GetComponent<sl::TransformComponent>();
	transform.m_position = glm::vec3{ 0.0f, 0.0f, 5.0f };
	transform.m_rotation = glm::vec3{ 0.0f, -90.0f, 0.0f };
	
	auto entity = sl::ECSWorld::CreateEntity("Test Entity");
	auto &rendering = entity.AddComponent<sl::RenderingComponent>();

	std::vector<float> vertices =
	{
		-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
		 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
		 0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
		-0.5f,  0.5f, 0.0f, 0.0f, 1.0f,
	};
	std::vector<uint32_t> indices = { 0, 1, 3, 1, 2, 3 };

	auto pMeshResource = std::make_unique<sl::MeshResource>("TODO");
	pMeshResource->SetLayout(sl::VertexLayout
	{
		{ "Position", sl::AttribType::Float, 3 },
		{ "UV", sl::AttribType::Float, 2 },
	});
	pMeshResource->SetVertexRowData(std::move(vertices));
	pMeshResource->SetIndexRowData(std::move(indices));
	sl::ResourceManager::AddMeshResource("SquareMesh", std::move(pMeshResource));
	rendering.m_optMeshResourceName = "SquareMesh";

	auto pTextureResource = std::make_unique<sl::TextureResource>(
		sl::Path::FromeAsset("Texture/jc.png"), SL_SAMPLER_REPEAT | SL_SAMPLER_TRILINEAR);
	sl::ResourceManager::AddTextureResource("JCTexture", std::move(pTextureResource));
	rendering.m_optTextureResourceName = "JCTexture";

	sl::Shader *pShader = sl::Shader::Creat("Base",
		sl::Path::FromeAsset("Shader/vs_Base.glsl"),
		sl::Path::FromeAsset("Shader/fs_Base.glsl"));
	sl::Shader *pIDShader = sl::Shader::Creat("EntityID",
		sl::Path::FromeAsset("Shader/vs_EntityID.glsl"),
		sl::Path::FromeAsset("Shader/fs_EntityID.glsl"));

	rendering.m_pShader = pShader;
	rendering.m_pIDShader = pIDShader;
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
