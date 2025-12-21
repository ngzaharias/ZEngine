#pragma once

#include "ECS/EntityView.h"
#include "ECS/QueryRegistry.h"

namespace ecs
{
	class EntityWorld;

	template<typename TRequired, typename TOptional>
	struct QueryIterator
	{
		using EntityView = ecs::EntityView_t<TRequired, TOptional>;
		using Iterator = ecs::QueryGroup::const_iterator;

		auto operator*() -> EntityView
		{
			return EntityView(*m_Iterator, m_World);
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