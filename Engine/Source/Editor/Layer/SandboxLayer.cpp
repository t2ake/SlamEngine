#include "SandboxLayer.h"

#include "Core/Path.hpp"
#include "RenderCore/RenderCore.h"
#include "Resource/ResourceManager.h"
#include "Scene/ECSWorld.h"
#include "Utils/ProfilerCPU.h"

#include <glm/gtc/matrix_transform.hpp>

SandboxLayer::SandboxLayer()
{
	SL_PROFILE;

	auto &transform = sl::ECSWorld::GetEditorCameraEntity().GetComponents<sl::TransformComponent>();
	transform.m_position = glm::vec3{ 0.0f, 0.0f, 5.0f };
	transform.m_rotation = glm::vec3{ 0.0f, -90.0f, 0.0f };
	
	auto entity = sl::ECSWorld::CreateEntity("Square Entity");
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

	auto pBaseShaderResource = std::make_unique<sl::ShaderResource>(
		sl::Path::FromeAsset("Shader/Base_vert.glsl"),
		sl::Path::FromeAsset("Shader/Base_frag.glsl")
	);
	sl::ResourceManager::AddShaderResource("BaseShader", std::move(pBaseShaderResource));
	rendering.m_optBaseShaderResourceName = "BaseShader";

	auto pIDShaderResource = std::make_unique<sl::ShaderResource>(
		sl::Path::FromeAsset("Shader/EntityID_vert.glsl"),
		sl::Path::FromeAsset("Shader/EntityID_frag.glsl")
	);
	sl::ResourceManager::AddShaderResource("IDShader", std::move(pIDShaderResource));
	rendering.m_optIDShaderResourceName = "IDShader";
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
