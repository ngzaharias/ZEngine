#include "ECS/ComponentRegistry.h"

bool ecs::ComponentRegistry::IsRegistered(const ecs::ComponentId componentId) const
{
	return m_Entries.Contains(componentId);
}

auto ecs::ComponentRegistry::GetEntry(const ecs::ComponentId componentId) const -> const ecs::ComponentEntry&
{
	Z_PANIC(IsRegistered(componentId), "Component hasn't been registered!");
	return m_Entries.Get(componentId);
}