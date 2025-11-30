#pragma once

template<class TComponent>
void ecs::ComponentRegistry::Register()
{
	const ecs::ComponentId id = ToTypeIndex<TComponent, ecs::ComponentTag>();
	ecs::ComponentEntry& entry = m_Entries.Emplace(id);
	entry.m_Name = ToTypeName<TComponent>();
}

template<class TComponent>
bool ecs::ComponentRegistry::IsRegistered() const
{
	const ecs::ComponentId id = ToTypeIndex<TComponent, ecs::ComponentTag>();
	return m_Entries.Contains(id);
}

template<class TComponent>
auto ecs::ComponentRegistry::GetEntry() const -> const ecs::ComponentEntry&
{
	const ecs::ComponentId id = ToTypeIndex<TComponent, ecs::ComponentTag>();
	return m_Entries.Get(id);
}