#pragma once

#include "Core/Log.h"
#include "Scene/CameraComponent.h"
#include "Scene/TagComponent.hpp"
#include "Scene/TransformComponent.hpp"

#include <entt/entt.hpp>

namespace sl
{

class Entity;

class ECSWorld final
{
	friend class Entity;

public:
	static entt::registry &GetRegistry() { return m_registry; }
	static Entity CreateEntity(std::string name = "Default Name");

	static void SetMainCameraEntity(Entity entity);
	static Entity GetMainCameraEntity();

private:
	static entt::registry m_registry;
	static Entity m_mainCameraEntity;
};

// Basically, Entity is just a tool class that allows us to use ECS more intuitively.
class Entity final
{
public:
	Entity() = default;
	Entity(entt::entity entity) : m_handle(entity) {}

	entt::entity GetHandle() const { return m_handle; }

	// Returns reference.
	template<class T, class... Args>
	decltype(auto) AddComponent(Args&&... args)
	{
		SL_ENGINE_ASSERT_INFO(!HasAllComponent<T>(), "Entity already holds component!");
		return ECSWorld::m_registry.emplace<T>(m_handle, std::forward<Args>(args)...);
	}

	// Returns reference.
	template<class T, class... Args>
	decltype(auto) ReplaceComponent(Args&&... args)
	{
		SL_ENGINE_ASSERT_INFO(HasAllComponent<T>(), "Entity does not hold component!");
		return ECSWorld::m_registry.replace<T>(m_handle, std::forward<Args>(args)...);
	}

	// Returns reference.
	template<class T, class... Args>
	decltype(auto) AddOrReplaceComponent(Args&&... args)
	{
		return ECSWorld::m_registry.emplace_or_replace<T>(m_handle, std::forward<Args>(args)...);
	}

	// Returns tuple of references.
	template<class... T>
	decltype(auto) GetComponent()
	{
		SL_ENGINE_ASSERT_INFO(HasAllComponent<T...>(), "Entity does not hold component!");
		return ECSWorld::m_registry.get<T...>(m_handle);
	}

	// Returns tuple of pointers.
	template<class... T>
	auto TryGetComponent()
	{
		return ECSWorld::m_registry.try_get<T...>(m_handle);
	}

	template<class... T>
	bool HasAllComponent()
	{
		return (ECSWorld::m_registry.all_of<T...>(m_handle));
	}

	template<class... T>
	bool HasAnyComponent()
	{
		return (ECSWorld::m_registry.any_of<T...>(m_handle));
	}

	bool HasComponent()
	{
		return !ECSWorld::m_registry.orphan(m_handle);
	}

	// 'remove' is safer than 'erase'.
	template<class T>
	auto RemoveComponent()
	{
		return ECSWorld::m_registry.remove<T>(m_handle);
	}

	// Attempting to erase a component from an entity that doesn't own it results in undefined behavior.
	template<class T>
	void EraseComponent()
	{
		ECSWorld::m_registry.erase<T>(m_handle);
	}

	// Destroys an entity and all its components.
	void Destroy()
	{
		ECSWorld::m_registry.destroy(m_handle);
		m_handle = entt::null;
	}

	operator bool() const { return entt::null != m_handle; }
	bool operator==(const Entity &other) const { return other.m_handle == m_handle; }
	bool operator!=(const Entity &other) const { return !(operator==(other)); }

private:
	entt::entity m_handle = entt::null;
};

} // namespace sl
