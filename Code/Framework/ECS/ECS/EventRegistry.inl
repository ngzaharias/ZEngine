#pragma once

template<class TEvent>
void ecs::EventRegistry::Register()
{
	static_assert(!std::is_const<TEvent>::value, "Type must be non-const.");
	static_assert(std::is_base_of<ecs::Event<TEvent>, TEvent>::value, "Type doesn't inherit from ecs::Event.");
	static_assert(std::is_convertible<TEvent*, ecs::Event<TEvent>*>::value, "Event must inherit from ecs::Event using the [public] keyword!");

	Z_PANIC(!IsRegistered<TEvent>(), "Event is already registered!");

	const ecs::EventId eventId = ToTypeIndex<TEvent, ecs::EventTag>();

	ecs::EventEntry& entry = m_Entries.Emplace(eventId);
	entry.m_Name = ToTypeName<TEvent>();
}

template<class TEvent>
bool ecs::EventRegistry::IsRegistered() const
{
	static_assert(!std::is_const<TEvent>::value, "Type must be non-const.");

	const ecs::EventId eventId = ToTypeIndex<TEvent, ecs::EventTag>();
	return m_Entries.Contains(eventId);
}

template<class TEvent>
auto ecs::EventRegistry::GetEntry() const -> const ecs::EventEntry&
{
	static_assert(!std::is_const<TEvent>::value, "Type must be non-const.");

	Z_PANIC(IsRegistered<TEvent>(), "Event isn't registered!");

	const ecs::EventId eventId = ToTypeIndex<TEvent, ecs::EventTag>();
	return m_Entries.Get(eventId);
}