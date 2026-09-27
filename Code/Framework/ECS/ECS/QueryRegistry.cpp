#include "ECS/QueryRegistry.h"

#include "Core/Assert.h"
#include "Core/Profiler.h"
#include "ECS/EntityLayout.h"

void ecs::QueryRegistry::Initialise()
{
	PROFILE_FUNCTION();

	for (const auto& [queryId, queryMask] : m_Masks)
		m_Groups[queryId];
}

void ecs::QueryRegistry::RegisterTable(const str::Guid& tableId, const ecs::EntityLayout& layout)
{
	for (auto&& [queryId, group] : m_Groups)
	{
		const ecs::QueryMask& queryMask = m_Masks.Get(queryId);
		const bool hasAlive = queryMask.m_ConditionAlive && !layout.m_IsDead;
		const bool hasDead = queryMask.m_ConditionDead && layout.m_IsDead;
		if (!hasAlive && !hasDead)
			continue;
		if (!layout.m_AddedMask.HasAll(queryMask.m_AddedMask))
			continue;
		if (!layout.m_RemovedMask.HasAll(queryMask.m_RemovedMask))
			continue;
		if (!layout.m_IncludeMask.HasAll(queryMask.m_UpdatedMask))
			continue;
		if (!layout.m_IncludeMask.HasAll(queryMask.m_IncludeMask))
			continue;
		if (!layout.m_IncludeMask.HasNone(queryMask.m_ExcludeMask))
			continue;

		group.Add(tableId);
	}
}

void ecs::QueryRegistry::UnregisterTable(const str::Guid& tableId, const ecs::EntityLayout& layout)
{
	for (auto&& [queryId, group] : m_Groups)
		group.Remove(tableId);
}

ecs::QueryMasks& ecs::QueryRegistry::GetMasks()
{
	return m_Masks;
}

const ecs::QueryGroup& ecs::QueryRegistry::GetGroup(const ecs::QueryId queryId) const
{
	Z_PANIC(m_Groups.Contains(queryId), "Query hasn't been registered! Ensure that Initialise has been called.");
	return m_Groups.Get(queryId);
}