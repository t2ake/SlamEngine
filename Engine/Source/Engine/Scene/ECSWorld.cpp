#include "ECSWorld.h"

namespace sl
{

Entity ECSWorld::CreateEntity(std::string name)
{
	Entity entity{ m_registry.create() };
	entity.AddComponent<TagComponent>(name);
	entity.AddComponent<TransformComponent>();

	return entity;
}

Entity ECSWorld::GetMainCameraEntity()
{
	auto group = m_registry.group<CameraComponent>();
	for (auto entity : group)
	{
		if (group.get<CameraComponent>(entity).m_isMainCamera)
		{
			return entity;
		}
	}

	return {};
}

CameraComponent &ECSWorld::GetMainCameraComponent()
{
	return GetMainCameraEntity().GetComponents<CameraComponent>();
}

TransformComponent &ECSWorld::GetMainCameraTransformComponent()
{
	return GetMainCameraEntity().GetComponents<TransformComponent>();
}

void Entity::Destroy()
{
	if (!IsValid())
	{
		return;
	}

	// Destroy this entity and all its components.
	SL_LOG_TRACE("Destroy Entity: \"{}\"", GetComponents<sl::TagComponent>().m_name);
	ECSWorld::m_registry.destroy(m_handle);
	m_handle = entt::null;
}

} // namespace sl
