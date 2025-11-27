#include "ECS/ComponentRegistry.h"

bool ecs::ComponentRegistry::IsRegistered(const ecs::ComponentId id) const
{
	return m_Entries.Contains(id);
}

auto ecs::ComponentRegistry::GetEntry(const ecs::ComponentId id) const -> const ecs::ComponentEntry&
{
	Z_PANIC(IsRegistered(id), "Component isn't registered!");
	return m_Entries.Get(id);
}