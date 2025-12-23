#pragma once

#include "ECS/EntityView.h"
#include "ECS/QueryRegistry.h"

namespace ecs
{
	class EntityWorld;

	template<typename TQuery>
	struct QueryIterator
	{
		using Required = ecs::query::IncludeAccess<TQuery>;
		using Optional = ecs::query::OptionalAccess<TQuery>;

		using EntityView = ecs::EntityView_t<Required, Optional>;
		using Iterator = ecs::QueryGroup::const_iterator;

		auto operator*() -> EntityView
		{
			return EntityView(m_World, *m_Iterator);
		}

		auto operator++() -> QueryIterator&
		{
			m_Iterator++;
			return *this;
		}

		bool operator!=(const QueryIterator& rhs) const
		{
			return m_Iterator != rhs.m_Iterator;
		}

		Iterator m_Iterator;
		ecs::EntityWorld& m_World;
	};
}