#include "ECSWorld.h"

namespace sl
{

entt::registry ECSWorld::m_registry;
Entity ECSWorld::m_editorCameraEntity;

Entity ECSWorld::CreateEntity(std::string name)
{
	Entity entity{ m_registry.create() };
	entity.AddComponent<TagComponent>(name);
	entity.AddComponent<TransformComponent>();

	return entity;
}

void ECSWorld::SetEditorCameraEntity(Entity entity)
{
	m_editorCameraEntity = entity;
}

Entity ECSWorld::GetEditorCameraEntity()
{
	return m_editorCameraEntity;
}

CameraComponent &ECSWorld::GetEditorCameraComponent()
{
	return m_editorCameraEntity.GetComponent<CameraComponent>();
}

void Entity::Destroy()
{
	if (!IsValid())
	{
		return;
	}
	if (TryGetComponent<sl::CornerstoneComponent>())
	{
		SL_LOG_WARN("Attempt to destroy entity \"{}\" with Cornerstone component!", GetComponent<sl::TagComponent>().m_name);
		return;
	}

	// Destroy this entity and all its components.
	SL_LOG_TRACE("Destroy Entity: \"{}\"", GetComponent<sl::TagComponent>().m_name);
	ECSWorld::m_registry.destroy(m_handle);
	m_handle = entt::null;
}

} // namespace sl
