#include "Scene.h"

namespace sl
{

entt::registry Scene::m_registry;

Entity Scene::CreateEntity(std::string name)
{
	Entity entity{ m_registry.create() };
	entity.AddComponent<TagComponent>(std::move(name));
	entity.AddComponent<TransformComponent>();

	return entity;
}

} // namespace sl
