#pragma once

template<typename TComponent>
bool ecs::EntityStorage::HasComponent(const ecs::Entity& entity) const
{
	const auto find = m_EntityMap.Find(entity);
	if (find == m_EntityMap.end())
		return false;

	const int32 tableIndex = find->second;
	const ecs::EntityTable& table = m_Tables[tableIndex];

	const ecs::ComponentId componentId = ToTypeId<TComponent, ecs::ComponentTag>();
	return table.HasComponent(entity, componentId);
}

template<typename TComponent>
auto ecs::EntityStorage::GetComponent(const ecs::Entity& entity) -> TComponent&
{
	Z_PANIC(m_EntityMap.Contains(entity), "");

	const int32 tableIndex = m_EntityMap.Get(entity);
	ecs::EntityTable& table = m_Tables[tableIndex];

	const ecs::ComponentId componentId = ToTypeId<TComponent, ecs::ComponentTag>();
	auto* component = table.GetComponent(entity, componentId);
	return *reinterpret_cast<TComponent*>(component);
}

template<typename TComponent>
auto ecs::EntityStorage::TryComponent(const ecs::Entity& entity) -> TComponent*
{
	Z_PANIC(m_EntityMap.Contains(entity), "");

	const int32 tableIndex = m_EntityMap.Get(entity);
	ecs::EntityTable& table = m_Tables[tableIndex];

	const ecs::ComponentId componentId = ToTypeId<TComponent, ecs::ComponentTag>();
	if (auto* component = table.TryComponent(entity, componentId))
		return reinterpret_cast<TComponent*>(component);
	return nullptr;
}