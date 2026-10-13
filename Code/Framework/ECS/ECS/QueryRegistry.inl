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

		ecs::QueryMask queryMask;

		// frame components
		{
			using AddedAccess = TypeDerived<ecs::FrameComponent, ecs::query::AddedAccess<TQuery>>::type;
			using IncludeAccess = TypeDerived<ecs::FrameComponent, ecs::query::IncludeAccess<TQuery>>::type;
			using ExcludeAccess = TypeDerived<ecs::FrameComponent, ecs::query::ExcludeAccess<TQuery>>::type;

			// no need to maintain added + include, just store in the same mask
			queryMask.m_FrameIncludeMask |= ecs::ToComponentMask(AddedAccess{});
			queryMask.m_FrameIncludeMask |= ecs::ToComponentMask(IncludeAccess{});
			queryMask.m_FrameExcludeMask = ecs::ToComponentMask(ExcludeAccess{});
		}

		// table components
		{
			using AddedAccess = ecs::query::AddedAccess<TQuery>;
			using RemovedAccess = ecs::query::RemovedAccess<TQuery>;
			using UpdatedAccess = ecs::query::UpdatedAccess<TQuery>;
			using IncludeAccess = ecs::query::IncludeAccess<TQuery>;
			using ExcludeAccess = ecs::query::ExcludeAccess<TQuery>;

			queryMask.m_TableAddedMask = ecs::ToComponentMask(AddedAccess{});
			queryMask.m_TableRemovedMask = ecs::ToComponentMask(RemovedAccess{});
			queryMask.m_TableUpdatedMask = ecs::ToComponentMask(UpdatedAccess{});
			queryMask.m_TableIncludeMask = ecs::ToComponentMask(IncludeAccess{});
			queryMask.m_TableExcludeMask = ecs::ToComponentMask(ExcludeAccess{});

			// entity tables can't contain any frame components
			queryMask.m_TableAddedMask.Clear(queryMask.m_FrameIncludeMask);
			queryMask.m_TableIncludeMask.Clear(queryMask.m_FrameIncludeMask);
			queryMask.m_TableExcludeMask.Clear(queryMask.m_FrameExcludeMask);
		}

		// conditions
		{
			queryMask.m_ConditionAlive = core::Contains(ecs::query::ConditionAccess<TQuery>{}, ecs::Alive{});
			queryMask.m_ConditionDead = core::Contains(ecs::query::ConditionAccess<TQuery>{}, ecs::Dead{});

			// default to alive if not specified
			if (!queryMask.m_ConditionAlive && !queryMask.m_ConditionDead)
				queryMask.m_ConditionAlive = true;
		}

		ecs::QueryMasks& queryMasks = ecs::QueryRegistry::GetMasks();
		Z_PANIC(queryMasks.Find(queryId) == queryMasks.end(), "QueryId already present in QueryRegistry!");

		queryMasks[queryId] = std::move(queryMask);
		return queryId;
	}

	return m_QueryId;
}