#pragma once

template<typename ...TRequired, typename ...TOptional>
ecs::EntityView_t<TypeList<TRequired...>, TypeList<TOptional...>>::EntityView_t(const ecs::Entity& entity, ecs::EntityWorld& world)
	: m_Entity(entity)
	, m_World(world)
{
	m_Required = m_World.template GetComponentsForView<TRequired...>(m_Entity);
	m_Optional = m_World.template TryComponentsForView<TOptional...>(m_Entity);
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
	m_World.m_FrameBuffer.UpdateComponent<TComponent>(m_Entity);
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
		m_World.m_FrameBuffer.UpdateComponent<TComponent>(m_Entity);
	return component;
}