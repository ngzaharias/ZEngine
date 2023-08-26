#include "ECS/QueryRegistry.h"

#include <Core/Assert.h>

void ecs::QueryRegistry::Initialise()
{
	for (const auto& [queryId, queryMask] : m_Masks)
		m_Groups[queryId];
}

ecs::QueryMasks& ecs::QueryRegistry::GetMasks()
{
	return m_Masks;
}

const ecs::QueryGroup& ecs::QueryRegistry::GetGroup(const ecs::QueryId queryId) const
{
	Z_ASSERT_CRASH(m_Groups.Find(queryId) != m_Groups.end(), "Query hasn't been registered! Ensure that Initialise has been called.");
	return m_Groups.Get(queryId);
}