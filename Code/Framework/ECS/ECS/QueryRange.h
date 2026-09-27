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
		operator const ecs::QueryGroup& () const { return m_Data; }

		static int32 Count(ecs::EntityWorld& entityWorld, const ecs::QueryGroup& queryGroup)
		{
			int32 count = 0;
			auto range = QueryRange<TQuery>{ entityWorld, queryGroup };
			for (auto&& view : range)
				count++;
			return count;
		}

		static bool HasAny(ecs::EntityWorld& entityWorld, const ecs::QueryGroup& queryGroup)
		{
			auto range = QueryRange<TQuery>{ entityWorld, queryGroup };
			return range.begin() != range.end();
		}

		auto begin() const
		{
			return QueryIterator<TQuery>{ m_World, std::begin(m_Data), std::end(m_Data) };
		}

		auto end() const
		{
			return QueryIterator<TQuery>{ m_World, std::end(m_Data), std::end(m_Data)};
		}

		ecs::EntityWorld& m_World;
		const ecs::QueryGroup& m_Data;
	};
}