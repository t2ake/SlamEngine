#pragma once

#include "Core/Log.h"
#include "Scene/CameraComponent.h"
#include "Scene/CornerstoneComponent.h"
#include "Scene/RenderingComponent.h"
#include "Scene/TagComponent.h"
#include "Scene/TransformComponent.h"

#include <entt/entt.hpp>

namespace sl
{

class ECSWorld final
{
	friend class Entity;

public:
	ECSWorld() = delete;
	ECSWorld(const ECSWorld &) = delete;
	ECSWorld &operator=(const ECSWorld &) = delete;
	ECSWorld(ECSWorld &&) = delete;
	ECSWorld &operator=(ECSWorld &&) = delete;
	~ECSWorld() = delete;

	static Entity CreateEntity(std::string name = "Empty Entity");
	static entt::registry &GetRegistry() { return m_registry; }

	static Entity GetMainCameraEntity();
	static CameraComponent &GetMainCameraComponent();

private:
	inline static entt::registry m_registry;
};

// Basically, Entity is just a tool class that allows us to use ECS more intuitively.
class Entity final
{
public:
	Entity() = default;
	Entity(entt::entity entity) : m_handle(entity) {}
	Entity(uint32_t entity) : m_handle((entt::entity)entity) {}

	entt::entity GetHandle() const { return m_handle; }
	bool IsValid() const { return ECSWorld::m_registry.valid(m_handle); }
	void Reset() { m_handle = entt::null; }
	void Destroy();

	// Returns reference.
	template<class T, class... Args>
	decltype(auto) AddComponent(Args&&... args)
	{
		SL_ASSERT(!HasAllComponentsOf<T>(), "Entity already holds component!");
		return ECSWorld::m_registry.emplace<T>(m_handle, std::forward<Args>(args)...);
	}

	// Returns reference.
	template<class T, class... Args>
	decltype(auto) ReplaceComponent(Args&&... args)
	{
		SL_ASSERT(HasAllComponentsOf<T>(), "Entity does not hold component!");
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
	decltype(auto) GetComponents()
	{
		SL_ASSERT(HasAllComponentsOf<T...>(),
			(sizeof...(T) == 1U) ? "Entity does not hold component!" : "Entity does not hold components!");
		return ECSWorld::m_registry.get<T...>(m_handle);
	}

	// Returns tuple of pointers.
	template<class... T>
	auto TryGetComponents()
	{
		return ECSWorld::m_registry.try_get<T...>(m_handle);
	}

	template<class... T>
	bool HasAllComponentsOf()
	{
		return (ECSWorld::m_registry.all_of<T...>(m_handle));
	}

	template<class... T>
	bool HasAnyComponentsOf()
	{
		return (ECSWorld::m_registry.any_of<T...>(m_handle));
	}

	bool HasAnyComponent()
	{
		return !ECSWorld::m_registry.orphan(m_handle);
	}

	template<class T>
	auto RemoveComponent()
	{
		// `registry::remove` is safer than `registry::erase`.
		return ECSWorld::m_registry.remove<T>(m_handle);
	}

	template<class T>
	auto EraseComponent()
	{
		// `registry::remove` is safer than `registry::erase`.
		return ECSWorld::m_registry.erase<T>(m_handle);
	}

	bool operator==(const Entity &other) const { return m_handle == other.m_handle; }
	bool operator!=(const Entity &other) const { return !(operator==(other)); }

private:
	entt::entity m_handle = entt::null;
};

} // namespace sl

inline bool operator==(const sl::Entity &entity, const entt::entity &other) { return entity.GetHandle() == other; }
inline bool operator!=(const sl::Entity &entity, const entt::entity &other) { return !operator==(entity, other); }
inline bool operator==(const entt::entity &other, const sl::Entity &entity) { return other == entity.GetHandle(); }
inline bool operator!=(const entt::entity &other, const sl::Entity &entity) { return !operator==(other, entity); }

inline bool operator==(const sl::Entity &entity, const uint32_t &other) { return (uint32_t)entity.GetHandle() == other; }
inline bool operator!=(const sl::Entity &entity, const uint32_t &other) { return !operator==(entity, other); }
inline bool operator==(const uint32_t &other, const sl::Entity &entity) { return other == (uint32_t)entity.GetHandle(); }
inline bool operator!=(const uint32_t &other, const sl::Entity &entity) { return !operator==(other, entity); }
