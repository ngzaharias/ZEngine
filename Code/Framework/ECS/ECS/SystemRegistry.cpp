#include "ECS/SystemRegistry.h"

#include "Core/Algorithms.h"
#include "Core/Map.h"
#include "Core/Profiler.h"
#include "Core/Set.h"
#include "ECS/Component.h"

#include <algorithm>

namespace
{
	const Set<ecs::SystemId> s_Empty;

	bool Comparator(const ecs::SystemEntry* a, const ecs::SystemEntry* b)
	{
		return a->m_Priority < b->m_Priority;
	}

	// https://www.geeksforgeeks.org/dsa/topological-sort-using-dfs/
	void TopologicalSort(const ecs::SystemRegistry& registry, const ecs::SystemId parentId, Map<ecs::SystemId, bool>& visited, Array<ecs::SystemId>& stack)
	{
		visited.Get(parentId) = true;

		const ecs::SystemEdges& edges = registry.GetEdges();
		const ecs::SystemEntries& entries = registry.GetEntries();

		const ecs::SystemEntry& entry = entries.Get(parentId);
		for (const TypeId typeId : entry.m_DependencyWrite)
		{
			const Set<ecs::SystemId>& childIds = edges.Get(typeId, s_Empty);
			for (const ecs::SystemId childId : childIds)
			{
				if (visited.Get(childId))
					continue;

				TopologicalSort(registry, childId, visited, stack);
			}
		}

		stack.Append(parentId);
	}

	Array<ecs::SystemId> TopologicalSort(const ecs::SystemRegistry& registry)
	{
		const ecs::SystemEdges& edges = registry.GetEdges();
		const ecs::SystemEntries& entries = registry.GetEntries();

		Array<ecs::SystemId> stack;
		stack.Reserve(entries.GetCount());

		Map<ecs::SystemId, bool> visited;
		for (auto&& [systemId, entry] : entries)
			visited[systemId] = false;

		for (auto&& [systemId, entry] : entries)
		{
			if (visited.Get(systemId))
				continue;

			TopologicalSort(registry, systemId, visited, stack);
		}

		Array<ecs::SystemId> order;
		order.Reserve(stack.GetCount());
		for (auto&& [i, systemId] : enumerate::Reverse(stack))
			order.Append(stack[i]);
		return order;
	}
}

void ecs::SystemRegistry::Initialise(ecs::EntityWorld& entityWorld)
{
	PROFILE_FUNCTION();

	// topological sort systems
	Z_LOG(ELog::Debug, "===== SORTING SYSTEMS =====");
	const Array<ecs::SystemId> order = TopologicalSort(*this);
	for (const ecs::SystemId systemId : order)
	{
		const auto* entry = m_Order.Append(&m_Entries.Get(systemId));
		Z_LOG(ELog::Debug, "{}", entry->m_Name);
	}
	//for (ecs::SystemEntry& entry : m_Entries.GetValues())
	//	m_Order.Append(&entry);

	//std::sort(m_Order.begin(), m_Order.end(), Comparator);

	for (ecs::SystemEntry* entry : m_Order)
		entry->m_Initialise(entityWorld, *entry->m_System);
}

void ecs::SystemRegistry::Shutdown(ecs::EntityWorld& entityWorld)
{
	PROFILE_FUNCTION();

	for (auto&& [i, entry] : enumerate::Reverse(m_Order))
	{
		entry->m_Shutdown(entityWorld, *entry->m_System);
		delete entry->m_System;
	}
	m_Entries.RemoveAll();
}

void ecs::SystemRegistry::Update(ecs::EntityWorld& entityWorld, const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	for (ecs::SystemEntry* entry : m_Order)
		entry->m_Update(entityWorld, *entry->m_System, gameTime);
}