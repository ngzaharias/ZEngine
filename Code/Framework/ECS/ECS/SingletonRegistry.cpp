#include "ECS/SingletonRegistry.h"

bool ecs::SingletonRegistry::IsRegistered(const ecs::SingletonId id) const
{
	return m_Entries.Contains(id);
}

auto ecs::SingletonRegistry::GetEntry(const ecs::SingletonId id) const -> const ecs::SingletonEntry&
{
	Z_PANIC(IsRegistered(id), "Singleton isn't registered!");
	return m_Entries.Get(id);
}