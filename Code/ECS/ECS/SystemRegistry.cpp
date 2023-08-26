#include "ECS/SystemRegistry.h"

#include "ECS/System.h"

#include <Core/Algorithms.h>

#include <algorithm>

namespace
{
	bool Comparator(const ecs::SystemEntry* a, const ecs::SystemEntry* b)
	{
		return a->m_Priority < b->m_Priority;
	}
}

ecs::SystemRegistry::SystemRegistry(ecs::EntityWorld& entityWorld)
	: m_EntityWorld(entityWorld)
{
}

void ecs::SystemRegistry::Initialise()
{
	for (ecs::SystemEntry& entry : m_Entries.GetValues())
		m_Priorities.Append(&entry);

	std::sort(m_Priorities.begin(), m_Priorities.end(), Comparator);

	for (ecs::SystemEntry* entry : m_Priorities)
		entry->m_Initialise(m_EntityWorld, *entry->m_System);
}

void ecs::SystemRegistry::Shutdown()
{
	for (auto&& [i, entry] : enumerate::Reverse(m_Priorities))
	{
		entry->m_Shutdown(m_EntityWorld, *entry->m_System);
		delete entry->m_System;
	}
	m_Entries.RemoveAll();
}

void ecs::SystemRegistry::Update(const GameTime& gameTime)
{
	for (ecs::SystemEntry* entry : m_Priorities)
		entry->m_Update(m_EntityWorld, *entry->m_System, gameTime);
}