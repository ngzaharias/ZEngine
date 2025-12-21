#pragma once

#include "ECS/QueryIterator.h"
#include "ECS/QueryRegistry.h"

namespace ecs
{
	class EntityWorld;

	template<typename TQuery>
	struct QueryRange
	{
		operator const ecs::QueryGroup& () const { return m_Data; }

		auto begin() -> QueryIterator<TQuery>
		{
			return QueryIterator<TQuery>{ std::begin(m_Data), m_World };
		}

		auto end() -> QueryIterator<TQuery>
		{
			return QueryIterator<TQuery>{ std::end(m_Data), m_World };
		}

		ecs::EntityWorld& m_World;
		const ecs::QueryGroup& m_Data;
	};
}