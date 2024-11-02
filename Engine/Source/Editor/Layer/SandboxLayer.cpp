#include "SandboxLayer.h"

#include "Core/Path.hpp"
#include "RenderCore/RenderCore.h"
#include "Resource/ModelImporter.h"
#include "Resource/ResourceManager.h"
#include "Scene/ECSWorld.h"
#include "Utils/ProfilerCPU.h"

SandboxLayer::SandboxLayer()
{
	SL_PROFILE;

	// Just some temporary code here.
	auto &transform = sl::ECSWorld::GetMainCameraEntity().GetComponents<sl::TransformComponent>();
	transform.m_position = glm::vec3{ 0.0f, 3.3f, 12.0f };
	transform.m_rotation = glm::vec3{ 0.0f, glm::radians(-90.0f), 0.0f};

	sl::ModelImporter importer("D:/Works/Model/slum_house/scene.gltf");
	importer.SetGlobalScale(glm::vec3{ 1.0f });
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
