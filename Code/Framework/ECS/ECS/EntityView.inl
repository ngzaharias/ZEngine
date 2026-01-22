#pragma once

template<typename ...TRequired, typename ...TOptional>
ecs::EntityView_t<TypeList<TRequired...>, TypeList<TOptional...>>::EntityView_t(ecs::EntityWorld& world, const ecs::Entity& entity)
	: m_World(world)
	, m_Entity(entity)
	, m_Required(m_World.template GetComponentsForView<TRequired...>(m_Entity))
	, m_Optional(m_World.template TryComponentsForView<TOptional...>(m_Entity))
{
}

template<typename ...TRequired, typename ...TOptional>
template<typename TOtherRequired, typename TOtherOptional>
ecs::EntityView_t<TypeList<TRequired...>, TypeList<TOptional...>>::EntityView_t(const EntityView_t<TOtherRequired, TOtherOptional>& rhs)
	: m_World(rhs.m_World)
	, m_Entity(rhs.m_Entity)
{
	((std::get<TRequired*>(m_Required) = std::get<TRequired*>(rhs.m_Required)), ...);
	((std::get<TOptional*>(m_Optional) = std::get<TOptional*>(rhs.m_Optional)), ...);
}

template<typename ...TRequired, typename ...TOptional>
template<typename TComponent>
auto ecs::EntityView_t<TypeList<TRequired...>, TypeList<TOptional...>>::ReadRequired() const -> const TComponent&
{
	return *std::get<TComponent*>(m_Required);
}

template<typename ...TRequired, typename ...TOptional>
template<typename TComponent>
auto ecs::EntityView_t<TypeList<TRequired...>, TypeList<TOptional...>>::WriteRequired() -> TComponent&
{
	m_World.m_EntityStorage.m_MainBuffer.UpdateComponent<TComponent>(m_Entity);
	return *std::get<TComponent*>(m_Required);
}

template<typename ...TRequired, typename ...TOptional>
template<typename TComponent>
bool ecs::EntityView_t<TypeList<TRequired...>, TypeList<TOptional...>>::HasOptional() const
{
	return std::get<TComponent*>(m_Optional);
}

template<typename ...TRequired, typename ...TOptional>
template<typename TComponent>
auto ecs::EntityView_t<TypeList<TRequired...>, TypeList<TOptional...>>::ReadOptional() const -> const TComponent*
{
	return std::get<TComponent*>(m_Optional);
}

template<typename ...TRequired, typename ...TOptional>
template<typename TComponent>
auto ecs::EntityView_t<TypeList<TRequired...>, TypeList<TOptional...>>::WriteOptional() -> TComponent*
{
	auto* component = std::get<TComponent*>(m_Optional);
	if (component)
		m_World.m_EntityStorage.m_MainBuffer.UpdateComponent<TComponent>(m_Entity);
	return component;
}