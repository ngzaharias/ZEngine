#pragma once

#include <ECS/Entity.h>
#include <ECS/QueryRegistry.h>

template <typename... TObjects>
ecs::WorldView<TObjects...>::WorldView(ecs::EntityWorld& entityWorld)
	: m_EntityWorld(entityWorld)
	, m_QueryRegistry(entityWorld.m_QueryRegistry)
{
}

template <typename... TObjects>
bool ecs::WorldView<TObjects...>::IsAlive(const Entity& entity) const
{
	return m_EntityWorld.IsAlive(entity);
}

template <typename... TObjects>
inline auto ecs::WorldView<TObjects...>::CreateEntity()->ecs::Entity
{
	return m_EntityWorld.CreateEntity();
}

template <typename... TObjects>
inline void ecs::WorldView<TObjects...>::DestroyEntity(const Entity& entity)
{
	m_EntityWorld.DestroyEntity(entity);
}

template <typename... TTypes>
template<class TComponent>
bool ecs::WorldView<TTypes...>::HasComponent(const Entity& entity, const bool alive /*= true*/) const
{
	static_assert(!std::is_const_v<TComponent>, "Component cannot be const.");
	static_assert(!std::is_reference_v<TComponent>, "Component cannot be a reference.");
	static_assert(!std::is_pointer_v<TComponent>, "Component cannot be a pointer.");
	static_assert(core::Contains<TComponent, TTypes...>(), "Component isn't present in WorldView.");

	return m_EntityWorld.template HasComponent<TComponent>(entity, alive);
}

template <typename... TTypes>
template<class TComponent>
bool ecs::WorldView<TTypes...>::HasSingleton(const bool alive /*= true*/) const
{
	static_assert(!std::is_const_v<TComponent>, "Component cannot be const.");
	static_assert(!std::is_reference_v<TComponent>, "Component cannot be a reference.");
	static_assert(!std::is_pointer_v<TComponent>, "Component cannot be a pointer.");
	static_assert(core::Contains<TComponent, TTypes...>(), "Component isn't present in WorldView.");

	return m_EntityWorld.template HasSingleton<TComponent>(alive);
}

template <typename... TTypes>
template<class TComponent>
auto ecs::WorldView<TTypes...>::GetComponent(const Entity& entity, const bool alive /*= true*/)->TComponent&
{
	static_assert(!std::is_reference_v<TComponent>, "Component cannot be a reference.");
	static_assert(!std::is_pointer_v<TComponent>, "Component cannot be a pointer.");
	static_assert(core::Contains<TComponent, TTypes...>(), "Component isn't present in WorldView.");

	return m_EntityWorld.template GetComponent<TComponent>(entity, alive);
}

template <typename... TTypes>
template<class TComponent>
auto ecs::WorldView<TTypes...>::GetSingleton(const bool alive /*= true*/)->TComponent&
{
	static_assert(!std::is_reference_v<TComponent>, "Component cannot be a reference.");
	static_assert(!std::is_pointer_v<TComponent>, "Component cannot be a pointer.");
	static_assert(core::Contains<TComponent, TTypes...>(), "Component isn't present in WorldView.");

	return m_EntityWorld.template GetSingleton<TComponent>(alive);
}

template <typename... TTypes>
template <typename TComponent, typename... TArgs>
auto ecs::WorldView<TTypes...>::AddComponent(const Entity& entity, TArgs&&... args)->decltype(auto)
{
	static_assert(!std::is_const_v<TComponent>, "Component cannot be const.");
	static_assert(!std::is_reference_v<TComponent>, "Component cannot be a reference.");
	static_assert(!std::is_pointer_v<TComponent>, "Component cannot be a pointer.");
	static_assert(core::Contains<TComponent, TTypes...>(), "Component isn't present in WorldView.");

	return m_EntityWorld.template AddComponent<TComponent>(entity, std::forward<TArgs>(args)...);
}

template <typename... TTypes>
template <typename TComponent, typename... TArgs>
auto ecs::WorldView<TTypes...>::AddEventComponent(TArgs&&... args)->decltype(auto)
{
	static_assert(!std::is_const_v<TComponent>, "Component cannot be const.");
	static_assert(!std::is_reference_v<TComponent>, "Component cannot be a reference.");
	static_assert(!std::is_pointer_v<TComponent>, "Component cannot be a pointer.");
	static_assert(core::Contains<TComponent, TTypes...>(), "Component isn't present in WorldView.");

	return m_EntityWorld.template AddEventComponent<TComponent>(std::forward<TArgs>(args)...);
}

template <typename... TTypes>
template <typename TComponent, typename... TArgs>
auto ecs::WorldView<TTypes...>::AddSingleton(TArgs&&... args)->decltype(auto)
{
	static_assert(!std::is_const_v<TComponent>, "Component cannot be const.");
	static_assert(!std::is_reference_v<TComponent>, "Component cannot be a reference.");
	static_assert(!std::is_pointer_v<TComponent>, "Component cannot be a pointer.");
	static_assert(core::Contains<TComponent, TTypes...>(), "Component isn't present in WorldView.");

	return m_EntityWorld.template AddSingleton<TComponent>(std::forward<TArgs>(args)...);
}

template <typename... TTypes>
template<class TComponent>
void ecs::WorldView<TTypes...>::RemoveComponent(const Entity& entity)
{
	static_assert(!std::is_const_v<TComponent>, "Component cannot be const.");
	static_assert(!std::is_reference_v<TComponent>, "Component cannot be a reference.");
	static_assert(!std::is_pointer_v<TComponent>, "Component cannot be a pointer.");
	static_assert(core::Contains<TComponent, TTypes...>(), "Component isn't present in WorldView.");

	m_EntityWorld.template RemoveComponent<TComponent>(entity);
}

template <typename... TTypes>
template<class TComponent>
void ecs::WorldView<TTypes...>::RemoveSingleton()
{
	static_assert(!std::is_const_v<TComponent>, "Component cannot be const.");
	static_assert(!std::is_reference_v<TComponent>, "Component cannot be a reference.");
	static_assert(!std::is_pointer_v<TComponent>, "Component cannot be a pointer.");
	static_assert(core::Contains<TComponent, TTypes...>(), "Component isn't present in WorldView.");

	m_EntityWorld.template RemoveSingleton<TComponent>();
}

template <typename... TTypes>
template<class TManager>
auto ecs::WorldView<TTypes...>::GetManager()->TManager&
{
	static_assert(!std::is_reference_v<TManager>, "Manager cannot be a reference.");
	static_assert(!std::is_pointer_v<TManager>, "Manager cannot be a pointer.");
	static_assert(core::Contains<TManager, TTypes...>(), "Manager isn't present in WorldView.");

	return m_EntityWorld.template GetManager<TManager>();
}

template <typename... TObjects>
template<class TQuery>
auto ecs::WorldView<TObjects...>::HasAny()-> bool
{
	Z_ASSERT_CRASH(m_EntityWorld.IsInitialised(), "ecs::EntityWorld hasn't been initialised!");

	static const ecs::QueryId queryId = ecs::QueryProxy<TQuery>::Id();
	const ecs::QueryGroup& queryGroup = m_QueryRegistry.GetGroup(queryId);
	return !queryGroup.IsEmpty();
}

template <typename... TObjects>
template<class TQuery>
auto ecs::WorldView<TObjects...>::Query()-> const Set<Entity>&
{
	Z_ASSERT_CRASH(m_EntityWorld.IsInitialised(), "ecs::EntityWorld hasn't been initialised!");

	static const ecs::QueryId queryId = ecs::QueryProxy<TQuery>::Id();
	const ecs::QueryGroup& queryGroup = m_QueryRegistry.GetGroup(queryId);
	return queryGroup;
}