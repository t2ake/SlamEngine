#include "ECSWorld.h"

namespace sl
{

entt::registry ECSWorld::m_registry;
Entity ECSWorld::m_mainCameraEntity;

Entity ECSWorld::CreateEntity(std::string name)
{
	Entity entity{ m_registry.create() };
	entity.AddComponent<TagComponent>(std::move(name));
	entity.AddComponent<TransformComponent>();

	return entity;
}

void ECSWorld::SetMainCameraEntity(Entity entity)
{
	m_mainCameraEntity = entity;
}

CameraComponent &ECSWorld::GetMainCameraComponent()
{
	return m_mainCameraEntity.GetComponent<CameraComponent>();
}

} // namespace sl
