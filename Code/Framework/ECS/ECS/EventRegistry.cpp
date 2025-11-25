#include "ECS/EventRegistry.h"

bool ecs::EventRegistry::IsRegistered(const ecs::EventId eventId) const
{
	return m_Entries.Contains(eventId);
}

auto ecs::EventRegistry::GetEntry(const ecs::EventId eventId) const -> const ecs::EventEntry&
{
	Z_PANIC(IsRegistered(eventId), "Event isn't registered!");

	return m_Entries.Get(eventId);
}