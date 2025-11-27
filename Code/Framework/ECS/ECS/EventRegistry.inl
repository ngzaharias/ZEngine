#pragma once

template<class TEvent>
void ecs::EventRegistry::Register()
{
	static_assert(!std::is_const<TEvent>::value, "Type cannot be const.");
	static_assert(std::is_base_of<ecs::Event<TEvent>, TEvent>::value, "Type doesn't inherit from ecs::Event.");
	static_assert(std::is_convertible<TEvent*, ecs::Event<TEvent>*>::value, "Event must inherit from ecs::Event using the [public] keyword!");

	Z_PANIC(!IsRegistered<TEvent>(), "Event is already registered!");

	const ecs::EventId id = ToTypeIndex<TEvent, ecs::EventTag>();
	ecs::EventEntry& entry = m_Entries.Emplace(id);
	entry.m_Name = ToTypeName<TEvent>();
}

template<class TEvent>
bool ecs::EventRegistry::IsRegistered() const
{
	static_assert(!std::is_const<TEvent>::value, "Type cannot be const.");

	const ecs::EventId id = ToTypeIndex<TEvent, ecs::EventTag>();
	return m_Entries.Contains(id);
}

template<class TEvent>
auto ecs::EventRegistry::GetEntry() const -> const ecs::EventEntry&
{
	static_assert(!std::is_const<TEvent>::value, "Type cannot be const.");
	Z_PANIC(IsRegistered<TEvent>(), "Event isn't registered!");

	const ecs::EventId id = ToTypeIndex<TEvent, ecs::EventTag>();
	return m_Entries.Get(id);
}