#pragma once

#include "ECS/Entity.h"
#include "ECS/QueryRegistry.h"

template <typename... TTypes>
ecs::WorldView<TTypes...>::WorldView(ecs::EntityWorld& entityWorld)
	: m_EntityWorld(entityWorld)
	, m_QueryRegistry(entityWorld.m_QueryRegistry)
{
}

template <typename... TTypes>
template <typename... TOthers>
ecs::WorldView<TTypes...>::operator ecs::WorldView<TOthers...>&()
{
	return reinterpret_cast<ecs::WorldView<TOthers...>&>(*this);
}

template <typename... TTypes>
bool ecs::WorldView<TTypes...>::IsAlive(const Entity& entity) const
{
	return m_EntityWorld.IsAlive(entity);
}

template <typename... TTypes>
inline auto ecs::WorldView<TTypes...>::CreateEntity()->ecs::Entity
{
	return m_EntityWorld.CreateEntity();
}

template <typename... TTypes>
inline void ecs::WorldView<TTypes...>::DestroyEntity(const Entity& entity)
{
	m_EntityWorld.DestroyEntity(entity);
}

template <typename... TTypes>
template <typename TComponent, typename... TArgs>
auto ecs::WorldView<TTypes...>::AddComponent(const Entity& entity, TArgs&&... args)->decltype(auto)
{
	static_assert(core::Contains<TComponent, TTypes...>(), "Component isn't present in WorldView.");

	return m_EntityWorld.template AddComponent<TComponent>(entity, std::forward<TArgs>(args)...);
}

template <typename... TTypes>
template<class TComponent>
void ecs::WorldView<TTypes...>::RemoveComponent(const Entity& entity)
{
	static_assert(core::Contains<TComponent, TTypes...>(), "Component isn't present in WorldView.");

	m_EntityWorld.template RemoveComponent<TComponent>(entity);
}

template <typename... TTypes>
template<class TComponent>
bool ecs::WorldView<TTypes...>::HasComponent(const Entity& entity, const bool alive /*= true*/) const
{
	static_assert(core::Contains<TComponent, TTypes...>(), "Component isn't present in WorldView.");

	return m_EntityWorld.template HasComponent<TComponent>(entity, alive);
}

template <typename... TTypes>
template<class TComponent>
auto ecs::WorldView<TTypes...>::ReadComponent(const Entity& entity, const bool alive /*= true*/)->const TComponent&
{
	static_assert(core::Contains<TComponent, TTypes...>(), "Component isn't present in WorldView.");

	return m_EntityWorld.template ReadComponent<TComponent>(entity, alive);
}

template <typename... TTypes>
template<class TComponent>
auto ecs::WorldView<TTypes...>::WriteComponent(const Entity& entity, const bool alive /*= true*/)->TComponent&
{
	static_assert(core::Contains<TComponent, TTypes...>(), "Component isn't present in WorldView.");
	// #todo: check write access

	return m_EntityWorld.template WriteComponent<TComponent>(entity, alive);
}

template <typename... TTypes>
template <typename TEvent, typename... TArgs>
auto ecs::WorldView<TTypes...>::AddEvent(TArgs&&... args)->decltype(auto)
{
	static_assert(core::Contains<TEvent, TTypes...>(), "Event isn't present in WorldView.");

	return m_EntityWorld.template AddEvent<TEvent>(std::forward<TArgs>(args)...);
}

template <typename... TTypes>
template<class TComponent>
auto ecs::WorldView<TTypes...>::ReadSingleton()->const TComponent&
{
	static_assert(core::Contains<TComponent, TTypes...>(), "Component isn't present in WorldView.");

	return m_EntityWorld.template ReadSingleton<TComponent>();
}

template <typename... TTypes>
template<class TComponent>
auto ecs::WorldView<TTypes...>::WriteSingleton()->TComponent&
{
	static_assert(core::Contains<TComponent, TTypes...>(), "Component isn't present in WorldView.");

	return m_EntityWorld.template WriteSingleton<TComponent>();
}

template <typename... TTypes>
template<class TResource>
auto ecs::WorldView<TTypes...>::ReadResource()->const TResource&
{
	static_assert(core::Contains<TResource, TTypes...>(), "Resource isn't present in WorldView.");

	return m_EntityWorld.template ReadResource<TResource>();
}

template <typename... TTypes>
template<class TResource>
auto ecs::WorldView<TTypes...>::WriteResource()->TResource&
{
	static_assert(core::Contains<TResource, TTypes...>(), "Resource isn't present in WorldView.");

	return m_EntityWorld.template WriteResource<TResource>();
}

template <typename... TTypes>
template<class TType>
auto ecs::WorldView<TTypes...>::HasAny()-> bool
{
	Z_PANIC(m_EntityWorld.IsInitialised(), "EntityWorld hasn't been initialised!");

	if constexpr (std::is_base_of<ecs::Event<TType>, TType>::value)
	{
		return !m_EntityWorld.m_EntityStorage.GetEvents<TType>().IsEmpty();
	}
	else
	{
		static const ecs::QueryId queryId = ecs::QueryProxy<TType>::Id();
		const ecs::QueryGroup& queryGroup = m_QueryRegistry.GetGroup(queryId);
		return !queryGroup.IsEmpty();
	}
}

template <typename... TTypes>
template<class TEvent>
auto ecs::WorldView<TTypes...>::Events()-> const Array<TEvent>&
{
	Z_PANIC(m_EntityWorld.IsInitialised(), "EntityWorld hasn't been initialised!");
	return m_EntityWorld.m_EntityStorage.GetEvents<TEvent>();
}

template <typename... TTypes>
template<class TQuery>
auto ecs::WorldView<TTypes...>::Query()-> const Set<Entity>&
{
	Z_PANIC(m_EntityWorld.IsInitialised(), "EntityWorld hasn't been initialised!");

	static const ecs::QueryId queryId = ecs::QueryProxy<TQuery>::Id();
	const ecs::QueryGroup& queryGroup = m_QueryRegistry.GetGroup(queryId);
	return queryGroup;
}