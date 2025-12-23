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

		auto begin()
		{
			return QueryIterator<TQuery>{ std::begin(m_Data), m_World };
		}

		auto end()
		{
			return QueryIterator<TQuery>{ std::end(m_Data), m_World };
		}

		ecs::EntityWorld& m_World;
		const ecs::QueryGroup& m_Data;
	};
}