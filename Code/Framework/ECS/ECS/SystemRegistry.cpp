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
	const Set<TypeId> s_Empty;

	bool IsDependency(const ecs::ETypeBase value)
	{
		switch (value)
		{
		case ecs::ETypeBase::Component:
		case ecs::ETypeBase::Singleton:
			return true;
		default:
			return false;
		}
	}

	// https://www.geeksforgeeks.org/dsa/topological-sort-using-dfs/
	void TopologicalSort(const ecs::EntityWorld& world, const TypeId parentId, Map<TypeId, bool>& visited, Array<TypeId>& stack, const int32 depth)
	{
		const ecs::SystemRegistry& registry = world.m_SystemRegistry;
		const ecs::SystemDependencies& writes = registry.GetWrites();
		const ecs::SystemDependencies& reads = registry.GetReads();
		const ecs::SystemEntries& entries = registry.GetEntries();
		const ecs::SystemEntry& entry = entries.Get(parentId);

		const ecs::TypeInfo& parentInfo = world.m_TypeMap.Get(parentId);

		visited.Get(parentId) = true;

		// To ensure the order is correct we must recursively add the systems that have a dependency on us.
		// Don't add the systems we depend on otherwise it is possible that a system will be added too early
		// and mess up the order for another system that hasn't been added yet.
		for (const TypeId componentId : entry.m_Write)
		{
			const ecs::TypeInfo& typeInfo = world.m_TypeMap.Get(componentId);
			if (!IsDependency(typeInfo.m_Base))
				continue;

			const Set<TypeId>& childIds = reads.Get(componentId, s_Empty);
			for (const TypeId childId : childIds)
			{
				if (visited.Get(childId))
					continue;

				TopologicalSort(world, childId, visited, stack, depth + 2);
			}
		}

		stack.Append(parentId);
	}

	Array<TypeId> TopologicalSort(const ecs::EntityWorld& world)
	{
		PROFILE_FUNCTION();

		const ecs::SystemRegistry& registry = world.m_SystemRegistry;
		const ecs::SystemEntries& entries = registry.GetEntries();

		Array<TypeId> stack;
		stack.Reserve(entries.GetCount());

		Map<TypeId, bool> visited;
		for (auto&& [typeId, entry] : entries)
			visited[typeId] = false;

		for (auto&& [typeId, entry] : entries)
		{
			if (visited.Get(typeId))
				continue;

			TopologicalSort(world, typeId, visited, stack, 0);
		}

		// systems are added in reverse order
		Array<TypeId> order;
		order.Reserve(stack.GetCount());
		for (auto&& [i, typeId] : enumerate::Reverse(stack))
			order.Append(stack[i]);
		return order;
	}
}

void ecs::SystemRegistry::Initialise(ecs::EntityWorld& world)
{
	PROFILE_FUNCTION();

	// flatten system graph into a sorted order
	const Array<TypeId> order = TopologicalSort(world);
	for (const TypeId typeId : order)
		m_Order.Append(&m_Entries.Get(typeId));

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