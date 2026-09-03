#pragma once

template<typename TComponent>
bool ecs::EntityStorage2::HasComponent(const ecs::Entity& entity) const
{
	Z_PANIC(m_EntityMap.Contains(entity), "");

	const int32 tableIndex = m_EntityMap.Get(entity);
	const ecs::EntityTable& table = m_Tables[tableIndex];

	const ecs::ComponentId componentId = ToTypeId<TComponent, ecs::ComponentTag>();
	return table.HasComponent(entity, componentId);
}

template<typename TComponent>
auto ecs::EntityStorage2::GetComponent(const ecs::Entity& entity) -> TComponent&
{
	Z_PANIC(m_EntityMap.Contains(entity), "");

	const int32 tableIndex = m_EntityMap.Get(entity);
	ecs::EntityTable& table = m_Tables[tableIndex];

	const ecs::ComponentId componentId = ToTypeId<TComponent, ecs::ComponentTag>();
	auto* component = table.GetComponent(entity, componentId);
	return *reinterpret_cast<TComponent*>(component);
}

template<typename TComponent>
auto ecs::EntityStorage2::TryComponent(const ecs::Entity& entity) -> TComponent*
{
	Z_PANIC(m_EntityMap.Contains(entity), "");

	const int32 tableIndex = m_EntityMap.Get(entity);
	ecs::EntityTable& table = m_Tables[tableIndex];

	const ecs::ComponentId componentId = ToTypeId<TComponent, ecs::ComponentTag>();
	auto* component = table.TryComponent(entity, componentId);
	return component
		? *static_cast<TComponent*>(component)
		: nullptr;
}