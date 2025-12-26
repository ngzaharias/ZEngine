#include "ECS/SystemRegistry.h"

#include "Core/Algorithms.h"
#include "Core/Map.h"
#include "Core/Profiler.h"
#include "Core/Set.h"
#include "ECS/Component.h"
#include "ECS/EntityWorld.h"

#include <algorithm>

namespace
{
	const Set<ecs::SystemId> s_Empty;

	bool Comparator(const ecs::SystemEntry* a, const ecs::SystemEntry* b)
	{
		return a->m_Priority < b->m_Priority;
	}

	// https://www.geeksforgeeks.org/dsa/topological-sort-using-dfs/
	void TopologicalSort(const ecs::EntityWorld& world, const ecs::SystemId parentId, Map<ecs::SystemId, bool>& visited, Array<ecs::SystemId>& stack, const int32 depth)
	{
		const ecs::SystemRegistry& registry = world.m_SystemRegistry;
		const ecs::SystemEdges& edges = registry.GetEdges();
		const ecs::SystemEntries& entries = registry.GetEntries();
		const ecs::SystemEntry& entry = entries.Get(parentId);

		str::String indent;
		for (int32 i = 0; i < depth; ++i)
			indent += '\t';

		Z_LOG(ELog::Debug, "{}{}", indent, entry.m_Name);

		visited.Get(parentId) = true;

		for (const TypeId typeId : entry.m_DependencyWrite)
		{
			const Set<ecs::SystemId>& childIds = edges.Get(typeId, s_Empty);

			if (!childIds.IsEmpty())
			{
				const str::String& name = world.m_TypeMap.Get(typeId);
				Z_LOG(ELog::Debug, "{}\tWrite({})", indent, name);
			}

			for (const ecs::SystemId childId : childIds)
			{
				if (visited.Get(childId))
					continue;

				TopologicalSort(world, childId, visited, stack, depth + 2);
			}
		}

		stack.Append(parentId);
	}

	Array<ecs::SystemId> TopologicalSort(const ecs::EntityWorld& world)
	{
		const ecs::SystemRegistry& registry = world.m_SystemRegistry;
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

			TopologicalSort(world, systemId, visited, stack, 0);
		}

		Array<ecs::SystemId> order;
		order.Reserve(stack.GetCount());
		for (auto&& [i, systemId] : enumerate::Reverse(stack))
			order.Append(stack[i]);
		return order;
	}
}

void ecs::SystemRegistry::Initialise(ecs::EntityWorld& world)
{
	PROFILE_FUNCTION();

	// topological sort systems
	Z_LOG(ELog::Debug, "===== TOPOLOGICAL SORT =====");
	const Array<ecs::SystemId> order = TopologicalSort(world);
	Z_LOG(ELog::Debug, "===== SYSTEM ORDER =====");
	for (const ecs::SystemId systemId : order)
	{
		const auto* entry = m_Order.Append(&m_Entries.Get(systemId));
		Z_LOG(ELog::Debug, "{}", entry->m_Name);
	}

	for (ecs::SystemEntry* entry : m_Order)
		entry->m_Initialise(world, *entry->m_System);
}

void ecs::SystemRegistry::Shutdown(ecs::EntityWorld& world)
{
	PROFILE_FUNCTION();

	for (auto&& [i, entry] : enumerate::Reverse(m_Order))
	{
		entry->m_Shutdown(world, *entry->m_System);
		delete entry->m_System;
	}
	m_Entries.RemoveAll();
}

void ecs::SystemRegistry::Update(ecs::EntityWorld& world, const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	for (ecs::SystemEntry* entry : m_Order)
		entry->m_Update(world, *entry->m_System, gameTime);
}