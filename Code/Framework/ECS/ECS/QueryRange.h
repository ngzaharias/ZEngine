#pragma once

#include "ECS/QueryIterator.h"
#include "ECS/QueryRegistry.h"

namespace ecs
{
	class EntityWorld;
}

namespace ecs
{
	template<typename TQuery>
	struct QueryRange
	{
	public:
		QueryRange(ecs::EntityWorld& entityWorld, const ecs::QueryGroup& queryGroup, const ecs::QueryMask& queryMask)
			: m_EntityWorld(entityWorld)
			, m_QueryGroup(queryGroup)
			, m_QueryMask(queryMask)
		{
		}

	public:
		auto begin() const
		{
			return QueryIterator<TQuery>{ m_EntityWorld, m_QueryMask, std::begin(m_QueryGroup), std::end(m_QueryGroup) };
		}

		auto end() const
		{
			return QueryIterator<TQuery>{ m_EntityWorld, m_QueryMask, std::end(m_QueryGroup), std::end(m_QueryGroup) };
		}


	public:
		static int32 Count(ecs::EntityWorld& entityWorld, const ecs::QueryGroup& queryGroup, const ecs::QueryMask& queryMask)
		{
			int32 count = 0;
			auto range = QueryRange<TQuery>{ entityWorld, queryGroup, queryMask };
			for (auto&& view : range)
				count++;
			return count;
		}

		static bool HasAny(ecs::EntityWorld& entityWorld, const ecs::QueryGroup& queryGroup, const ecs::QueryMask& queryMask)
		{
			auto range = QueryRange<TQuery>{ entityWorld, queryGroup, queryMask };
			return range.begin() != range.end();
		}

	private:
		ecs::EntityWorld& m_EntityWorld;
		const ecs::QueryGroup& m_QueryGroup;
		const ecs::QueryMask& m_QueryMask;
	};
}