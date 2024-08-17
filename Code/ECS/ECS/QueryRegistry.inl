#pragma once

#include "Core/TypeInfo.h"
#include "Core/TypeList.h"
#include "ECS/ComponentMask.h"
#include "ECS/QueryTypes.h"

template<class TQuery>
const ecs::QueryId ecs::QueryProxy<TQuery>::m_QueryId = ecs::QueryProxy<TQuery>::Id();

template <typename TQuery>
inline ecs::QueryId ecs::QueryProxy<TQuery>::Id()
{
	static bool isRegistered = false;
	if (!isRegistered)
	{
		isRegistered = true;
		const ecs::QueryId queryId = ToTypeIndex<TQuery, ecs::QueryTag>();

		ecs::QueryMask queryMask;
		queryMask.m_AddedMask = ecs::ToComponentMask(ecs::query::AddedAccess<TQuery>{});
		queryMask.m_RemovedMask = ecs::ToComponentMask(ecs::query::RemovedAccess<TQuery>{});
		queryMask.m_UpdatedMask = ecs::ToComponentMask(ecs::query::UpdatedAccess<TQuery>{});
		queryMask.m_IncludeMask = ecs::ToComponentMask(ecs::query::IncludeAccess<TQuery>{});
		queryMask.m_ExcludeMask = ecs::ToComponentMask(ecs::query::ExcludeAccess<TQuery>{});

		ecs::QueryMasks& queryMasks = ecs::QueryRegistry::GetMasks();
		Z_PANIC(queryMasks.Find(queryId) == queryMasks.end(), "QueryId already present in QueryRegistry!");

		queryMasks[queryId] = std::move(queryMask);
		return queryId;
	}

	return m_QueryId;
}

template<typename TQuery>
const ecs::QueryGroup& ecs::QueryRegistry::GetGroup() const
{
	static const ecs::QueryId queryId = ecs::QueryProxy<TQuery>::Id();
	const auto find = m_Groups.Find(queryId);
	Z_PANIC(find != m_Groups.end(), "Query hasn't been registered! Ensure that Initialise has been called.");

	return find->second;
}