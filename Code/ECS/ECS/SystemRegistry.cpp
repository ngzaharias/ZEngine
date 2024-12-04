#include "ECS/SystemRegistry.h"

#include "ECS/System.h"

#include "Core/Algorithms.h"
#include "Core/Profiler.h"
#include <algorithm>

namespace
{
	bool Comparator(const ecs::SystemEntry* a, const ecs::SystemEntry* b)
	{
		return a->m_Priority < b->m_Priority;
	}
}

void ecs::SystemRegistry::Initialise(ecs::EntityWorld& entityWorld)
{
	PROFILE_FUNCTION();

	for (ecs::SystemEntry& entry : m_Entries.GetValues())
		m_Priorities.Append(&entry);

	std::sort(m_Priorities.begin(), m_Priorities.end(), Comparator);

	for (ecs::SystemEntry* entry : m_Priorities)
		entry->m_Initialise(entityWorld, *entry->m_System);
}

void ecs::SystemRegistry::Shutdown(ecs::EntityWorld& entityWorld)
{
	PROFILE_FUNCTION();

	for (auto&& [i, entry] : enumerate::Reverse(m_Priorities))
	{
		entry->m_Shutdown(entityWorld, *entry->m_System);
		delete entry->m_System;
	}
	m_Entries.RemoveAll();
}

void ecs::SystemRegistry::Update(ecs::EntityWorld& entityWorld, const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	for (ecs::SystemEntry* entry : m_Priorities)
		entry->m_Update(entityWorld, *entry->m_System, gameTime);
}