#pragma once

#include "ECS/QueryIterator.h"
#include "ECS/QueryRegistry.h"

namespace ecs
{
	class EntityWorld;

	template<typename TQuery>
	struct QueryRange
	{
		using Required = ecs::query::IncludeAccess<TQuery>::NonConst;
		using Optional = ecs::query::OptionalAccess<TQuery>::NonConst;

		operator const ecs::QueryGroup& () const { return m_Data; }

		auto begin()
		{
			return QueryIterator<Required, Optional>{ std::begin(m_Data), m_World };
		}

		auto end()
		{
			return QueryIterator<Required, Optional>{ std::end(m_Data), m_World };
		}

		ecs::EntityWorld& m_World;
		const ecs::QueryGroup& m_Data;
	};
}