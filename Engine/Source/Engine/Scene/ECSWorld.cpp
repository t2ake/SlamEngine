#include "ECSWorld.h"

namespace sl
{

entt::registry ECSWorld::m_registry;
Entity ECSWorld::m_editorCameraEntity;

Entity ECSWorld::CreateEntity(std::string name)
{
	Entity entity{ m_registry.create() };
	entity.AddComponent<TagComponent>(std::move(name));
	entity.AddComponent<TransformComponent>();

	return entity;
}

void ECSWorld::DestroyEntity(Entity entity)
{
	if (!entity)
	{
		return;
	}
	if (entity.TryGetComponent<sl::CornerstoneComponent>())
	{
		SL_ENGINE_WARN("Attempt to destroy entity \"{}\" with Cornerstone component!", entity.GetComponent<sl::TagComponent>().m_name);
		return;
	}

	SL_ENGINE_TRACE("Destroy Entity: \"{}\"", entity.GetComponent<sl::TagComponent>().m_name);
	m_registry.destroy(entity);
	entity.Reset();
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

} // namespace sl
