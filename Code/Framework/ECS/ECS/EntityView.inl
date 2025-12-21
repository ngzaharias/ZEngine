#pragma once

template<typename ...TRequired, typename ...TOptional>
ecs::EntityView_t<TypeList<TRequired...>, TypeList<TOptional...>>::EntityView_t(const ecs::Entity& entity, ecs::EntityWorld& world)
	: m_Entity(entity)
	, m_World(world)
{
};

template<typename ...TRequired, typename ...TOptional>
template<typename TComponent>
auto ecs::EntityView_t<TypeList<TRequired...>, TypeList<TOptional...>>::ReadRequired() const -> const TComponent&
{
	return m_World.ReadComponent<TComponent>(m_Entity);
}

template<typename ...TRequired, typename ...TOptional>
template<typename TComponent>
auto ecs::EntityView_t<TypeList<TRequired...>, TypeList<TOptional...>>::WriteRequired() -> TComponent&
{
	return m_World.WriteComponent<TComponent>(m_Entity);
}

template<typename ...TRequired, typename ...TOptional>
template<typename TComponent>
auto ecs::EntityView_t<TypeList<TRequired...>, TypeList<TOptional...>>::ReadOptional() const -> const TComponent*
{
	return m_World.HasComponent<TComponent>(m_Entity)
		? &m_World.ReadComponent<TComponent>(m_Entity)
		: nullptr;
}

template<typename ...TRequired, typename ...TOptional>
template<typename TComponent>
auto ecs::EntityView_t<TypeList<TRequired...>, TypeList<TOptional...>>::WriteOptional() -> TComponent*
{
	return m_World.HasComponent<TComponent>(m_Entity)
		? &m_World.WriteComponent<TComponent>(m_Entity)
		: nullptr;
}