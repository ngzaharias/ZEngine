#pragma once

template<class TEvent>
void ecs::EventRegistry::Register()
{
	const ecs::EventId id = ToTypeId<TEvent, ecs::EventTag>();
	ecs::EventEntry& entry = m_Entries.Emplace(id);
	entry.m_Name = ToTypeName<TEvent>();
}

template<class TEvent>
bool ecs::EventRegistry::IsRegistered() const
{
	const ecs::EventId id = ToTypeId<TEvent, ecs::EventTag>();
	return m_Entries.Contains(id);
}

template<class TEvent>
auto ecs::EventRegistry::GetEntry() const -> const ecs::EventEntry&
{
	const ecs::EventId id = ToTypeId<TEvent, ecs::EventTag>();
	return m_Entries.Get(id);
}