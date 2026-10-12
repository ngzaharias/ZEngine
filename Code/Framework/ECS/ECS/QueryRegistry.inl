#pragma once

template<class TQuery>
const ecs::QueryId ecs::QueryProxy<TQuery>::m_QueryId = ecs::QueryProxy<TQuery>::Id();

template<typename TQuery>
inline ecs::QueryId ecs::QueryProxy<TQuery>::Id()
{
	static bool isRegistered = false;
	if (!isRegistered)
	{
		isRegistered = true;
		const ecs::QueryId queryId = ToTypeId<TQuery, ecs::QueryTag>();

		using AddedAccess = ecs::query::AddedAccess<TQuery>;
		using RemovedAccess = ecs::query::RemovedAccess<TQuery>;
		using UpdatedAccess = ecs::query::UpdatedAccess<TQuery>;
		using IncludeAccess = ecs::query::IncludeAccess<TQuery>;
		using ExcludeAccess = ecs::query::ExcludeAccess<TQuery>;
		using ExternalAccess = TypeDerived<ecs::FrameComponent, IncludeAccess>::type;

		const ecs::ComponentMask addedMask = ecs::ToComponentMask(AddedAccess{});
		const ecs::ComponentMask removedMask = ecs::ToComponentMask(RemovedAccess{});
		const ecs::ComponentMask updatedMask = ecs::ToComponentMask(UpdatedAccess{});
		const ecs::ComponentMask includeMask = ecs::ToComponentMask(IncludeAccess{});
		const ecs::ComponentMask excludeMask = ecs::ToComponentMask(ExcludeAccess{});
		const ecs::ComponentMask externalMask = ecs::ToComponentMask(ExternalAccess{});

		ecs::QueryMask queryMask;
		queryMask.m_ExternIncludeMask = externalMask;

		queryMask.m_TableAddedMask = addedMask;
		queryMask.m_TableRemovedMask = removedMask;
		queryMask.m_TableUpdatedMask = updatedMask;
		queryMask.m_TableIncludeMask = includeMask;
		queryMask.m_TableExcludeMask = excludeMask;

		// entity tables can't contain any external components
		queryMask.m_TableAddedMask.Clear(externalMask);
		queryMask.m_TableRemovedMask.Clear(externalMask);
		queryMask.m_TableUpdatedMask.Clear(externalMask);
		queryMask.m_TableIncludeMask.Clear(externalMask);
		queryMask.m_TableExcludeMask.Clear(externalMask);

		queryMask.m_ConditionAlive = core::Contains(ecs::query::ConditionAccess<TQuery>{}, ecs::Alive{});
		queryMask.m_ConditionDead = core::Contains(ecs::query::ConditionAccess<TQuery>{}, ecs::Dead{});

		// default to alive if not specified
		if (!queryMask.m_ConditionAlive && !queryMask.m_ConditionDead)
			queryMask.m_ConditionAlive = true;

		ecs::QueryMasks& queryMasks = ecs::QueryRegistry::GetMasks();
		Z_PANIC(queryMasks.Find(queryId) == queryMasks.end(), "QueryId already present in QueryRegistry!");

		queryMasks[queryId] = std::move(queryMask);
		return queryId;
	}

	return m_QueryId;
}