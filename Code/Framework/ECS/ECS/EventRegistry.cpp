#include "ECS/EventRegistry.h"

bool ecs::EventRegistry::IsRegistered(const ecs::EventId id) const
{
	return m_Entries.Contains(id);
}

auto ecs::EventRegistry::GetEntry(const ecs::EventId id) const -> const ecs::EventEntry&
{
	Z_PANIC(IsRegistered(id), "Event isn't registered!");
	return m_Entries.Get(id);
}